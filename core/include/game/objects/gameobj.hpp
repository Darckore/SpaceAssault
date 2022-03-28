#pragma once
#include "game/components/component.hpp"

namespace engine::world
{
  class game_object
  {
  public:
    using component_type = component;
    using id_type = component_type::id_type;
    using data_type = std::unordered_map<id_type, component_type*>;

  public:
    CLASS_SPECIALS_NOCOPY_CUSTOM(game_object);

    virtual ~game_object() noexcept = default;

    game_object() noexcept = default;

    friend class component_store;

  public:
    template <game_component Component>
    const Component* get_component() const noexcept
    {
      auto res = m_data.find(Component::type_id());
      if (res == m_data.end())
      {
        return nullptr;
      }
      return static_cast<const Component*>(res->second);
    }

    template <game_component Component>
    Component* get_component() noexcept
    {
      return utils::mutate(std::as_const(*this).template get_component<Component>());
    }

  private:
    void attach_component(component_type& c) noexcept
    {
      m_data.insert_or_assign(c.id(), &c);
    }
    void remove_component(id_type id) noexcept
    {
      m_data.erase(id);
    }

  private:
    data_type m_data;
  };
}