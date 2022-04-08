#pragma once
#include "core/singleton_base.hpp"
#include "gfx/window.hpp"
#include "gfx/renderer.hpp"

namespace engine
{
  namespace detail
  {
    template <typename System>
    concept engine_system = std::is_base_of_v<singleton<System>, System>;
  }

  class systems
  {
  public:
    CLASS_SPECIALS_NONE(systems);

    static auto& window() noexcept
    {
      using res_type = graphics::window;
      return singleton<res_type>::get();
    }

  private:
    friend class core;

    template <detail::engine_system Sys, typename ...Args>
    static bool init_system(Args&& ...args) noexcept
    {
      return singleton<Sys>::create(std::forward<Args>(args)...);
    }

    template <detail::engine_system Sys>
    static void shutdown_system() noexcept
    {
      singleton<Sys>::shutdown();
    }
  };
}