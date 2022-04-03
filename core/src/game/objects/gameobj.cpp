#include "game/objects/gameobj.hpp"

namespace engine::world
{
  // Private members

  void game_object::attach_component(component_type& c) noexcept
  {
    m_data.insert_or_assign(c.id(), &c);
  }
  void game_object::remove_component(id_type id) noexcept
  {
    m_data.erase(id);
  }
  void game_object::remove_all() noexcept
  {
    m_data.clear();
  }
}