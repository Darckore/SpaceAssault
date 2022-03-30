#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

namespace engine::graphics
{
  class window;
  class gfx;
  class sprite;

  namespace detail
  {
    class bitmap;
  }

  class renderer
  {
  private:
    using handle_type  = void*;
    using size_type    = std::size_t;
    using storage_type = std::unordered_map<const gfx*, renderer>;
    using point_type   = utils::vecd2;
    using pixel_type   = utils::vecf2;
    using bitmap       = detail::bitmap;
    using sprite_map   = std::unordered_map<size_type, bitmap>;

    template <typename T>
    using com_ptr = Microsoft::WRL::ComPtr<T>;

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
    void image(const sprite& s, const point_type& pos, const point_type& dir) noexcept;

  private:
    bool enable_dbg() noexcept;
    bool create_adapter() noexcept;
    bool create_device() noexcept;
    bool create_cmd_queue() noexcept;
    bool create_swapchain() noexcept;

    void init() noexcept;
    void release() noexcept;

    pixel_type viewport_to_screen(const point_type& v) const noexcept;

    bitmap& to_bitmap(const sprite& s) noexcept;

  private:
    static constexpr auto bufCnt = 3u;

    template <typename T>
    using arr_type = std::array<T, bufCnt>;

    template <typename T>
    using ptr_arr = arr_type<com_ptr<T>>;

  private:
    const window& m_wnd;

    com_ptr<IDXGIFactory4>               m_factory{};
    com_ptr<IDXGIAdapter4>               m_adapter{};
    com_ptr<ID3D12Device2>               m_device{};
    com_ptr<ID3D12CommandQueue>          m_cmdQueue{};
    com_ptr<IDXGISwapChain4>             m_swapChain{};
    com_ptr<ID3D12GraphicsCommandList>   m_cmdList{};
    com_ptr<ID3D12DescriptorHeap>        m_descriptorHeap{};

    ptr_arr<ID3D12Resource>         m_backBuffers{};
    ptr_arr<ID3D12CommandAllocator> m_cmdAllocators{};

    size_type m_RTVDescriptorSize;
    size_type m_currentBuf;

    com_ptr<ID3D12Fence> m_fence{};
    arr_type<size_type> m_fenceVals{};
    size_type   m_fenceCur{};
    handle_type g_FenceEvent;

    sprite_map m_bitmapCache;
  };

}