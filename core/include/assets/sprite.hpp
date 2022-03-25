#pragma once

namespace engine::graphics
{
  class sprite
  {
  public:
    using name_type = std::string_view;
    using data_type = std::vector<std::uint8_t>;
    using size_type = std::uint32_t;

    enum class byte_order : uint8_t
    {
      png,
      bmp
    };

    using enum byte_order;

  public:
    CLASS_SPECIALS_NODEFAULT(sprite);

    sprite(name_type fname, byte_order byteOrder = png) noexcept;

    explicit operator bool() const noexcept;

  public:
    size_type width()  const noexcept;
    size_type height() const noexcept;
    byte_order mode()  const noexcept;

  private:
    unsigned load(name_type fname) noexcept;
    void to_bmp_bytes() noexcept;

  private:
    data_type m_data;
    size_type m_width{};
    size_type m_height{};
    byte_order m_byteOrder{};
  };
}