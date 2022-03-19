#pragma once
#include "gfx/gfx.hpp"

namespace engine::graphics
{
  class camera
  {
  public:
    using gfx_type    = gfx;
    using dist_type   = double;
    using aspect_type = gfx::ratio_type;

  public:
    CLASS_SPECIALS_NODEFAULT(camera);

    explicit camera(gfx_type& g) noexcept;

    explicit operator bool() const noexcept;

  public:
    void resize(dist_type fieldSize) noexcept;

    void update() noexcept;

  public: // eventually private
    gfx_type& gfx() noexcept;

  private:

  private:
    gfx_type* m_gfx{};
    aspect_type m_aspect{};
    dist_type m_fieldSize{};
  };
}