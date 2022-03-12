#include "gfx/renderer.hpp"
#include "gfx/gfx.hpp"
#include "gfx/window.hpp"

namespace assault::graphics
{
  // Statics

  renderer::storage_type& renderer::storage() noexcept
  {
    static storage_type storage;
    return storage;
  }

  renderer& renderer::get(const gfx* g, const window& wnd)
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
    m_handle{ reinterpret_cast<HWND>(wnd.handle()) }
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
    auto [viewportWidth, viewportHeight] = m_wnd.size();
    m_oldBuffer = (HBITMAP)SelectObject(m_backBufferDC, m_backBuffer);
    BitBlt(m_backBufferDC, 0, 0, viewportWidth, viewportHeight, nullptr, 0, 0, BLACKNESS);
  }
  void renderer::end_drawing() noexcept
  {
    auto [viewportWidth, viewportHeight] = m_wnd.size();
    BitBlt(m_windowDC, 0, 0, viewportWidth, viewportHeight, m_backBufferDC, 0, 0, SRCCOPY);
    SelectObject(m_backBufferDC, m_oldBuffer);
  }

  // Private members

  void renderer::init() noexcept
  {
    m_windowDC = GetDC(m_handle);
    m_backBufferDC = CreateCompatibleDC(m_windowDC);

    auto [viewportWidth, viewportHeight] = m_wnd.size();
    m_backBuffer = CreateCompatibleBitmap(m_windowDC, viewportWidth, viewportHeight);

    HBITMAP oldBitmap = (HBITMAP)SelectObject(m_backBufferDC, m_backBuffer);
    DeleteObject(oldBitmap);
    DeleteObject((HBRUSH)SelectObject(m_backBufferDC, GetStockObject(HOLLOW_BRUSH)));
  }
  void renderer::release() noexcept
  {
    SelectObject(m_backBufferDC, m_oldBuffer);
    DeleteObject(m_backBuffer);

    DeleteDC(m_backBufferDC);
    m_backBufferDC = nullptr;

    ReleaseDC(m_handle, m_windowDC);
    m_windowDC = nullptr;
  }
  HBITMAP renderer::buffer() noexcept
  {
    return m_backBuffer;
  }
}