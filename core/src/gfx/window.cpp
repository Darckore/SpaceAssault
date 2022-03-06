#include "gfx/window.h"
#include "core/win_includes.h"

namespace assault::graphics
{
  // Special members

  window::~window() noexcept
  {
    auto inst_handle = GetModuleHandle(0);
    DestroyWindow(static_cast<HWND>(m_handle));
    UnregisterClass(m_name.c_str(), inst_handle);
  }

  window::window(str_type title, str_type name, size_type width, size_type height) :
    m_title{ std::move(title) },
    m_name{ std::move(name) },
    m_width{ width },
    m_height{ height }
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
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    }

    return DefWindowProc(handle, msg_code, wp, lp);
  }

  window::handle_type window::handle() const noexcept
  {
    return m_handle;
  }

  // Private members

  namespace
  {
    LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
      window *wndPtr = nullptr;
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
    auto make_wnd_class(std::string_view className) noexcept
    {
      auto inst_handle = GetModuleHandle(0);

      WNDCLASSEX wc = {};
      wc.cbSize = sizeof(WNDCLASSEX);
      wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
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
    auto adjust_size(window::size_type w, window::size_type h) noexcept
    {
      auto screenWidth  = GetSystemMetrics(SM_CXSCREEN);
      auto screenHeight = GetSystemMetrics(SM_CYSCREEN);

      // Adjust client width and height
      RECT r = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };
      AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

      auto width  = r.right - r.left;
      auto height = r.bottom - r.top;
      auto posX = screenWidth / 2 - width / 2;
      auto posY = screenHeight / 2 - height / 2;

      return RECT{ posX, posY, width, height };
    }
  }

  void window::init()
  {
    auto inst_handle = make_wnd_class(m_name);
    if (!inst_handle)
    {
      throw wnd_error{ "Failed to register window class" };
    }

    auto [posX, posY, width, height] = adjust_size(m_width, m_height);
    auto handle = CreateWindowEx(
      WS_EX_APPWINDOW,
      m_name.data(),
      NULL,
      WS_OVERLAPPEDWINDOW,
      posX, posY, width, height,
      NULL, NULL,
      inst_handle,
      this
    );

    if (!handle)
    {
      throw wnd_error{ "Failed to create window" };
    }

    SetWindowText(handle, m_title.data());
    ShowWindow(handle, SW_SHOW);
    UpdateWindow(handle);
    m_handle = handle;
  }

}