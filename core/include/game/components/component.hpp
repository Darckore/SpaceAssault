#pragma once

namespace engine::world
{
  class game_object;

  class component
  {
  protected:
    using id_type = std::size_t;
    using owner_type = game_object;

  protected:
    static id_type make_id() noexcept;

  public:
    CLASS_SPECIALS_NODEFAULT_NOCOPY(component);

    virtual ~component() noexcept;

  protected:
    explicit component(id_type id, owner_type* owner) noexcept;

  public:


  public:
    id_type id() const noexcept;

  private:
    id_type m_id;
    owner_type* m_owner{};
  };

  template <typename Derived>
  class base_component : public component
  {
  public:
    using base_type  = component;
    using child_type = Derived;
    using id_type = base_type::id_type;
    using owner_type = base_type::owner_type;
    using derived_from = base_component<child_type>;

  public:
    static id_type type_id() noexcept
    {
      static id_type ti = make_id();
      return ti;
    }

  public:
    CLASS_SPECIALS_NOCOPY_CUSTOM(base_component);

  protected:
    explicit base_component(owner_type* owner) noexcept :
      base_type{ type_id(), owner }
    { }
  };
}