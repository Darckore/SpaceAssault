#pragma once
#include "gfx/window.hpp"
#include "core/win_includes.hpp"
#include "gfx/directx/d3d12.h"
#include "gfx/directx/d3dx12.h"
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <directxmath.h>

namespace engine::graphics::detail
{
  using size_type = std::uint64_t;
  
  template <typename T>
  using com_ptr = Microsoft::WRL::ComPtr<T>;
  
  inline constexpr auto bufCnt = 3u;
  
  template <typename T>
  using arr_type = std::array<T, bufCnt>;
  
  template <typename T>
  using ptr_arr = arr_type<com_ptr<T>>;

  ////////////////////
  // Direct3D target
  ////////////////////

  class target
  {
  public:
    CLASS_SPECIALS_NONE(target);

    ~target() noexcept
    {
      CloseHandle(m_fenceEvent);
    }

    target(const window& wnd) noexcept :
      m_wnd{ wnd }
    {
      if (!init_pipeline())
      {
        m_device = nullptr;
      }
    }

    explicit operator bool() const noexcept
    {
      return static_cast<bool>(m_device);
    }

    ID3D12Device2* operator->() noexcept
    {
      return m_device.Get();
    }

    bool begin_frame() noexcept
    {
      if (!*this)
      {
        return false;
      }

      auto alloc = m_cmdAllocators[m_currentBuf].Get();
      auto buf = m_backBuffers[m_currentBuf].Get();
      if (FAILED(alloc->Reset()))
      {
        // todo: error
        return false;
      }
      if (FAILED(m_cmdList->Reset(alloc, nullptr)))
      {
        // todo: error
        return false;
      }

      auto barrier = barrier_before(buf);
      m_cmdList->ResourceBarrier(1, &barrier);

      FLOAT backClr[] = { 0.4f, 0.6f, 0.9f, 1.0f };
      CD3DX12_CPU_DESCRIPTOR_HANDLE rtv{
          m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(),
          static_cast<INT>(m_currentBuf),
          static_cast<UINT>(m_RTVDescriptorSize)
      };

      m_cmdList->ClearRenderTargetView(rtv, backClr, 0, nullptr);
      return true;
    }
    bool end_frame() noexcept
    {
      if (!*this)
      {
        return false;
      }

      auto buf = m_backBuffers[m_currentBuf].Get();
      auto barrier = barrier_after(buf);
      m_cmdList->ResourceBarrier(1, &barrier);

      if (FAILED(m_cmdList->Close()))
      {
        // todo: error
        return false;
      }

      ID3D12CommandList* cmdList = m_cmdList.Get();
      m_cmdQueue->ExecuteCommandLists(1, &cmdList);

      if (FAILED(m_cmdQueue->Signal(m_fences[m_currentBuf].Get(), m_fenceVals[m_currentBuf])))
      {
        // todo: error
        return false;
      }

      if (FAILED(m_swapChain->Present(1, 0)))
      {
        // todo: error
        return false;
      }

      if (!wait())
      {
        // todo: error
        return false;
      }
      m_currentBuf = m_swapChain->GetCurrentBackBufferIndex();
      return true;
    }

  private:
    bool init_pipeline() noexcept
    {
      return {
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
        && create_fences()
      };
    }

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
        const auto isHW = (descr.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0;
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

        D3D12_MESSAGE_SEVERITY severities[]{ D3D12_MESSAGE_SEVERITY_INFO };
        D3D12_MESSAGE_ID ids[]{
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
        };

        D3D12_INFO_QUEUE_FILTER filter{};
        filter.DenyList.NumSeverities = _countof(severities);
        filter.DenyList.pSeverityList = severities;
        filter.DenyList.NumIDs = _countof(ids);
        filter.DenyList.pIDList = ids;
        infoQueue->PushStorageFilter(&filter);
      }
    #endif

      m_RTVDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
      return true;
    }
    bool create_cmd_queue() noexcept
    {
      D3D12_COMMAND_QUEUE_DESC desc{};
      desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
      desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
      desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
      desc.NodeMask = 0;
      return SUCCEEDED(m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_cmdQueue)));
    }
    bool create_swapchain() noexcept
    {
      auto [w, h] = m_wnd.size();
      DXGI_SWAP_CHAIN_DESC1 desc;
      desc.Width = static_cast<unsigned>(w);
      desc.Height = static_cast<unsigned>(h);
      desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      desc.Stereo = FALSE;
      desc.SampleDesc = { 1, 0 };
      desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      desc.BufferCount = bufCnt;
      desc.Scaling = DXGI_SCALING_STRETCH;
      desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
      desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
      desc.Flags = 0;

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
      if (FAILED(res))
      {
        // todo: error
        return false;
      }

      res = sc.As(&m_swapChain);
      if (FAILED(res))
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
      desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

      if (FAILED(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_descriptorHeap))))
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
    bool create_fences() noexcept
    {
      for (auto idx = 0ull; idx < bufCnt; ++idx)
      {
        if (FAILED(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fences[idx]))))
        {
          // todo: error
          return false;
        }
      }

      m_fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
      if (!m_fenceEvent)
      {
        // todo: error
        return false;
      }

      return true;
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

    bool wait() noexcept
    {
      auto fence = m_fences[m_currentBuf].Get();
      auto&& value = m_fenceVals[m_currentBuf];
      if (fence->GetCompletedValue() < value)
      {
        if (FAILED(fence->SetEventOnCompletion(value, m_fenceEvent)))
        {
          // todo: error
          return false;
        }

        WaitForSingleObject(m_fenceEvent, INFINITE);
      }

      ++value;
      return true;
    }

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

    size_type m_RTVDescriptorSize{};
    size_type m_currentBuf{};

    ptr_arr<ID3D12Fence> m_fences{};
    arr_type<size_type> m_fenceVals{};
    HANDLE m_fenceEvent{};
  };


  ////////////////////
  // Direct3D pipeline
  ////////////////////

  class pipeline
  {
  public:
    CLASS_SPECIALS_NONE(pipeline);

    ~pipeline() noexcept = default;

    pipeline(const window& wnd) noexcept :
      m_wnd{ wnd },
      m_target{ m_wnd }
    {
      init_drawing();
    }

    explicit operator bool() const noexcept
    {
      return static_cast<bool>(m_target);
    }

  public:
    bool begin_frame() noexcept
    {
      return m_target.begin_frame();
    }
    bool end_frame() noexcept
    {
      return m_target.end_frame();
    }

  private:
    bool init_drawing() noexcept
    {
      return {
           create_rs()
        && load_shaders()
        && create_state()
      };
    }

    bool create_rs() noexcept
    {
      CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
      rootSignatureDesc.Init(0, nullptr, 0, nullptr,
                             D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

      com_ptr<ID3DBlob> signature{};
      auto res = D3D12SerializeRootSignature(&rootSignatureDesc,
                                             D3D_ROOT_SIGNATURE_VERSION_1,
                                             &signature,
                                             nullptr);
      if (FAILED(res))
      {
        // todo: error
        return false;
      }

      res = m_target->CreateRootSignature(0,
                                          signature->GetBufferPointer(),
                                          signature->GetBufferSize(),
                                          IID_PPV_ARGS(&m_rootSignature));
      if (FAILED(res))
      {
        // todo: error
        return false;
      }

      return true;
    }
    bool load_shaders() noexcept
    {
      // vertex shader
      auto res = D3DCompileFromFile(L"data/assets/shaders/vertex.hlsl",
                                    nullptr,
                                    nullptr,
                                    "main",
                                    "vs_5_0",
                                    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
                                    0,
                                    &m_vertexShader,
                                    nullptr);
      if (FAILED(res))
      {
        // todo: error
        return false;
      }

      // pixel shader
      com_ptr<ID3DBlob> pixelShader{};
      res = D3DCompileFromFile(L"data/assets/shaders/pixel.hlsl",
                              nullptr,
                              nullptr,
                              "main",
                              "ps_5_0",
                              D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
                              0,
                              &m_pixelShader,
                              nullptr);
      if (FAILED(res))
      {
        // todo: error
        return false;
      }

      return true;
    }
    bool create_state() noexcept
    {
      D3D12_INPUT_ELEMENT_DESC inputLayout[] {
        {
          "POSITION",
          0,
          DXGI_FORMAT_R32G32B32_FLOAT,
          0,
          0,
          D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
          0
        }
      };

      D3D12_INPUT_LAYOUT_DESC layoutDesc{};
      layoutDesc.NumElements = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
      layoutDesc.pInputElementDescs = inputLayout;

      D3D12_SHADER_BYTECODE vsCode{};
      vsCode.BytecodeLength =  m_vertexShader->GetBufferSize();
      vsCode.pShaderBytecode = m_vertexShader->GetBufferPointer();

      D3D12_SHADER_BYTECODE psCode{};
      psCode.BytecodeLength =  m_pixelShader->GetBufferSize();
      psCode.pShaderBytecode = m_pixelShader->GetBufferPointer();

      DXGI_SAMPLE_DESC sampleDesc{};
      sampleDesc.Count = 1;

      D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
      psoDesc.InputLayout    = layoutDesc;
      psoDesc.pRootSignature = m_rootSignature.Get();
      psoDesc.VS = vsCode;
      psoDesc.PS = psCode;
      psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
      psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
      psoDesc.SampleDesc = sampleDesc;
      psoDesc.SampleMask = 0xffffffff;
      psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC{ D3D12_DEFAULT };
      psoDesc.BlendState = CD3DX12_BLEND_DESC{ D3D12_DEFAULT };
      psoDesc.NumRenderTargets = 1;
      
      if (FAILED(m_target->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_stateObj))))
      {
        // todo: error
        return false;
      }

      return true;
    }

  private:
    const window& m_wnd;

    target m_target;

    com_ptr<ID3DBlob> m_vertexShader{};
    com_ptr<ID3DBlob> m_pixelShader{};

    com_ptr<ID3D12PipelineState> m_stateObj{};
    com_ptr<ID3D12RootSignature> m_rootSignature{};
    com_ptr<ID3D12Resource>      m_vertexBuf{};
    D3D12_VERTEX_BUFFER_VIEW     m_vertexView{};
    
    D3D12_VIEWPORT m_viewport{};
    D3D12_RECT     m_scissorRect{};
  };

}