#include <notstd/task_queue.hxx>

#include <array>
#include <condition_variable>
#include <chrono>
#include <memory>
#include <mutex>

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>


using namespace threading;
using namespace nearobject;

namespace nearobject
{
namespace test
{
static constexpr NearObjectCapabilities AllCapabilitiesSupported = {
    true, // SupportsRanging
    true, // SupportsPositioning
    true, // SupportsSecureDevice
    true, // SupportsSecureChannels
};

static const std::vector<std::shared_ptr<NearObject>> NearObjectsContainerEmpty{};
static const std::vector<std::shared_ptr<NearObject>> NearObjectsContainerSingle{ std::make_shared<NearObject>() };
static const std::vector<std::shared_ptr<NearObject>> NearObjectsContainerMultiple{ 
    std::make_shared<NearObject>(), 
    std::make_shared<NearObject>(), 
    std::make_shared<NearObject>(),
    std::make_shared<NearObject>(),
    std::make_shared<NearObject>(),
};

struct NearObjectSessionEventCallbacksNoop :
    public NearObjectSessionEventCallbacks
{
    void
    OnSessionEnded(NearObjectSession *) override
    {}

    void
    OnRangingStarted(NearObjectSession *) override
    {}

    void
    OnRangingStopped(NearObjectSession *) override
    {}

    void
    OnNearObjectPropertiesChanged(NearObjectSession *, const std::vector<std::shared_ptr<NearObject>>) override
    {}

    void
    OnSessionMembershipChanged(NearObjectSession *, const std::vector<std::shared_ptr<NearObject>>, const std::vector<std::shared_ptr<NearObject>>) override
    {}
};

struct NearObjectSessionTest final :
    public NearObjectSession
{
    // Forward base class constructor.
    using NearObjectSession::NearObjectSession;

    // Make some protected members public for testing purposes.
    using NearObjectSession::GetPeers;
    using NearObjectSession::AddNearObjectPeer;
    using NearObjectSession::AddNearObjectPeers;
    using NearObjectSession::RemoveNearObjectPeer;
    using NearObjectSession::RemoveNearObjectPeers;
    using NearObjectSession::NearObjectPropertiesChanged;
    using NearObjectSession::EndSession;
};

} // namespace test
} // namespace nearobject

void assert(bool i){
    if(not i){
        std::cout << "failed assert\n";
    }
}

struct NearObjectSessionEventCallbacksTestMembershipChanged final :
        public test::NearObjectSessionEventCallbacksNoop
    {
        void
        OnSessionMembershipChanged(NearObjectSession * /* session */, const std::vector<std::shared_ptr<NearObject>> nearObjectsAdded, const std::vector<std::shared_ptr<NearObject>> nearObjectsRemoved) override
        {
            bool validationDone = false;
            std::cout << "hello!validate\n";

            if (ValidateAdded) {
                std::cout << "validate added\n";

                assert(nearObjectsAdded == NearObjectsAdded);
                validationDone = true;
            }
            if (ValidateRemoved) {
                std::cout << "validate removed\n";
                assert(nearObjectsRemoved == NearObjectsRemoved);
                validationDone = true;
            }
            std::cout << "validation done\n";
            if (validationDone) {
                ValidationDone = true;
                m_validationDone.notify_all();
            }
        }

        std::vector<std::shared_ptr<NearObject>> NearObjectsAdded{};
        std::vector<std::shared_ptr<NearObject>> NearObjectsRemoved{};

        void
        WaitForValidationComplete()
        {
            auto validationIsDone = [&](){ return ValidationDone; };
            auto validationDoneLock = std::unique_lock{ m_validationDoneGate };
            assert(m_validationDone.wait_for(validationDoneLock, std::literals::chrono_literals::operator ""ms<(char)53, (char)48>(), validationIsDone));
        }

        bool ValidateAdded{ false };
        bool ValidateRemoved{ false };
        bool ValidationDone{ false };

    private:
        std::mutex m_validationDoneGate;
        std::condition_variable m_validationDone;
    };

int
main()
{
    auto callbacks = std::make_shared<NearObjectSessionEventCallbacksTestMembershipChanged>();
    test::NearObjectSessionTest session(test::AllCapabilitiesSupported, std::vector<std::shared_ptr<NearObject>>{}, callbacks);

    callbacks->ValidateAdded = true;
    callbacks->NearObjectsAdded = test::NearObjectsContainerSingle;
    session.AddNearObjectPeer(test::NearObjectsContainerSingle[0]);
    assert(session.GetPeers().size() == callbacks->NearObjectsAdded.size());
    callbacks->WaitForValidationComplete();
}