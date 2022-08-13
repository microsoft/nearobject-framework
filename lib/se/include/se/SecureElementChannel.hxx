
#ifndef SECURE_ELEMENT_CHANNEL_HXX
#define SECURE_ELEMENT_CHANNEL_HXX

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

#endif // SECURE_ELEMENT_CHANNEL_HXX
