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

    // std::make_unique<NearObjectSession>(test::AllCapabilitiesSupported, test::NearObjectsContainerSingle, callbacksNoop);
    NearObjectSession session(capabilities, {}, callbacksNoop);
    session.StartRanging();
    // assert(session.GetCapabilities() == capabilities);
    std::cout << "Waiting 3 seconds for completion\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    session.IsRangingActive();
    // session.StopRanging();

    // std::cout << "<< stopped ranging\n";

    // assert(session.GetCapabilities() == capabilities);
    // auto looper = std::make_unique<CLooper>();

    // std::cout << "Starting looper" << std::endl;
    // // To start and run
    // looper->run();

    // std::this_thread::sleep_for(std::chrono::milliseconds(5));
    // std::cout << (looper->running()? "looper running\n" : "looper not running\n");
    // auto dispatcher = looper->getDispatcher();

    // std::cout << "Adding tasks" << std::endl;

    // for (uint32_t k = 0; k < 50; ++k) {
        // auto const task = [k=10]() {
        //     std::cout << "Invocation " << k
        //               << ": Hello, I have been executed asynchronously on the looper for " << (k + 1)
        //               << " times." << std::endl;
        // };

        // dispatcher->post(std::move(task));

    // }

    // std::cout << "Waiting 5 seconds for completion" << std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(5));

    // std::cout << "Stopping looper" << std::endl;
    // // To stop it and clean it up
    // dispatcher = nullptr;
    // looper->stop();
    // looper = nullptr;
}