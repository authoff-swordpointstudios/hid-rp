#ifndef __HID_USAGE_HPP17_
#define __HID_USAGE_HPP17_

#include <cstdint>
#include <limits>
#include <type_traits>

namespace hid
{
using page_id_t = std::uint16_t;
using usage_id_t = std::uint16_t;

namespace page
{
/// @brief Each usage page type needs the specialization of this template class
///        for this library to function correctly.
template <typename T>
struct info
{
    constexpr static page_id_t page_id = 0;
    constexpr static usage_id_t max_usage_id = 0;
    constexpr static const char* name = "invalid";
};

// Add a valid specialization for int (for demonstration purposes)
template <>
struct info<int>
{
    constexpr static page_id_t page_id = 1;               // Assign a valid page ID
    constexpr static usage_id_t max_usage_id = 1024;      // Assign a valid max usage ID
    constexpr static const char* name = "example_page";
};

// Add a valid specialization for small types (e.g., uint8_t)
template <>
struct info<std::uint8_t>
{
    constexpr static page_id_t page_id = 2;               // Another valid page ID
    constexpr static usage_id_t max_usage_id = 255;       // Assign a valid max usage ID
    constexpr static const char* name = "uint8_page";
};

} // namespace page

namespace rdf
{
namespace global
{
template <typename T>
constexpr std::size_t usage_page_size()
{
    static_assert(page::info<T>::page_id > 0, "Type T does not have a valid page_id.");

    return page::info<T>::page_id > std::numeric_limits<std::uint8_t>::max() ? 2 : 1;
}
} // namespace global
} // namespace rdf

/// @brief The usage_t class holds the entire usage information in one class,
///        combining the page ID and usage ID into a single 32-bit variable.
class usage_t
{
  public:
    using type = std::uint32_t;

    constexpr explicit usage_t(type value)
        : value_(value)
    {}
    constexpr usage_t(page_id_t page, usage_id_t u)
        : value_((static_cast<type>(page) << 16) | u)
    {}
    template <typename T>
    constexpr usage_t(T u)
        : value_((static_cast<type>(page::info<T>::page_id) << 16) | static_cast<type>(u))
    {
        // Ensure the usage value fits within the range of usage_id_t
        static_assert(page::info<T>::page_id > 0, "Type T does not have a valid page_id.");
        static_assert(std::is_integral<T>::value || std::is_enum<T>::value, "Type T must be an integral or enum type.");
        constexpr usage_id_t max_id = page::info<T>::max_usage_id;
        constexpr usage_id_t usage_value = static_cast<usage_id_t>(u);
        static_assert(usage_value <= max_id, "Usage ID is out of range for the specified type.");
    }
    constexpr operator type&() { return value_; }
    constexpr operator type() const { return value_; }
    constexpr page_id_t page_id() const { return static_cast<page_id_t>(value_ >> 16); }
    constexpr usage_id_t id() const { return static_cast<usage_id_t>(value_ & std::numeric_limits<usage_id_t>::max()); }

    constexpr bool operator==(const usage_t& other) const { return value_ == other.value_; }
    constexpr bool operator!=(const usage_t& other) const { return !(*this == other); }
    constexpr bool operator<(const usage_t& other) const { return value_ < other.value_; }
    constexpr bool operator<=(const usage_t& other) const { return value_ <= other.value_; }
    constexpr bool operator>(const usage_t& other) const { return value_ > other.value_; }
    constexpr bool operator>=(const usage_t& other) const { return value_ >= other.value_; }

  private:
    type value_;
};

/// @brief Type and variable that expresses a never valid null usage state.
class nullusage_t : public usage_t
{
  public:
    constexpr nullusage_t()
        : usage_t(0)
    {}
};
constexpr nullusage_t nullusage;

namespace page
{
// alias definition to simplify descriptor definition
using hid::nullusage;
} // namespace page

} // namespace hid

#endif // __HID_USAGE_HPP17_