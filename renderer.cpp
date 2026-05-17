#include "renderer.hpp"
#include "windowhelper.hpp"
#include <Windows.h>
#include <iostream>
#include <d3d12.h>
#include <wrl.h>
#include "Shader.hpp"

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



	m_SrvUavdescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
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

	for (UINT i = 0; i < FrameCount; i++)
	{
		hr = m_swapChain->GetBuffer(i,IID_PPV_ARGS(&m_backBuffers[i]));

		if (FAILED(hr))
		{
			std::cout << "[RENDERER] Failed to get back buffer" << std::endl;
			return false;
		}

		m_rtvHandles[i] = allocateRtvDescriptor();

		m_device->CreateRenderTargetView(m_backBuffers[i].Get(), nullptr, m_rtvHandles[i]);
	}

	if (!createCommandAllocator())
	{
		std::cout << "[RENDERER] Failed to create command allocator" << std::endl;
		return false;
	}

	if (!createCommandList())
	{
		std::cout << "[RENDERER] Failed to create command list" << std::endl;
		return false;
	}

	if (!createFence())
	{
		std::cout << "[RENDERER] Failed to create fence" << std::endl;
		return false;
	}

	if (!createRootSignature())
	{
		std::cout << "[RENDERER] Failed to create root signature" << std::endl;
		return false;
	}

	if (!createInputLayout())
	{
		std::cout << "[RENDERER] Failed to create input layout" << std::endl;
		return false;
	}

	
#ifdef Guptadebug
	std::cout << "Renderer Setup success" << std::endl;
#endif
	return true;
}

void Renderer::renderFrame()
{
	m_commandAllocator->Reset();
	m_commandList->Reset(m_commandAllocator.Get(), nullptr);

	ID3D12Resource* currentBuffer = m_backBuffers[m_frameIndex].Get();
	//Transistion

	D3D12_RESOURCE_BARRIER barrier = {};

	barrier.Type =
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	barrier.Transition.pResource =
		currentBuffer;

	barrier.Transition.StateBefore =
		D3D12_RESOURCE_STATE_PRESENT;

	barrier.Transition.StateAfter =
		D3D12_RESOURCE_STATE_RENDER_TARGET;

	barrier.Transition.Subresource =
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	//Record
	m_commandList->ResourceBarrier(1, &barrier);
	m_commandList->OMSetRenderTargets(1, &m_rtvHandles[m_frameIndex], FALSE, nullptr);

	//Choose background color
	float clearColor[] =
	{
		0.1f,
		0.1f,
		0.6f,
		1.0f
	};
	m_commandList->ClearRenderTargetView(m_rtvHandles[m_frameIndex], clearColor, 0, nullptr);

	//Transition back
	barrier.Transition.StateBefore =
		D3D12_RESOURCE_STATE_RENDER_TARGET;

	barrier.Transition.StateAfter =
		D3D12_RESOURCE_STATE_PRESENT;

	//Record
	m_commandList->ResourceBarrier(
		1,
		&barrier);

	m_commandList->Close();

	// Execute all the commands recorded
	ID3D12CommandList* lists[] =
	{
		m_commandList.Get()
	};

	m_directQueue->ExecuteCommandLists(
		1,
		lists);

	//Present
	m_swapChain->Present(
		1,
		0);

	m_frameIndex =
		m_swapChain->GetCurrentBackBufferIndex();
	
	// WE WILL REMOVE THIS SINCE IT IS NOT OKAY FOR ASSIGNMENT JUST BEGINNER VERSION
	m_fenceValue++;

	m_directQueue->Signal(m_fence.Get(),m_fenceValue);

	if (m_fence->GetCompletedValue() < m_fenceValue)
	{
		m_fence->SetEventOnCompletion(
			m_fenceValue,
			m_fenceEvent);

		WaitForSingleObject(
			m_fenceEvent,
			INFINITE);
	}
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
	m_frameIndex =
		m_swapChain->GetCurrentBackBufferIndex();
	return SUCCEEDED(hr);
}

bool Renderer::createCommandAllocator()
{
	HRESULT hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	return SUCCEEDED(hr);
}

bool Renderer::createCommandList()
{
	HRESULT hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList));
	if (FAILED(hr))
	{
		return false;
	}
	m_commandList->Close();
	return SUCCEEDED(hr);
}

bool Renderer::createFence()
{
	HRESULT hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));

	if (FAILED(hr))
	{
		return false;
	}
	m_fenceEvent = CreateEvent(
		nullptr,
		FALSE,
		FALSE,
		nullptr);

	return true;
}

bool Renderer::createRootSignature()
{
	D3D12_ROOT_SIGNATURE_DESC rootDesc = {};

	rootDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	Microsoft::WRL::ComPtr<ID3DBlob> error;
	

	HRESULT hr = D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	return SUCCEEDED(hr);
}

bool Renderer::createInputLayout()
{
	m_inputElement =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},

		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			12,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};

	m_inputLayout.pInputElementDescs = m_inputElement.data();
	m_inputLayout.NumElements = m_inputElement.size();


	return true;
}

D3D12_CPU_DESCRIPTOR_HANDLE Renderer::allocateSrvUavDescriptor()
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle =
		m_srvUavHeap->GetCPUDescriptorHandleForHeapStart();

	handle.ptr += m_SrvUavHeapOffset * m_SrvUavdescriptorSize;
	m_SrvUavHeapOffset++;
	return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE Renderer::allocateRtvDescriptor()
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += m_rtvHeapOffset * m_rtvdescriptorSize;
	m_rtvHeapOffset++;
	return handle;
}
