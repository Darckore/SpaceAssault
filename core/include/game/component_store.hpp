#pragma once
#include "game/components/component.hpp"

namespace engine::world
{
  namespace detail
  {
    class base_cmp_collection
    {
    public:
      using id_type = component::id_type;

    public:
      CLASS_SPECIALS_NONE_CUSTOM(base_cmp_collection);

      virtual ~base_cmp_collection() noexcept = default;

      virtual void remove(game_object* owner) noexcept = 0;

    protected:
      base_cmp_collection() noexcept = default;
    };

    template <game_component Component>
    class component_collection : public base_cmp_collection
    {
    public:
      using value_type = Component;
      using data_type = std::unordered_map<game_object*, value_type>;

    public:
      CLASS_SPECIALS_NONE_CUSTOM(component_collection);

      virtual ~component_collection() noexcept = default;

      component_collection() noexcept = default;

    public:
      template <typename ...Args>
      value_type& add(game_object* owner, Args&& ...args) noexcept
      {
        return m_data.try_emplace(owner, std::forward<Args>(args)...).first->second;
      }
      
      virtual void remove(game_object* owner) noexcept override
      {
        m_data.erase(owner);
      }

    private:
      data_type m_data;
    };

  }

  class component_store
  {
  public:
    using value_type = detail::base_cmp_collection;
    using ptr_type   = std::unique_ptr<value_type>;
    using id_type    = value_type::id_type;
    using data_type  = std::map<id_type, ptr_type>;

    template <game_component Component>
    using collection = detail::component_collection<Component>;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(component_store);

    virtual ~component_store() noexcept = default;

    component_store() noexcept = default;

  public:
    template <game_component Component, typename ...Args>
    Component& add(game_object* owner, Args&& ...args) noexcept
    {
      auto&& cmp = get_collection<Component>().add(owner, owner, std::forward<Args>(args)...);
      attach_to(owner, cmp);
      return cmp;
    }

    template <game_component Component>
    void remove(game_object* owner) noexcept
    {
      remove_from(owner, Component::type_id());
      return get_collection<Component>().remove(owner);
    }

    void remove_all(game_object* owner) noexcept;

  private:
    template <game_component Component>
    collection<Component>& get_collection() noexcept
    {
      using res_type = collection<Component>;

      auto&& item = m_data[Component::type_id()];
      if (!item)
      {
        item = std::make_unique<res_type>();
      }
      return static_cast<res_type&>(*item);
    }

    void attach_to(game_object* owner, component& cmp) const noexcept;
    void remove_from(game_object* owner, id_type id) const noexcept;

  private:
    data_type m_data;
  };
}