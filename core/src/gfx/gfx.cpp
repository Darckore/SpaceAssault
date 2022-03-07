#include "gfx/gfx.h"
#include "core/win_includes.h"

namespace assault::graphics
{
  // Renderer

  class renderer
  {
  public:
    using storage_type = std::unordered_map<const gfx*, renderer>;
    static storage_type& storage() noexcept
    {
      static storage_type storage;
      return storage;
    }

  public:
    CLASS_SPECIALS_NONE(renderer);

    ~renderer() noexcept
    {
      release();
    }

    renderer(const window& wnd) noexcept :
      m_wnd{ wnd },
      m_handle{ reinterpret_cast<HWND>(wnd.handle()) }
    {
      init();
    }

  public:
    void reset() noexcept
    {
      release();
      init();
    }

    HBITMAP buffer() noexcept
    {
      return m_backBuffer;
    }
    void init_drawing() noexcept
    {
      auto [viewportWidth, viewportHeight] = m_wnd.size();
      m_oldBuffer = (HBITMAP)SelectObject(m_backBufferDC, m_backBuffer);
      BitBlt(m_backBufferDC, 0, 0, viewportWidth, viewportHeight, nullptr, 0, 0, BLACKNESS);
    }
    void end_drawing() noexcept
    {
      auto [viewportWidth, viewportHeight] = m_wnd.size();
      BitBlt(m_windowDC, 0, 0, viewportWidth, viewportHeight, m_backBufferDC, 0, 0, SRCCOPY);
      SelectObject(m_backBufferDC, m_oldBuffer);
    }

  private:
    void init() noexcept
    {
      m_windowDC = GetDC(m_handle);
      m_backBufferDC = CreateCompatibleDC(m_windowDC);

      auto [viewportWidth, viewportHeight] = m_wnd.size();
      m_backBuffer = CreateCompatibleBitmap(m_windowDC, viewportWidth, viewportHeight);

      HBITMAP oldBitmap = (HBITMAP)SelectObject(m_backBufferDC, m_backBuffer);
      DeleteObject(oldBitmap);
      DeleteObject((HBRUSH)SelectObject(m_backBufferDC, GetStockObject(HOLLOW_BRUSH)));
    }
    void release() noexcept
    {
      SelectObject(m_backBufferDC, m_oldBuffer);
      DeleteObject(m_backBuffer);

      DeleteDC(m_backBufferDC);
      m_backBufferDC = nullptr;

      ReleaseDC(m_handle, m_windowDC);
      m_windowDC = nullptr;
    }

  private:
    const window& m_wnd;
    HWND m_handle{};
    HDC m_windowDC{};
    HDC m_backBufferDC{};
    HBITMAP m_backBuffer{};
    HBITMAP m_oldBuffer{};
  };

  // Special members

  gfx::~gfx() noexcept
  {
    release_renderer();
  }

  gfx::gfx(window& wnd) :
    m_wnd{ wnd },
    m_render{ get_renderer() },
    m_viewPort{ wnd.size() }
  { }

  // Public members

  void gfx::begin_frame() noexcept
  {
    setup();
    m_render.init_drawing();
  }
  void gfx::draw() noexcept
  {
    m_render.end_drawing();
  }
  void gfx::setup() noexcept
  {
    auto wndSize = m_wnd.size();
    if (wndSize == m_viewPort)
    {
      return;
    }

    m_viewPort = wndSize;
    m_render.reset();
  }

  // Private members

  renderer& gfx::get_renderer()
  {
    auto&& res_storage = renderer::storage();
    if (auto item = res_storage.find(this); item != res_storage.end())
    {
      return item->second;
    }

    return res_storage.emplace(this, m_wnd).first->second;
  }
  void gfx::release_renderer()
  {
    auto&& res_storage = renderer::storage();
    res_storage.erase(this);
  }

}