#include "game/component_store.hpp"

namespace engine::world
{
  namespace detail
  {
    base_cmp_collection::base_cmp_collection(id_type id) noexcept :
      m_id{ id }
    { }

    base_cmp_collection::id_type base_cmp_collection::id() const noexcept
    {
      return m_id;
    }
  }

  // Special members

  component_store::component_store() noexcept = default;

}