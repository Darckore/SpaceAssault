#include "game/component_store.hpp"
#include "game/objects/gameobj.hpp"

namespace engine::world
{
  // Private members

  void component_store::attach_to(game_object* owner, component& cmp) const noexcept
  {
    if (owner)
    {
      owner->attach_component(cmp);
    }
  }
  void component_store::remove_from(game_object* owner, id_type id) const noexcept
  {
    if (owner)
    {
      owner->remove_component(id);
    }
  }
}