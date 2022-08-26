#include <notstd/task_queue.hxx>

#include <array>
#include <condition_variable>
#include <chrono>
#include <memory>
#include <mutex>

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>

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
    virtual void
    OnSessionEnded(NearObjectSession *) override
    {}

    virtual void
    OnRangingStarted(NearObjectSession *) override
    {}

    virtual void
    OnRangingStopped(NearObjectSession *) override
    {}

    virtual void
    OnNearObjectPropertiesChanged(NearObjectSession *, const std::vector<std::shared_ptr<NearObject>>) override
    {}

    virtual void
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

using namespace threading;
using namespace nearobject;

void assert(bool i){
    if(not i){
        std::cout << "failed assert\n";
    }
}

int
main()
{
    const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
    const auto &capabilities = test::AllCapabilitiesSupported;

    NearObjectSession session(capabilities, {}, callbacksNoop);
    session.StartRanging();
    std::cout << "Waiting 3 seconds for completion\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    session.IsRangingActive();
}