#include "renderer.hpp"
#include "windowhelper.hpp"
#include <Windows.h>
#include <iostream>
#include <d3d12.h>


#define Guptadebug
Renderer::Renderer()
{
	
}

Renderer::~Renderer()
{
}

bool Renderer::Setup(HINSTANCE instance, int nCmdShow, size_t window_width, size_t window_height) 
{
	if (!SetupWindow(instance, window_width, window_height, nCmdShow,this->m_window))
	{
		std::cout << "[RENDERER] Failed to setup window" << std::endl;
		return false;
	}

	if (!createFactory())
	{
		std::cout << "[RENDERER] Failed to create factory" << std::endl;
		//return false;
	}

	if (!createDevice())
	{
		std::cout << "[RENDERER] Failed to create device" << std::endl;
		return false;
	}

	m_SrvUavdescriptorSize = m_device->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 100;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HRESULT hr = m_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_srvUavHeap));
	if (FAILED(hr)) {
		std::cout << "[RENDERER] Failed to create descriptor heap" << std::endl;
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

D3D12_CPU_DESCRIPTOR_HANDLE Renderer::allocateSrvUavDescriptor()
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle =
		m_srvUavHeap->GetCPUDescriptorHandleForHeapStart();

	handle.ptr += m_currentHeapOffset * m_SrvUavdescriptorSize;



	return D3D12_CPU_DESCRIPTOR_HANDLE();
}
