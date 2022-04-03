#pragma once

namespace engine::world
{
  class game_object;

  class component
  {
  public:
    using time_type  = engine::time_type;
    using id_type    = std::size_t;
    using owner_type = game_object;

  protected:
    static id_type make_id() noexcept;

  public:
    CLASS_SPECIALS_NODEFAULT(component);

    virtual ~component() noexcept;

  protected:
    explicit component(id_type id, owner_type* owner) noexcept;

  public:
    virtual void update(time_type dt) noexcept = 0;

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

    using derived_from = base_component<child_type>;

  public:
    static id_type type_id() noexcept
    {
      static id_type ti = make_id();
      return ti;
    }

  public:
    CLASS_SPECIALS_NODEFAULT(base_component);

    virtual ~base_component() noexcept = default;

  protected:
    explicit base_component(owner_type* owner) noexcept :
      base_type{ type_id(), owner }
    { }
  };

  template <typename C>
  concept game_component = std::is_base_of_v<base_component<C>, C>;
}