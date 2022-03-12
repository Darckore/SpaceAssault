#include "gfx/window.hpp"
#include "core/win_includes.hpp"

namespace assault::graphics
{
  // Special members

  window::~window() noexcept
  {
    auto inst_handle = GetModuleHandle(0);
    DestroyWindow(handle<HWND>());
    UnregisterClass(m_name.c_str(), inst_handle);
  }

  window::window(str_type title, str_type name) :
    m_title{ std::move(title) },
    m_name{ std::move(name) }
  {
    init();
  }

  // Additional definitions

  struct window::msg_wrapper
  {
    HWND handle;
    UINT msg_code;
    WPARAM wp;
    LPARAM lp;

    
  };

  // Public members

  window::proc_result window::window_proc(msg_wrapper msg) noexcept
  {
    auto [handle, msg_code, wp, lp] = msg;
    switch (msg_code)
    {
    case WM_SIZE:
      m_size = { GET_X_LPARAM(lp), GET_Y_LPARAM(lp) };
      return 0;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    }

    return DefWindowProc(handle, msg_code, wp, lp);
  }

  window::dimensions window::size() const noexcept
  {
    return m_size;
  }

  // Private members

  struct wnd_helper
  {
    static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      window* wndPtr = nullptr;
      if (msg == WM_NCCREATE)
      {
        wndPtr = reinterpret_cast<window*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wndPtr));
        return TRUE;
      }

      wndPtr = reinterpret_cast<window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
      if (wndPtr)
      {
        return wndPtr->window_proc({ hwnd, msg, wParam, lParam });
      }

      return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    static auto make_wnd_class(std::string_view className) noexcept
    {
      auto inst_handle = GetModuleHandle(0);

      WNDCLASSEX wc = {};
      wc.cbSize = sizeof(WNDCLASSEX);
      wc.style = CS_HREDRAW | CS_VREDRAW;
      wc.lpfnWndProc = wnd_proc;
      wc.cbClsExtra = 0;
      wc.cbWndExtra = 0;
      wc.hInstance = inst_handle;
      wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
      wc.hIconSm = wc.hIcon;
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
      wc.lpszMenuName = NULL;
      wc.lpszClassName = className.data();

      if (!RegisterClassEx(&wc))
      {
        return decltype(inst_handle){};
      }

      return inst_handle;
    }
    static auto calc_size() noexcept
    {
      MONITORINFO monitorInfo{ sizeof(monitorInfo) };
      auto monitorHandle = MonitorFromPoint(POINT{}, MONITOR_DEFAULTTOPRIMARY);
      GetMonitorInfo(monitorHandle, &monitorInfo);

      const auto& monitorRect = monitorInfo.rcMonitor;
      const auto posX   = monitorRect.left;
      const auto posY   = monitorRect.top;
      const auto width  = monitorRect.right - posX;
      const auto height = monitorRect.bottom - posY;
      return RECT{ posX, posY, width, height };
    }
  };

  void window::init()
  {
    auto inst_handle = wnd_helper::make_wnd_class(m_name);
    if (!inst_handle)
    {
      throw wnd_error{ "Failed to register window class" };
    }

    auto [posX, posY, width, height] = wnd_helper::calc_size();
    m_size = { width, height };
    auto handle = CreateWindow(
      m_name.data(),
      m_title.data(),
      WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      posX, posY, width, height,
      NULL, NULL,
      inst_handle,
      this
    );
    
    if (!handle)
    {
      throw wnd_error{ "Failed to create window" };
    }

    SetWindowPos(handle, HWND_TOP, posX, posY, width, height,
                 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

    SetWindowText(handle, m_title.data());
    ShowWindow(handle, SW_SHOW);
    UpdateWindow(handle);
    m_handle = handle;
  }

}