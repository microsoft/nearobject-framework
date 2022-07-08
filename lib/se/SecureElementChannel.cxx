
#include "SecureElementChannel.hxx"

using namespace se;

SecureElementChannel::SecureElementChannel(uint8_t channelId) :
    ChannelId(channelId)
{
}

SecureElementChannel::~SecureElementChannel()
{
    Close();
}

void
SecureElementChannel::Close()
{
}
