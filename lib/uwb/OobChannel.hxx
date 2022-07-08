
#ifndef __OOB_CHANNEL_HXX__
#define __OOB_CHANNEL_HXX__

#include "OobConnection.hxx"

#include <memory>

namespace uwb
{

class OobChannel
{
public:
    virtual std::unique_ptr<OobConnection>
    EstablishConnection() = 0;
};

} // namespace uwb

#endif // __OOB_CHANNEL_HXX__