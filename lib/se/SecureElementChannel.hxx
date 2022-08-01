
#ifndef __SECURE_ELEMENT_CHANNEL_HXX__
#define __SECURE_ELEMENT_CHANNEL_HXX__

#include <cstdint>

namespace se
{
struct SecureElementChannel
{
    SecureElementChannel(uint8_t channelId);
    ~SecureElementChannel();

    void
    Close();

    const uint8_t ChannelId;
};

} // namespace se

#endif // __SECURE_ELEMENT_CHANNEL_HXX__
