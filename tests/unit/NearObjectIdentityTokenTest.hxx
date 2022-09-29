
#ifndef NEAR_OBJECT_IDENTITY_TOKEN_PROVIDER_TEST_HXX
#define NEAR_OBJECT_IDENTITY_TOKEN_PROVIDER_TEST_HXX

#include <cstdint>
#include <memory>

#include <nearobject/NearObjectIdentityToken.hxx>

namespace nearobject::test
{
struct NearObjectIdentityTokenTest : 
    public NearObjectIdentityToken
{
    explicit NearObjectIdentityTokenTest(std::array<const uint8_t, 4> data = std::array<const uint8_t, 4>{0,0,0,0})  : 
        m_data{data},
        m_value{m_data}
    {}

    std::span<const uint8_t>
    GetToken() const noexcept override
    {
        return m_value;
    }

    static std::shared_ptr<NearObjectIdentityTokenTest>
    MakeDefault()
    {
        return std::shared_ptr<NearObjectIdentityTokenTest>();
    }

private:
    const std::array<const uint8_t, 4> m_data;
    std::span<const uint8_t> m_value;
};
} // namespace nearobject::test

#endif // NEAR_OBJECT_IDENTITY_TOKEN_PROVIDER_TEST_HXX
