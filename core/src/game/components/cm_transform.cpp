#include "game/components/cm_transform.hpp"

namespace engine::world::components
{
  // Special members

  transform::transform(owner_type& owner) noexcept :
    transform{ owner, {}, vector_type::axis_norm<0>() }
  { }

  transform::transform(owner_type& owner, vector_type pos, vector_type dir) noexcept :
    derived_from{ &owner },
    m_position{ pos },
    m_heading{ dir.get_normalised() }
  { }

  // Public members

  const transform::vector_type& transform::position() const noexcept
  {
    return m_position;
  }
  const transform::vector_type& transform::heading() const noexcept
  {
    return m_heading;
  }

  void transform::reposition(const vector_type& to) noexcept
  {
    m_position = to;
  }
  void transform::shift(const vector_type& delta) noexcept
  {
    m_position += delta;
  }
  void transform::head_to(const vector_type& target) noexcept
  {
    m_heading = (target - m_position).get_normalised();
  }
}