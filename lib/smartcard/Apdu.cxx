
#include "Apdu.hxx"

#include <stdexcept>

using namespace smartcard;

ApduCommand::ApduCommand(std::byte cla, std::byte ins, std::byte p1, std::byte p2):
    ApduCommand(cla, ins, p1, p2, {})
{
}

ApduCommand::ApduCommand(std::byte cla, std::byte ins, std::byte p1, std::byte p2, std::vector<std::byte> data)
{
    // TODO: this doesn't account for all the crazy combinations, so must be updated to handle them.
    m_payload.resize(4 + data.size());
    m_payload[0] = cla;
    m_payload[1] = ins;
    m_payload[2] = p1;
    m_payload[3] = p2;
    m_payload.insert(std::next(std::cbegin(m_payload), 4), std::cbegin(data), std::cend(data));
    m_dataView = gsl::span<std::byte>{ std::next(std::data(m_payload), 4), std::size(data) };
    m_payloadView = gsl::span<std::byte>{ std::next(std::data(m_payload)), std::size(m_payload) };
}

const std::byte
ApduCommand::Cla() const
{
    return m_payload[0];
}

const std::byte
ApduCommand::Ins() const
{
    return m_payload[1];
}

const std::byte
ApduCommand::P1() const
{
    return m_payload[2];
}

const std::byte
ApduCommand::P2() const
{
    return m_payload[3];
}

gsl::span<std::byte> ApduCommand::Data() const
{
    return m_dataView;
}

gsl::span<std::byte> ApduCommand::Payload() const
{
    return m_payloadView;
}

/* static */
ApduCommand
ApduCommand::FromVector(const std::vector<std::byte>& buffer)
{
    throw std::logic_error("not implemented");
}

/* static */
ApduResponse
ApduResponse::FromVector(const std::vector<std::byte>& buffer)
{
    throw std::logic_error("not implemented");
}