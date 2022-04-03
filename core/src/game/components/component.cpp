#include "game/components/component.hpp"
#include "game/objects/gameobj.hpp"

namespace engine::world
{
  // Statics

  component::id_type component::make_id() noexcept
  {
    static id_type id{};
    return id++;
  }

  // Special members

  component::~component() noexcept = default;

  component::component(id_type id, owner_type* owner) noexcept :
    m_id{ id },
    m_owner{ owner }
  { }

  // Public members

  component::id_type component::id() const noexcept
  {
    return m_id;
  }
}