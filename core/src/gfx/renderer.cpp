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
  void renderer::line(const utils::vecd2& v1, const utils::vecd2& v2) noexcept
  {
    auto pen = CreatePen(PS_SOLID, 5, 0x0000FF00);
    auto device = detail::device(m_buf);
    DeleteObject((HPEN)SelectObject(detail::device(m_buf), pen));

    utils::point2d p1 = v1;
    utils::point2d p2 = v2;
    MoveToEx(device, p1.get<0>(), p1.get<1>(), nullptr);
	  LineTo(device, p2.get<0>(), p2.get<1>());

    DeleteObject(pen);
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
}