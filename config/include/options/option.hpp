#pragma once
#include "options/value.hpp"

namespace engine::config
{
  class section;

  class option
  {
  public:
    using value_type = value;
    using name_type  = std::string_view;
    using val_store  = std::vector<value_type>;
    using size_type  = val_store::size_type;

  public:
    CLASS_SPECIALS_NODEFAULT_NOCOPY(option);

    option(name_type name, section& parent) noexcept;

    auto begin() const noexcept
    {
      return m_values.begin();
    }
    auto end() const noexcept
    {
      return m_values.end();
    }
    auto begin() noexcept
    {
      return m_values.begin();
    }
    auto end() noexcept
    {
      return m_values.end();
    }
    
    auto& operator[](size_type index) const noexcept
    {
      return m_values[index];
    }
    auto& operator[](size_type index) noexcept
    {
      return utils::mutate(std::as_const(*this).operator[](index));
    }

  public:
    void add_value(detail::val_type auto v)
    {
      m_values.emplace_back(v);
    }

    name_type name() const noexcept;

    section& parent() noexcept;
    const section& parent() const noexcept;

    size_type size() const noexcept;

  private:
    section*  m_parent{};
    name_type m_name;
    val_store m_values;
  };
}