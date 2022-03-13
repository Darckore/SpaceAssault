#pragma once
#include "gfx/window.hpp"
#include "core/err.hpp"

namespace engine
{
  class core;
}

namespace engine::graphics
{
  class window;
  class renderer;

  class gfx
  {
  public:
    friend class engine::core;

  public:
    class gfx_error : public err::error
    {
    public:
      static constexpr auto prefix = "gfx"sv;

    public:
      using error::error;
    };

  public:
    using viewport_size  = window::dimensions;
    using size_type      = window::size_type;
    using ratio_type     = utils::ratio<size_type>;
    using vertex_type    = utils::vecd2;
    using scale_type     = vertex_type::value_type;

  public:
    CLASS_SPECIALS_NONE(gfx);

    ~gfx() noexcept;

    explicit operator bool() const noexcept;

  private:
    gfx(window& wnd) noexcept;

  public:
    ratio_type aspect() const noexcept;
    size_type width() const noexcept;
    size_type height() const noexcept;

    void bind_scaling(scale_type scale) noexcept;
    void set_origin(vertex_type origin) noexcept;

  public:
    // stupid test code
    void draw(const vertex_type& v1, const vertex_type& v2) noexcept;

  private:
    void begin_frame() noexcept;
    void draw() noexcept;
    void setup() noexcept;

    renderer* get_renderer() noexcept;
    void release_renderer() noexcept;

    ratio_type calc_aspect_ratio() const noexcept;

    vertex_type world_to_viewport(const vertex_type& v) const noexcept;

  private:
    window& m_wnd;
    renderer* m_renderer;
    viewport_size m_size{};
    ratio_type m_aspect{};
    vertex_type m_origin{};
    scale_type m_scale{};
  };
}