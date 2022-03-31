#pragma once
#include "gfx/directx/d3d12.h"
#include <dxgi1_6.h>
#include <wrl.h>

namespace engine::graphics
{
  class window;
  class gfx;
  class sprite;

  namespace detail
  {
    class bitmap;
    class resources;
  }

  class renderer
  {
  private:
    using size_type    = std::size_t;
    using storage_type = std::unordered_map<const gfx*, renderer>;
    using point_type   = utils::vecd2;
    using pixel_type   = utils::vecf2;
    using bitmap       = detail::bitmap;
    using resource_ptr = std::unique_ptr<detail::resources>;
    using sprite_map   = std::unordered_map<size_type, bitmap>;

  public:
    static renderer& get(const gfx* g, const window& wnd) noexcept;
    static void release(const gfx* g) noexcept;

  private:
    static storage_type& storage() noexcept;

  public:
    CLASS_SPECIALS_NONE(renderer);

    ~renderer() noexcept;

    renderer(const window& wnd) noexcept;

    explicit operator bool() const noexcept;

  public:
    void resize() noexcept;
    void init_drawing() noexcept;
    void end_drawing() noexcept;

    //stupid test code
    void image(const sprite& s, const point_type& pos, const point_type& dir) noexcept;

  private:
    void init() noexcept;
    void release() noexcept;

    pixel_type viewport_to_screen(const point_type& v) const noexcept;

    bitmap& to_bitmap(const sprite& s) noexcept;

  private:
    const window& m_wnd;
    resource_ptr m_res;
    sprite_map m_bitmapCache;
  };

}