#pragma once

namespace engine::graphics
{
  class window;
  class gfx;

  class resource_manager
  {
  public:
    using handle_type = void*;

  public:
    friend class renderer;

    CLASS_SPECIALS_NONE(resource_manager);

    ~resource_manager() noexcept;

    explicit operator bool() const noexcept;

  private:
    resource_manager(const window& wnd) noexcept;

  private:
    void init() noexcept;
    void resize() noexcept;
    void kill() noexcept;

    void clear() noexcept;
    void swap() noexcept;

  private:
    const window& m_wnd;
    handle_type m_d2dFactory{};
    handle_type m_target{};
    handle_type m_brush{};
  };

  class renderer
  {
  private:
    using storage_type = std::unordered_map<const gfx*, renderer>;
    using point_type   = utils::vecd2;
    using pixel_type   = utils::vecf2;

  public:
    static renderer& get(const gfx* g, const window& wnd) noexcept;
    static void release(const gfx* g) noexcept;

  private:
    static storage_type& storage() noexcept;

  public:
    CLASS_SPECIALS_NONE(renderer);

    ~renderer() noexcept;

    renderer(const window& wnd) noexcept;

    explicit operator bool() const noexcept;

  public:
    void resize() noexcept;
    void init_drawing() noexcept;
    void end_drawing() noexcept;

    //stupid test code
    void line(const point_type& v1, const point_type& v2) noexcept;

  private:
    void init() noexcept;
    void release() noexcept;

    pixel_type viewport_to_screen(const point_type& v) const noexcept;

  private:
    const window& m_wnd;
    resource_manager m_resMgr;
  };

}