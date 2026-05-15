#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#include "windowhelper.hpp"
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include "structuredBuffer.hpp"
class Renderer
{
public:
	Renderer();
	~Renderer();
	
	bool Setup(HINSTANCE hinstance, int nCmdShow, size_t window_width, size_t window_height);
	bool createFactory();
	bool createDevice();
	bool createCommandQueue();
	bool createSwapchain();
	D3D12_CPU_DESCRIPTOR_HANDLE allocateSrvUavDescriptor();
	D3D12_CPU_DESCRIPTOR_HANDLE allocateRtvDescriptor();
private:
	HWND m_window = nullptr;

	UINT m_width = 0;
	UINT m_height = 0;

	Microsoft::WRL::ComPtr<ID3D12Device> m_device;
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_factory;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapChain;
	static constexpr UINT FrameCount = 2;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_backBuffers[FrameCount];

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_directQueue; // Rendering 
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_computeQueue; // Compute / Particle simulation
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_copyQueue; // Uploads / resources
	

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_srvUavHeap; // Descriptor Heap for shader resources,
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap; // Descriptor Heap for rendertargetviews
									

	UINT m_SrvUavdescriptorSize = 0;
	UINT m_SrvUavHeapOffset = 0;

	UINT m_rtvdescriptorSize = 0;
	UINT m_rtvHeapOffset = 0;
};