#pragma once

namespace engine::graphics
{
  class window;
  class gfx;

  class buffer
  {
  public:
    using handle_type = void*;

  public:
    friend class renderer;

    CLASS_SPECIALS_ALL_CUSTOM(buffer);

  private:
    buffer() = default;

  private:
    void init_buffers(const window& wnd) noexcept;
    void kill_buffers(const window& wnd) noexcept;

    void clear(const window& wnd) noexcept;
    void swap(const window& wnd) noexcept;

  public:
    template <typename To>
    auto active_buf() const noexcept
    {
      return reinterpret_cast<To>(m_cur);
    }
    template <typename To>
    auto prev_buf() const noexcept
    {
      return reinterpret_cast<To>(m_prev);
    }
    template <typename To>
    auto device() const noexcept
    {
      return reinterpret_cast<To>(m_device);
    }
    template <typename To>
    auto wnd_device() const noexcept
    {
      return reinterpret_cast<To>(m_wndDevice);
    }

  private:
    handle_type m_prev{};
    handle_type m_cur{};

    handle_type m_device{};
    handle_type m_wndDevice{};
  };

  class renderer
  {
  private:
    using storage_type = std::unordered_map<const gfx*, renderer>;

  public:
    static renderer& get(const gfx* g, const window& wnd) noexcept;
    static void release(const gfx* g) noexcept;

  private:
    static storage_type& storage() noexcept;

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

  private:
    const window& m_wnd;
    buffer m_buf;
  };

}