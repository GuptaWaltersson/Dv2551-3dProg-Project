#include "renderer.hpp"
#include "windowhelper.hpp"
#include <Windows.h>
#include <iostream>
#include <d3d12.h>
#include <wrl.h>


#define Guptadebug
Renderer::Renderer()
{
	
}

Renderer::~Renderer()
{
}

bool Renderer::Setup(HINSTANCE instance, int nCmdShow, size_t window_width, size_t window_height) 
{
	m_width = window_width;
	m_height = window_height;
	if (!SetupWindow(instance, window_width, window_height, nCmdShow,this->m_window))
	{
		std::cout << "[RENDERER] Failed to setup window" << std::endl;
		return false;
	}

	if (!createFactory())
	{
		std::cout << "[RENDERER] Failed to create factory" << std::endl;
		return false;
	}

	if (!createDevice())
	{
		std::cout << "[RENDERER] Failed to create device" << std::endl;
		return false;
	}

	if (!createCommandQueue())
	{
		std::cout << "[RENDERER] Failed to create command queue" << std::endl;
		return false;
	}

	if (!createSwapchain())
	{
		std::cout << "[RENDERER] Failed to create swapchain" << std::endl;
		return false;
	}

	for (UINT i = 0; i < FrameCount; i++)
	{
		m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i]));
	}



	m_SrvUavdescriptorSize = m_device->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_rtvdescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 100;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HRESULT hr = m_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_srvUavHeap));
	if (FAILED(hr)) {
		std::cout << "[RENDERER] Failed to create srv uav descriptor heap" << std::endl;
		return false;
	}

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = m_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_rtvHeap));
	if (FAILED(hr))
	{
		std::cout << "[RENDERER] Failed to create rtv descriptor heap" << std::endl;
		return false;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = allocateSrvUavDescriptor();
	D3D12_CPU_DESCRIPTOR_HANDLE uavHandle = allocateSrvUavDescriptor();
#ifdef Guptadebug
	std::cout << "Renderer Setup success" << std::endl;
#endif
	return true;
}

bool Renderer::createFactory()
{
	UINT dxgiFlags = 0;

#if defined(_DEBUG)
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;

		if (SUCCEEDED(D3D12GetDebugInterface(
			IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			dxgiFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif

	HRESULT hr = CreateDXGIFactory2(
		dxgiFlags,
		IID_PPV_ARGS(&m_factory));

	if (FAILED(hr))
	{
		return false;
	}

	return true;
	return false;
}

bool Renderer::createDevice()
{
	HRESULT hr = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_device));

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool Renderer::createCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	HRESULT hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_directQueue));

	if (FAILED(hr))
	{
		std::cout << "[COMMAND QUEUE] could not create direct queue" << std::endl;
		return false;
	}

	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_computeQueue));
	if (FAILED(hr))
	{
		std::cout << "[COMPUTE QUEUE] could not create compute queue" << std::endl;
		return false;
	}

	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
	hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_copyQueue));

	if (FAILED(hr))
	{
		std::cout << "[COPY QUEUE] Could not create copy queue" << std::endl;
		return false;
	}

	return true;
}

bool Renderer::createSwapchain()
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};

	desc.Width = m_width;
	desc.Height = m_height;

	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	desc.BufferCount = 2;

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;
	
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
	HRESULT hr = m_factory->CreateSwapChainForHwnd(m_directQueue.Get(), this->m_window, &desc, nullptr, nullptr, &swapChain1);

	if (FAILED(hr))
	{
		return false;
	}
	hr = swapChain1.As(&m_swapChain);
	return SUCCEEDED(hr);
}

D3D12_CPU_DESCRIPTOR_HANDLE Renderer::allocateSrvUavDescriptor()
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle =
		m_srvUavHeap->GetCPUDescriptorHandleForHeapStart();

	handle.ptr += m_SrvUavHeapOffset * m_SrvUavdescriptorSize;

	return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE Renderer::allocateRtvDescriptor()
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += m_rtvHeapOffset * m_rtvdescriptorSize;
	return handle;
}
