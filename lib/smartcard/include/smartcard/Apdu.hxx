
#ifndef APDU_HXX
#define APDU_HXX

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace smartcard
{
class ApduCommand
{
public:
    ApduCommand(std::byte cla, std::byte ins, std::byte p1, std::byte p2);
    ApduCommand(std::byte cla, std::byte ins, std::byte p1, std::byte p2, const std::vector<std::byte>& data);

    const std::byte
    Cla() const;
    const std::byte
    Ins() const;
    const std::byte
    P1() const;
    const std::byte
    P2() const;
    std::span<std::byte>
    Data() const;
    std::span<std::byte>
    Payload() const;

public:
    static ApduCommand
    FromVector(const std::vector<std::byte>& buffer);

private:
    std::span<std::byte> m_dataView;
    std::span<std::byte> m_payloadView;
    std::vector<std::byte> m_payload;
};

class ApduResponse
{
public:
    static ApduResponse
    FromVector(const std::vector<std::byte>& buffer);
};

} // namespace smartcard

#endif // APDU_HXX
