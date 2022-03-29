#pragma once

namespace engine::graphics
{
  class sprite
  {
  public:
    using name_type = fsys::path;
    using data_type = std::vector<std::uint8_t>;
    using size_type = std::uint32_t;

    enum class byte_order : uint8_t
    {
      png,
      bmp
    };

    using enum byte_order;

    template <std::forward_iterator It>
    class quad_iter
    {
    public:
      using iterator_category = It::iterator_category;
      using size_type = std::size_t;
      using difference_type = std::ptrdiff_t;
      using value_type = utils::colour;

      static constexpr auto quadSize = size_type{ 4 };

    public:
      CLASS_SPECIALS_NODEFAULT(quad_iter);

      quad_iter(It iter, byte_order byteOrder) noexcept :
        m_iter{ iter },
        m_mode{ byteOrder }
      {
      }

      bool operator==(const quad_iter& other) const noexcept
      {
        return m_mode == other.m_mode && m_iter == other.m_iter;
      }
      bool operator!=(const quad_iter& other) const noexcept
      {
        return !(*this == other);
      }

      quad_iter& operator++() noexcept
      {
        std::advance(m_iter, quadSize);
        return *this;
      }
      quad_iter operator++(int) noexcept
      {
        auto res = *this;
        ++(*this);
        return res;
      }

      value_type operator*() const noexcept
      {
        value_type res;
        auto it = m_iter;

        if (m_mode == byte_order::bmp)
        {
          res.set_blue(*(it++));
          res.set_green(*(it++));
          res.set_red(*(it++));
          res.set_alpha(*(it++));
        }
        else if (m_mode == byte_order::png)
        {
          res.set_red(*(it++));
          res.set_green(*(it++));
          res.set_blue(*(it++));
          res.set_alpha(*(it++));
        }

        return res;
      }

    private:
      It m_iter;
      byte_order m_mode;
    };

    template <std::forward_iterator It>
    class sprite_colours
    {
    public:
      using iterator  = quad_iter<It>;
      using size_type = iterator::size_type;
      using difference_type = iterator::difference_type;

    public:
      CLASS_SPECIALS_NODEFAULT(sprite_colours);

      sprite_colours(It beg, It end, byte_order byteOrder) noexcept :
        m_beg{ beg, byteOrder },
        m_end{ end, byteOrder }
      { }

      auto begin() const noexcept
      {
        return m_beg;
      }
      auto end() const noexcept
      {
        return m_end;
      }

    private:
      iterator m_beg;
      iterator m_end;
    };

    using colour_container = sprite_colours<data_type::const_iterator>;

  public:
    CLASS_SPECIALS_NODEFAULT(sprite);

    sprite(name_type fname, byte_order byteOrder = png) noexcept;

    explicit operator bool() const noexcept;

  public:
    size_type width()  const noexcept;
    size_type height() const noexcept;
    byte_order mode()  const noexcept;

    colour_container colours() const noexcept;

  private:
    unsigned load() noexcept;
    void to_bmp_bytes() noexcept;

  private:
    name_type m_name;
    data_type m_data;
    size_type m_width{};
    size_type m_height{};
    byte_order m_byteOrder{};
  };
}