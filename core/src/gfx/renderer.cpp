#include "gfx/renderer.hpp"
#include "gfx/gfx.hpp"
#include "gfx/window.hpp"
#include "core/win_includes.hpp"
#include "assets/sprite.hpp"

#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1")

namespace engine::graphics
{
  // Addditional definitions

  namespace detail
  {
    static inline auto wnd_size(const window& wnd) noexcept
    {
      auto [w, h] = wnd.size();
      return D2D1::SizeU(static_cast<UINT32>(w), static_cast<UINT32>(h));
    }

    template <typename I>
    void release(I** resource) noexcept
    {
      if (*resource)
      {
        (*resource)->Release();
        (*resource) = nullptr;
      }
    }

    class bitmap
    {
    public:
      using value_type = ID2D1Bitmap*;

    public:
      CLASS_SPECIALS_NONE(bitmap);

      ~bitmap() noexcept
      {
        release(&m_bitmap);
      }

      bitmap(ID2D1RenderTarget* target, const sprite& s) noexcept
      {
        auto src = s.data();
        
        const auto sz     = D2D1::SizeU(s.width(), s.height());
        const auto format = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
        const auto props  = D2D1::BitmapProperties(format);
        const auto pitch  = s.width() * 4;
        const auto res    = target->CreateBitmap(sz, src, pitch, &props, &m_bitmap);
        utils::unused(res);
      }

      explicit operator bool() const noexcept
      {
        return static_cast<bool>(m_bitmap);
      }

      value_type operator->() noexcept
      {
        return m_bitmap;
      }

    private:
      value_type m_bitmap{};
    };
  }

  // Statics

  renderer::storage_type& renderer::storage() noexcept
  {
    static storage_type storage;
    return storage;
  }

  renderer& renderer::get(const gfx* g, const window& wnd) noexcept
  {
    auto&& store = storage();
    if (auto item = store.find(g); item != store.end())
    {
      return item->second;
    }

    return store.emplace(g, wnd).first->second;
  }
  void renderer::release(const gfx* g) noexcept
  {
    auto&& store = storage();
    store.erase(g);
  }

  // Special members

  renderer::~renderer() noexcept
  {
    release();
    CoUninitialize();
  }

  renderer::renderer(const window& wnd) noexcept :
    m_wnd{ wnd }
  {
    if (FAILED(CoInitialize(nullptr)))
    {
      // todo: error
      return;
    }

    init();
  }

  renderer::operator bool() const noexcept
  {
    return static_cast<bool>(m_target);
  }

  // Public members

  void renderer::resize() noexcept
  {
    auto size = detail::wnd_size(m_wnd);
    m_target->Resize(size);
  }

  void renderer::init_drawing() noexcept
  {
    m_target->BeginDraw();
    m_target->Clear(D2D1::ColorF{ D2D1::ColorF::AntiqueWhite });
  }
  void renderer::end_drawing() noexcept
  {
    auto res = m_target->EndDraw();
    utils::unused(res);
  }

  // Draw stuff

  // stupid test code
  void renderer::image(const sprite& s, const point_type& pos, const point_type& dir) noexcept
  {
    if (!s)
    {
      return;
    }

    auto&& bm = to_bitmap(s);
    if (!bm)
    {
      return;
    }

    utils::unused(pos, dir);
    //ID2D1SolidColorBrush* brush{};
    //m_target->CreateSolidColorBrush(D2D1::ColorF{ D2D1::ColorF::DarkBlue }, &brush);
    //auto p1 = viewport_to_screen(v1);
    //auto p2 = viewport_to_screen(v2);
    //m_target->DrawLine(D2D1::Point2F(p1[0], p1[1]),
    //                   D2D1::Point2F(p2[0], p2[1]),
    //                   brush,
    //                   5.0f);
    //
    //detail::release(&brush);
  }

  // Private members

  void renderer::init() noexcept
  {
    if (!m_wnd)
      return;

    auto res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
    if (FAILED(res))
    {
      // todo: error
      release();
      return;
    }

    auto wndHandle = m_wnd.handle<HWND>();
    auto size = detail::wnd_size(m_wnd);
    res = m_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                                            D2D1::HwndRenderTargetProperties(wndHandle, size),
                                            &m_target);

    if (FAILED(res))
    {
      // todo: error
      release();
      return;
    }

    m_target->SetTransform(D2D1::Matrix3x2F::Identity());
  }
  void renderer::release() noexcept
  {
    m_bitmapCache.clear();
    detail::release(&m_factory);
    detail::release(&m_target);
  }

  renderer::pixel_type renderer::viewport_to_screen(const point_type& v) const noexcept
  {
    using coord_t = pixel_type::value_type;
    const auto [w, h] = m_wnd.size();
    const auto x = static_cast<coord_t>(v[0]) + w / 2;
    const auto y = h / 2 - static_cast<coord_t>(v[1]);
    return { x, y };
  }

  renderer::bitmap& renderer::to_bitmap(const sprite& s) noexcept
  {
    const auto key = fsys::hash_value(s.filename());
    if (auto item = m_bitmapCache.find(key); item != m_bitmapCache.end())
    {
      return item->second;
    }

    auto newitem = m_bitmapCache.try_emplace(key, m_target, s);
    return newitem.first->second;
  }
}