
#ifndef NEAR_OBJECT_IDENTITY_TOKEN_PROVIDER_TEST_HXX
#define NEAR_OBJECT_IDENTITY_TOKEN_PROVIDER_TEST_HXX

#include <memory>

#include <nearobject/NearObjectIdentityToken.hxx>

namespace nearobject::test
{
struct NearObjectIdentityTokenTest : 
    public NearObjectIdentityToken
{
    explicit NearObjectIdentityTokenTest(std::array<const std::byte, 4> data = std::array<const std::byte, 4>{std::byte(0x00),std::byte(0x00),std::byte(0x00),std::byte(0x00)}) : 
        m_data{data},
        m_value{m_data}
    {}

    std::span<const std::byte>
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
    const std::array<const std::byte, 4> m_data;
    std::span<const std::byte> m_value;
};
} // namespace nearobject::test

#endif // NEAR_OBJECT_IDENTITY_TOKEN_PROVIDER_TEST_HXX
