#include "gfx/renderer.hpp"
#include "gfx/gfx.hpp"
#include "gfx/window.hpp"
#include "core/win_includes.hpp"
#include "assets/sprite.hpp"

#include "gfx/directx/d3d12.h"
#include "gfx/directx/d3dx12.h"
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "dxgi.lib")

namespace engine::graphics
{
  // Addditional definitions

  namespace detail
  {
    //////////////
    //  bitmap
    //////////////

    class bitmap
    {
    public:
      using value_type = void*;

    public:
      CLASS_SPECIALS_NONE(bitmap);

      ~bitmap() noexcept = default;

      bitmap(void* target, const sprite& s) noexcept
      {
        utils::unused(target, s);
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


    //////////////
    //  resources
    //////////////

    class resources
    {
    public:
      using size_type   = std::size_t;

      template <typename T>
      using com_ptr = Microsoft::WRL::ComPtr<T>;

      static constexpr auto bufCnt = 3u;

      template <typename T>
      using arr_type = std::array<T, bufCnt>;

      template <typename T>
      using ptr_arr = arr_type<com_ptr<T>>;

    public:
      CLASS_SPECIALS_NONE(resources);

      ~resources() = default;

      resources(const window& wnd) noexcept :
        m_wnd{ wnd }
      {
        const bool success
        {
             static_cast<bool>(m_wnd)
          && enable_dbg()
          && create_adapter()
          && create_device()
          && create_cmd_queue()
          && create_swapchain()
          && create_descriptor_heap()
          && update_rtvs()
          && init_allocators()
          && create_cmd_list()
        };

        if (!success)
        {
          m_device = nullptr;
        }
      }

      explicit operator bool() const noexcept
      {
        return static_cast<bool>(m_device);
      }

    public:
      void begin_frame() noexcept
      {
        auto alloc = m_cmdAllocators[m_currentBuf].Get();
        auto buf   = m_backBuffers[m_currentBuf].Get();
        alloc->Reset();
        m_cmdList->Reset(alloc, nullptr);

        auto barrier = barrier_before(buf); 
        m_cmdList->ResourceBarrier(1, &barrier);

        FLOAT backClr[] = { 0.4f, 0.6f, 0.9f, 1.0f };
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtv { 
            m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(),
            static_cast<INT>(m_currentBuf),
            static_cast<UINT>(m_RTVDescriptorSize)
        };

        m_cmdList->ClearRenderTargetView(rtv, backClr, 0, nullptr);
      }
      bool end_frame() noexcept
      {
        auto buf     = m_backBuffers[m_currentBuf].Get();
        auto barrier = barrier_after(buf);
        m_cmdList->ResourceBarrier(1, &barrier);

        if (FAILED(m_cmdList->Close()))
        {
          // todo: error
          return false;
        }

        ID3D12CommandList* cmdList = m_cmdList.Get();
        m_cmdQueue->ExecuteCommandLists(1, &cmdList);

        return true;
      }

    private:
      CD3DX12_RESOURCE_BARRIER barrier_before(ID3D12Resource* resource) noexcept
      {
        return CD3DX12_RESOURCE_BARRIER::Transition(resource,
                                          D3D12_RESOURCE_STATE_PRESENT,
                                          D3D12_RESOURCE_STATE_RENDER_TARGET);
      }
      CD3DX12_RESOURCE_BARRIER barrier_after(ID3D12Resource* resource) noexcept
      {
        return CD3DX12_RESOURCE_BARRIER::Transition(resource,
                                          D3D12_RESOURCE_STATE_RENDER_TARGET,
                                          D3D12_RESOURCE_STATE_PRESENT);
      }

      com_ptr<ID3D12CommandAllocator> create_allocator() noexcept
      {
          com_ptr<ID3D12CommandAllocator> alloc;
          m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&alloc));
          return alloc;
      }
      bool init_allocators() noexcept
      {
        for (auto i = 0u; i < bufCnt; ++i)
        {
          auto alloc = create_allocator();
          if (!alloc)
          {
            // todo: error
            return false;
          }
          m_cmdAllocators[i] = alloc;
        }
        return true;
      }

      bool enable_dbg() noexcept
      {
      #ifndef NDEBUG
        com_ptr<ID3D12Debug> dbg;
        const auto res = D3D12GetDebugInterface(IID_PPV_ARGS(&dbg));
        if (FAILED(res))
        {
          // todo: error
          return false;
        }
        dbg->EnableDebugLayer();
      #endif
        return true;
      }
      bool create_adapter() noexcept
      {
        constexpr std::uint32_t flags =
      #ifndef NDEBUG
          DXGI_CREATE_FACTORY_DEBUG;
      #else
          0;
      #endif

        auto res = CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_factory));
        if (FAILED(res))
        {
          // todo: error
          return false;
        }

        auto vmem = 0ull;
        com_ptr<IDXGIAdapter1> adapter{};
        auto checkAdapter = [&vmem, &adapter](const DXGI_ADAPTER_DESC1& descr)
        {
          const auto isHW   = (descr.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0;
          const auto devRes = D3D12CreateDevice(adapter.Get(),
                                                D3D_FEATURE_LEVEL_11_0,
                                                __uuidof(ID3D12Device),
                                                nullptr);
          const auto devOk = isHW && SUCCEEDED(devRes);
          return devOk && descr.DedicatedVideoMemory > vmem;
        };

        for (auto idx = 0u; m_factory->EnumAdapters1(idx, &adapter) != DXGI_ERROR_NOT_FOUND; ++idx)
        {
          DXGI_ADAPTER_DESC1 adapterDescr;
          adapter->GetDesc1(&adapterDescr);

          if (checkAdapter(adapterDescr))
          {
            vmem = adapterDescr.DedicatedVideoMemory;
            res = adapter.As(&m_adapter);
          }

          if (FAILED(res))
          {
            // todo: error
            return false;
          }
        }

        return true;
      }
      bool create_device() noexcept
      {
        auto res = D3D12CreateDevice(m_adapter.Get(),
                                     D3D_FEATURE_LEVEL_11_0,
                                     IID_PPV_ARGS(&m_device));
        if (FAILED(res))
        {
          // todo: error
          return false;
        }

      #ifndef NDEBUG
        com_ptr<ID3D12InfoQueue> infoQueue;
        if (SUCCEEDED(m_device.As(&infoQueue)))
        {
          infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
          infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
          infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

          D3D12_MESSAGE_SEVERITY severities[] { D3D12_MESSAGE_SEVERITY_INFO };
          D3D12_MESSAGE_ID ids[] {
              D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
              D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
              D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
          };

          D3D12_INFO_QUEUE_FILTER filter{};
          filter.DenyList.NumSeverities = _countof(severities);
          filter.DenyList.pSeverityList = severities;
          filter.DenyList.NumIDs        = _countof(ids);
          filter.DenyList.pIDList       = ids;
          infoQueue->PushStorageFilter(&filter);
        }
      #endif

        m_RTVDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        return true;
      }
      bool create_cmd_queue() noexcept
      {
        D3D12_COMMAND_QUEUE_DESC desc{};
        desc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;
        return SUCCEEDED(m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_cmdQueue)));
      }
      bool create_swapchain() noexcept
      {
        auto [w, h] = m_wnd.size();
        DXGI_SWAP_CHAIN_DESC1 desc;
        desc.Width       = static_cast<unsigned>(w);
        desc.Height      = static_cast<unsigned>(h);
        desc.Format      = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Stereo      = FALSE;
        desc.SampleDesc  = { 1, 0 };
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = bufCnt;
        desc.Scaling     = DXGI_SCALING_STRETCH;
        desc.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.AlphaMode   = DXGI_ALPHA_MODE_UNSPECIFIED;
        desc.Flags       = 0;

        auto handle = m_wnd.handle<HWND>();
        com_ptr<IDXGISwapChain1> sc;
        auto res = m_factory->CreateSwapChainForHwnd(m_cmdQueue.Get(), handle,
                                                     &desc, nullptr, nullptr, &sc);
        if (FAILED(res))
        {
          // todo: error
          return false;
        }
        
        res = m_factory->MakeWindowAssociation(handle, DXGI_MWA_NO_ALT_ENTER);
        if(FAILED(res))
        {
          // todo: error
          return false;
        }

        res = sc.As(&m_swapChain);
        if(FAILED(res))
        {
          // todo: error
          return false;
        }

        m_currentBuf = m_swapChain->GetCurrentBackBufferIndex();
        return true;
      }
      bool create_descriptor_heap() noexcept
      {
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.NumDescriptors = bufCnt;
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

        if(FAILED(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_descriptorHeap))))
        {
          // todo: error
          return false;
        }

        return true;
      }
      bool create_cmd_list() noexcept
      {
        auto alloc = m_cmdAllocators[m_currentBuf].Get();
        auto res = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                               alloc, nullptr, IID_PPV_ARGS(&m_cmdList));
        if (FAILED(res))
        {
          // todo: error
          return false;
        }

        res = m_cmdList->Close();
        return SUCCEEDED(res);
      }

      bool update_rtvs() noexcept
      {
        CD3DX12_CPU_DESCRIPTOR_HANDLE  rtvHandle{ m_descriptorHeap->GetCPUDescriptorHandleForHeapStart() };

        for (auto i = 0u; i < bufCnt; ++i)
        {
          com_ptr<ID3D12Resource> buf;
          if (SUCCEEDED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&buf))))
          {
            m_device->CreateRenderTargetView(buf.Get(), nullptr, rtvHandle);
            m_backBuffers[i] = buf;
            rtvHandle.Offset(static_cast<INT>(m_RTVDescriptorSize));
            continue;
          }

          // todo: error
          return false;
        }

        return true;
      }

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
    return static_cast<bool>(m_res);
  }

  // Public members

  void renderer::resize() noexcept
  {
    //auto size = detail::wnd_size(m_wnd);
    //m_target->Resize(size);
  }

  void renderer::init_drawing() noexcept
  {
    m_res->begin_frame();
  }
  void renderer::end_drawing() noexcept
  {
    if (!m_res->end_frame())
    {
      // todo: error
      m_res.reset(nullptr);
    }
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
  }

  // Private members

  void renderer::init() noexcept
  {
    m_res = std::make_unique<detail::resources>(m_wnd);
    if (!(*m_res))
    {
      // todo: error
      m_res.reset(nullptr);
    }
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

    auto newitem = m_bitmapCache.try_emplace(key, nullptr, s);
    return newitem.first->second;
  }
}