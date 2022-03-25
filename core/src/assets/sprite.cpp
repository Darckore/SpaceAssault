#include "assets/sprite.hpp"
#include "lodepng/lodepng.h"

namespace engine::graphics
{
  // Special members

  sprite::sprite(name_type fname, byte_order byteOrder /* = png*/) noexcept :
    m_byteOrder{ byteOrder }
  {
    const auto error = load(fname);
    if (error)
    {
      utils::unused(error);
      // todo: report error: lodepng_error_text(error)
    }
  }

  sprite::operator bool() const noexcept
  {
    return !m_data.empty();
  }

  // Public members

  sprite::size_type sprite::width() const noexcept
  {
    return m_width;
  }
  sprite::size_type sprite::height() const noexcept
  {
    return m_height;
  }
  sprite::byte_order sprite::mode() const noexcept
  {
    return m_byteOrder;
  }

  // Private members

  unsigned sprite::load(name_type fname) noexcept
  {
    auto w = 0u;
    auto h = 0u;
    const auto error = lodepng::decode(m_data, w, h, fname.data());

    if (error)
    {
      return error;
    }

    m_width  = w;
    m_height = h;
    if (m_byteOrder == bmp)
    {
      to_bmp_bytes();
    }

    return error;
  }
  void sprite::to_bmp_bytes() noexcept
  {
    
  }
}