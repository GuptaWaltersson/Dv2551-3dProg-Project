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

	D3D12_CPU_DESCRIPTOR_HANDLE allocateSrvUavDescriptor();
private:
	HWND m_window = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Device> m_device;
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_factory;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_directQueue;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_srvUavHeap; // Descriptor Heap for shader resources,
									//we will need a different one for dsv one for rtv and one for sampler if needed

	UINT m_SrvUavdescriptorSize = 0;

	UINT m_currentHeapOffset = 0;
};