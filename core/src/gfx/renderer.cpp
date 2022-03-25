#include "gfx/renderer.hpp"
#include "gfx/gfx.hpp"
#include "gfx/window.hpp"
#include "core/win_includes.hpp"

namespace engine::graphics
{
  // Additional definitions
  
  namespace detail
  {
    auto active_buf(buffer& owner) noexcept
    {
      return owner.active_buf<HBITMAP>();
    }
    auto prev_buf(buffer& owner) noexcept
    {
      return owner.prev_buf<HBITMAP>();
    }
    auto device(buffer& owner) noexcept
    {
      return owner.device<HDC>();
    }
    auto wnd_device(buffer& owner) noexcept
    {
      return owner.wnd_device<HDC>();
    }
  }

  ////////////////////
  // buffer
  ////////////////////

  void buffer::init_buffers(const window& wnd) noexcept
  {
    auto wndHandle = wnd.handle<HWND>();
    auto windowDC  = GetDC(wndHandle);
    auto backBufferDC = CreateCompatibleDC(windowDC);

    auto [viewportWidth, viewportHeight] = wnd.size();
    auto backBuffer = CreateCompatibleBitmap(windowDC, viewportWidth, viewportHeight);

    HBITMAP oldBitmap = (HBITMAP)SelectObject(backBufferDC, backBuffer);
    DeleteObject(oldBitmap);
    DeleteObject((HBRUSH)SelectObject(backBufferDC, GetStockObject(HOLLOW_BRUSH)));

    m_cur  = backBuffer;
    
    m_wndDevice = windowDC;
    m_device = backBufferDC;
  }
  void buffer::kill_buffers(const window& wnd) noexcept
  {
    auto backBuffer = detail::active_buf(*this);
    auto oldBuffer  = detail::prev_buf(*this);
    
    auto backBufferDC = detail::device(*this);
    auto windowDC     = detail::wnd_device(*this);

    SelectObject(backBufferDC, oldBuffer);
    DeleteObject(backBuffer);

    DeleteDC(backBufferDC);
    m_cur    = nullptr;
    m_device = nullptr;
    m_prev   = oldBuffer;

    auto wndHandle = wnd.handle<HWND>();
    ReleaseDC(wndHandle, windowDC);
    m_wndDevice = nullptr;
  }

  void buffer::clear(const window& wnd) noexcept
  {
    auto [viewportWidth, viewportHeight] = wnd.size();

    auto backBufferDC = detail::device(*this);
    auto backBuffer   = detail::active_buf(*this);
    m_prev = (HBITMAP)SelectObject(backBufferDC, backBuffer);
    BitBlt(backBufferDC, 0, 0, viewportWidth, viewportHeight, nullptr, 0, 0, BLACKNESS);
  }
  void buffer::swap(const window& wnd) noexcept
  {
    auto [viewportWidth, viewportHeight] = wnd.size();

    auto windowDC = detail::wnd_device(*this);
    auto backBufferDC = detail::device(*this);
    auto oldBuffer  = detail::prev_buf(*this);
    BitBlt(windowDC, 0, 0, viewportWidth, viewportHeight, backBufferDC, 0, 0, SRCCOPY);
    SelectObject(backBufferDC, oldBuffer);
  }

  namespace gdi
  {
    ////////////////////
    // pen
    ////////////////////
    
    class pen
    {
    public:
      CLASS_SPECIALS_NODEFAULT(pen);

      pen(buffer& buf, int style, int width, COLORREF c) noexcept :
        m_pen{ CreatePen(style, width, c) },
        m_device{ detail::device(buf) }
      {
        auto old = reinterpret_cast<HPEN>(SelectObject(m_device, m_pen));
        DeleteObject(old);
      }

      ~pen() noexcept
      {
        DeleteObject(m_pen);
      }

      HDC device() noexcept
      {
        return m_device;
      }

    private:
      HPEN m_pen{};
      HDC m_device{};
    };
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
    m_wnd{ wnd }
  {
    init();
  }

  // Public members

  void renderer::reset() noexcept
  {
    release();
    init();
  }

  void renderer::init_drawing() noexcept
  {
    m_buf.clear(m_wnd);
  }
  void renderer::end_drawing() noexcept
  {
    m_buf.swap(m_wnd);
  }

  // Draw stuff

  // stupid test code
  void renderer::line(const point_type& v1, const point_type& v2) noexcept
  {
    gdi::pen pen{ m_buf, PS_SOLID, 5, utils::colour{ 255, 128, 64 } };
    const auto p1 = viewport_to_screen(v1);
    const auto p2 = viewport_to_screen(v2);
    MoveToEx(pen.device(), p1.get<0>(), p1.get<1>(), nullptr);
    LineTo(pen.device(), p2.get<0>(), p2.get<1>());
  }

  // Private members

  void renderer::init() noexcept
  {
    m_buf.init_buffers(m_wnd);
  }
  void renderer::release() noexcept
  {
    m_buf.kill_buffers(m_wnd);
  }

  renderer::pixel_type renderer::viewport_to_screen(const point_type& v) const noexcept
  {
    using coord_t = pixel_type::value_type;
    const auto [w, h] = m_wnd.size();
    const auto x = static_cast<coord_t>(v.get<0>()) + w / 2;
    const auto y = h / 2 - static_cast<coord_t>(v.get<1>());
    return { x, y };
  }
}