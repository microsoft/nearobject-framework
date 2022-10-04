
#ifndef NOTSTD_TYPE_TRAITS
#define NOTSTD_TYPE_TRAITS

#include <type_traits>
#include <variant>

namespace notstd
{
/**
 * @brief Determines if a candidate type is one of the alternative types in the
 * specified variant aggregate type.
 * 
 * This is the base version which defaults to returning false.
 * 
 * @tparam typename 
 * @tparam typename 
 */
template <typename, typename>
constexpr bool is_variant_alternative = std::false_type::value;

/**
 * @brief Determines if a candidate type is one of the alternative types in the
 * specified variant aggregate type.
 * 
 * This is the specialization version that actually performs the check on a variant. 
 * 
 * @tparam VariantTypes The variant aggregate type to check against.
 * @tparam CandidateType The type to check.
 */
template <typename... VariantTypes, typename CandidateType>
inline constexpr bool is_variant_alternative<std::variant<VariantTypes...>, CandidateType> = 
    (std::is_same_v<CandidateType, VariantTypes> || ...);
}

#endif // NOTSTD_TYPE_TRAITS
