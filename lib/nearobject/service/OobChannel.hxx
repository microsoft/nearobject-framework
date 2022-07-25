
#ifndef __OOB_CHANNEL_HXX__
#define __OOB_CHANNEL_HXX__

#include "OobConnection.hxx"

#include <memory>

namespace nearobject
{
class OobChannel
{
public:
    virtual ~OobChannel() = default;

    std::unique_ptr<OobConnection>
    EstablishConnection();

private:
    virtual std::unique_ptr<OobConnection>
    EstablishConnectionImpl() = 0;
};

} // namespace nearobject

#endif // __OOB_CHANNEL_HXX__
