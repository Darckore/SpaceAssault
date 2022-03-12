#pragma once
#include "core/win_includes.hpp"

namespace assault::graphics
{
  class window;
  class gfx;

  class renderer
  {
  private:
    using storage_type = std::unordered_map<const gfx*, renderer>;
    static storage_type& storage() noexcept;

  public:
    static renderer& get(const gfx* g, const window& wnd);
    static void release(const gfx* g) noexcept;

  public:
    CLASS_SPECIALS_NONE(renderer);

    ~renderer() noexcept;

    renderer(const window& wnd) noexcept;

  public:
    void reset() noexcept;

    void init_drawing() noexcept;
    void end_drawing() noexcept;

  private:
    void init() noexcept;
    void release() noexcept;
    HBITMAP buffer() noexcept;

  private:
    const window& m_wnd;
    HWND m_handle{};
    HDC m_windowDC{};
    HDC m_backBufferDC{};
    HBITMAP m_backBuffer{};
    HBITMAP m_oldBuffer{};
  };

}