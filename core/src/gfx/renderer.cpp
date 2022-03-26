#include "gfx/renderer.hpp"
#include "gfx/gfx.hpp"
#include "gfx/window.hpp"
#include "core/win_includes.hpp"

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1")

namespace engine::graphics
{
  ////////////////////
  // buffer
  ////////////////////

  // Addditional definitions

  namespace detail
  {
    static inline auto d2d_factory(resource_manager::handle_type f) noexcept
    {
      return reinterpret_cast<ID2D1Factory*>(f);
    }
    static inline auto render_target(resource_manager::handle_type t) noexcept
    {
      return reinterpret_cast<ID2D1HwndRenderTarget*>(t);
    }
    static inline auto brush(resource_manager::handle_type b) noexcept
    {
      return reinterpret_cast<ID2D1SolidColorBrush*>(b);
    }
    static inline auto wnd_size(const window& wnd) noexcept
    {
      auto [w, h] = wnd.size();
      return D2D1::SizeU(static_cast<UINT32>(w), static_cast<UINT32>(h));
    }

    template <typename I>
    void release(I** resource) noexcept
    {
      if (*resource)
      {
        (*resource)->Release();
        (*resource) = nullptr;
      }
    }
  }

  // Special members

  resource_manager::resource_manager(const window& wnd) noexcept :
    m_wnd{ wnd }
  {
    if (FAILED(CoInitialize(nullptr)))
    {
      // todo: error
      return;
    }

    init();
  }

  resource_manager::~resource_manager() noexcept
  {
    kill();
    CoUninitialize();
  }

  resource_manager::operator bool() const noexcept
  {
    return static_cast<bool>(m_target);
  }

  // Private members

  void resource_manager::init() noexcept
  {
    if (!m_wnd)
      return;

    auto factory = detail::d2d_factory(m_d2dFactory);
    
    auto res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
    if (FAILED(res))
    {
      // todo: error
      kill();
      return;
    }
    m_d2dFactory = factory;

    auto target = detail::render_target(m_target);  
    auto wndHandle = m_wnd.handle<HWND>();
    auto size = detail::wnd_size(m_wnd);

    res = factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                                          D2D1::HwndRenderTargetProperties(wndHandle, size),
                                          &target);
    m_target = target;

    if (FAILED(res))
    {
      // todo: error
      kill();
      return;
    }
  }
  void resource_manager::resize() noexcept
  {
    auto size = detail::wnd_size(m_wnd);
    detail::render_target(m_target)->Resize(size);
  }
  void resource_manager::kill() noexcept
  {
    auto f = detail::d2d_factory(m_d2dFactory);
    detail::release(&f);
    m_d2dFactory = nullptr;

    auto t = detail::render_target(m_target);
    detail::release(&t);
    m_target = nullptr;

    auto b = detail::brush(m_brush);
    detail::release(&b);
    m_brush = nullptr;
  }

  void resource_manager::clear() noexcept
  {
    auto target = detail::render_target(m_target);
    if (!target)
    {
      return;
    }

    target->BeginDraw();
    target->Clear(D2D1_COLOR_F{ D2D1::ColorF::Black });
  }
  void resource_manager::swap() noexcept
  {
    auto target = detail::render_target(m_target);
    if (!target)
    {
      return;
    }
    
    target->EndDraw();
  }


  ////////////////////
  // renderer
  ////////////////////

  // Statics

  renderer::storage_type& renderer::storage() noexcept
  {
    static storage_type storage;
    return storage;
  }

  renderer& renderer::get(const gfx* g, const window& wnd) noexcept
  {
    auto&& store = storage();
    if (auto item = store.find(g); item != store.end())
    {
      return item->second;
    }

    return store.emplace(g, wnd).first->second;
  }
  void renderer::release(const gfx* g) noexcept
  {
    auto&& store = storage();
    store.erase(g);
  }

  // Special members

  renderer::~renderer() noexcept
  {
    release();
  }

  renderer::renderer(const window& wnd) noexcept :
    m_wnd{ wnd },
    m_resMgr{ wnd }
  {
  }

  renderer::operator bool() const noexcept
  {
    return m_wnd && m_resMgr;
  }

  // Public members

  void renderer::resize() noexcept
  {
    m_resMgr.resize();
  }

  void renderer::init_drawing() noexcept
  {
    m_resMgr.clear();
  }
  void renderer::end_drawing() noexcept
  {
    m_resMgr.swap();
  }

  // Draw stuff

  // stupid test code
  void renderer::line(const point_type& v1, const point_type& v2) noexcept
  {
    auto target = detail::render_target(m_resMgr.m_target);
    if (!target)
      return;

    auto brush = detail::brush(m_resMgr.m_brush);
    if (!brush)
    {
      target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Coral), &brush);
      m_resMgr.m_brush = brush;
    }
    
    auto p1 = viewport_to_screen(v1);
    auto p2 = viewport_to_screen(v2);
    target->DrawLine(D2D1::Point2F(p1[0], p1[1]),
                     D2D1::Point2F(p2[0], p2[1]),
                     brush,
                     5.0f);
  }

  // Private members

  void renderer::init() noexcept
  {
    m_resMgr.init();
  }
  void renderer::release() noexcept
  {
    m_resMgr.kill();
  }

  renderer::pixel_type renderer::viewport_to_screen(const point_type& v) const noexcept
  {
    using coord_t = pixel_type::value_type;
    const auto [w, h] = m_wnd.size();
    const auto x = static_cast<coord_t>(v[0]) + w / 2;
    const auto y = h / 2 - static_cast<coord_t>(v[1]);
    return { x, y };
  }
}