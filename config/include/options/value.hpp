#pragma once

namespace engine::config
{
  namespace detail
  {
    template <typename T>
    concept val_type =
      !std::is_same_v<T, char>  &&
       (std::is_arithmetic_v<T> ||
        std::is_same_v<T, std::string_view>);
  }

  class value
  {
  public:
    using bool_val   = bool;
    using int_val    = std::int64_t;
    using float_val  = double;
    using str_val    = std::string_view;
    using value_type = std::variant<
                                     bool_val,
                                     int_val,
                                     float_val,
                                     str_val
                                   >;

  public:
    CLASS_SPECIALS_NODEFAULT(value);

    constexpr value(detail::val_type auto v) noexcept :
      m_val{ v }
    { }

    value& operator=(detail::val_type auto v) noexcept
    {
      m_val = v;
    }

    template <detail::val_type T>
    bool is() const noexcept
    {
      return std::holds_alternative<T>(m_val);
    }

    template <detail::val_type T>
    auto& get() noexcept
    {
      return std::get<T>(m_val);
    }

    template <detail::val_type T>
    auto try_get() noexcept
    {
      if (is<T>())
        return &get<T>();

      using ret_t = T*;
      return ret_t{};
    }

  private:
    value_type m_val;
  };
}