
#ifndef APDU_HXX
#define APDU_HXX

#include <cstddef>
#include <cstdint>
#include <vector>

#include <gsl/span>

namespace smartcard
{
class ApduCommand
{
public:
    ApduCommand(std::byte cla, std::byte ins, std::byte p1, std::byte p2);
    ApduCommand(std::byte cla, std::byte ins, std::byte p1, std::byte p2, std::vector<std::byte> data);

    const std::byte
    Cla() const;
    const std::byte
    Ins() const;
    const std::byte
    P1() const;
    const std::byte
    P2() const;
    gsl::span<std::byte>
    Data() const;
    gsl::span<std::byte>
    Payload() const;

public:
    static ApduCommand
    FromVector(const std::vector<std::byte>& buffer);

private:
    gsl::span<std::byte> m_dataView;
    gsl::span<std::byte> m_payloadView;
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
