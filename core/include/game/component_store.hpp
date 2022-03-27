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

    protected:
      explicit base_cmp_collection(id_type id) noexcept;

    public:
      id_type id() const noexcept;

    private:
      id_type m_id;
    };

    template <typename Component>
      requires (std::is_base_of_v<base_component<Component>, Component>)
    class component_collection : public base_cmp_collection
    {
    public:
      using value_type = Component;

    public:
      CLASS_SPECIALS_NONE_CUSTOM(component_collection);

      component_collection() noexcept :
        base_cmp_collection{ value_type::type_id() }
      { }

    private:

    };

  }

  class component_store
  {
  public:
    using value_type = detail::base_cmp_collection;
    using ptr_type   = std::unique_ptr<value_type>;
    using id_type    = value_type::id_type;
    using data_type  = std::map<id_type, ptr_type>;

  public:
    CLASS_SPECIALS_NONE_CUSTOM(component_store);

    component_store() noexcept;

  private:
    data_type m_data;
  };
}