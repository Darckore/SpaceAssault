#pragma once
#include "game/components/component.hpp"

namespace engine::world::components
{
  class transform : public base_component<transform>
  {
  public:
    using vector_type = utils::vecd2;

  public:
    CLASS_SPECIALS_NODEFAULT(transform);

    virtual ~transform() = default;

    explicit transform(owner_type& owner) noexcept;

    transform(owner_type& owner, vector_type pos, vector_type dir) noexcept;

  public:
    const vector_type& position() const noexcept;
    const vector_type& heading() const noexcept;

    void reposition(const vector_type& to)  noexcept;
    void shift(const vector_type& delta)    noexcept;
    void head_to(const vector_type& target) noexcept;

  private:
    virtual void update(time_type) noexcept override{}

  private:
    vector_type m_position;
    vector_type m_heading;
  };
}