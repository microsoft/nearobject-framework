
#include <nearobject/NearObjectIdentityTokenUwb.hxx>
#include <nearobject/serialization/NearObjectIdentityTokenUwbJsonSerializer.hxx>

using namespace nearobject;

NearObjectIdentityTokenUwb::NearObjectIdentityTokenUwb(uwb::UwbMacAddress uwbMacAddress) :
    m_uwbMacAddress(std::move(uwbMacAddress)),
    m_token(ToToken(*this))
{
}

/* static */
NearObjectIdentityTokenUwb
NearObjectIdentityTokenUwb::FromToken(std::span<const uint8_t> token)
{
    const auto data = nlohmann::json::from_msgpack(token);
    const auto json = nlohmann::json(data);
    auto identityTokenUwb = json.get<NearObjectIdentityTokenUwb>();
    return identityTokenUwb;
}

/* static */
std::vector<uint8_t>
NearObjectIdentityTokenUwb::ToToken(const NearObjectIdentityTokenUwb& identityTokenUwb)
{
    auto json = nlohmann::json(identityTokenUwb);
    auto data = nlohmann::json::to_msgpack(json);
    return data;
}

/* static */
std::unique_ptr<NearObjectIdentityToken>
NearObjectIdentityTokenUwb::UniqueFromToken(std::span<const uint8_t> token)
{
    // TODO: seems pretty dumb to have to allocate the object first, then
    // copy/move a serialized object into it. nlohmann must support some way of
    // dynamically allocating the object instead. Needs further investigation.
    auto instance = std::make_unique<NearObjectIdentityTokenUwb>();
    *instance = std::move(NearObjectIdentityTokenUwb::FromToken(token));
    return instance;
}

std::span<const uint8_t>
NearObjectIdentityTokenUwb::GetToken() const noexcept
{
    return m_token;
}

std::string
NearObjectIdentityTokenUwb::ToString() const
{
    return "uwb:" + NearObjectIdentityToken::ToString();
}

bool
NearObjectIdentityTokenUwb::IsEqual(const NearObjectIdentityToken& other) const noexcept
{
    // This cast is safe since the operator==() implementation guarantees the
    // type of 'other' to be 'NearObjectIdentityTokenUwb' using a typeid check.
    const auto rhs = static_cast<const NearObjectIdentityTokenUwb&>(other);

    // Forward base class checks, then validate specifics from this class.
    return NearObjectIdentityToken::IsEqual(other) && rhs.GetMacAddress() == this->GetMacAddress();
}

const uwb::UwbMacAddress
NearObjectIdentityTokenUwb::GetMacAddress() const noexcept
{
    return m_uwbMacAddress;
}
