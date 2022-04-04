#pragma once

namespace engine::graphics
{
  class window;
  class sprite;

  namespace detail
  {
    class pipeline;
  }

  class renderer
  {
  private:
    using size_type    = std::size_t;
    using point_type   = utils::vecd2;
    using pixel_type   = utils::vecf2;
    using pipeline_ptr = std::unique_ptr<detail::pipeline>;

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
    void image(const sprite& s, const point_type& pos, const point_type& dir) noexcept;

  private:
    void init() noexcept;

    pixel_type viewport_to_screen(const point_type& v) const noexcept;

  private:
    const window& m_wnd;
    pipeline_ptr m_pipeline;
  };

}