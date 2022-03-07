#pragma once
#include "core/exception.h"

namespace assault::graphics
{
  class window;

  class gfx
  {
  public:
    class gfx_error : public err::exception
    {
    public:
      using exception::exception;
    };

  private:
    class resources;

  public:
    CLASS_SPECIALS_NONE(gfx);

    ~gfx() noexcept;

    gfx(window& wnd);

  public:
    void begin_frame() noexcept;
    void draw() noexcept;

  private:
    window& m_wnd;
  };
}