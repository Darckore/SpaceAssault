#pragma once
#include "game/base_game.hpp"
#include "gfx/camera.hpp"

namespace engine::world
{
  class scene
  {
  public:
    enum class status : uint8_t
    {
      ready,
      loading,
      error
    };

  public:
    using owner_type  = i_game;
    using gfx_type    = owner_type::graphics_type;
    using time_type   = owner_type::time_type;
    using camera_type = graphics::camera;

  public:
    CLASS_SPECIALS_NONE(scene);

    scene(owner_type& owner, camera_type cam) noexcept;

    explicit operator bool() const noexcept;

  public:
    virtual void update(time_type dt) noexcept = 0;
    virtual void render() noexcept = 0;

  private:
    virtual bool init() noexcept = 0;

  public:
    bool is_loading() const noexcept;

    camera_type& camera() noexcept;
    owner_type& game() noexcept;

    bool load() noexcept;

  private:
    void loading() noexcept;
    void ok() noexcept;
    void error() noexcept;

  private:
    owner_type& m_owner;
    camera_type m_cam;
    status m_status{ status::error };
  };
}