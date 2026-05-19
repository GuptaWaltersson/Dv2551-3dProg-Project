#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#include "windowhelper.hpp"
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include "Shader.hpp"
#include "ParticleCollection.hpp"
#include "vertexBuffer.hpp"
#include "camera.hpp"
#include "constantBuffer.hpp"
#include "ParticleRenderer.hpp"
struct CameraConstantBuffer
{
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
};

class Renderer
{
public:
	Renderer();
	~Renderer();
	
	bool Setup(HINSTANCE hinstance, int nCmdShow, size_t window_width, size_t window_height);

	void renderFrame(float frameTime);
	void Movement(float frameTime);
	void waitForGpuShutdown();

	bool createFactory();
	bool createDevice();
	bool createCommandQueue();
	bool createSwapchain();
	bool createCommandAllocator();
	bool createCommandList();
	bool createFence();
	bool createRootSignature();
	bool createInputLayout();
	bool createPipelineState();

	D3D12_CPU_DESCRIPTOR_HANDLE allocateSrvUavDescriptor();
	D3D12_CPU_DESCRIPTOR_HANDLE allocateRtvDescriptor();
	D3D12_RASTERIZER_DESC createRasterizerDesc();
	D3D12_BLEND_DESC createBlendStateDesc();
	D3D12_VIEWPORT createViewport();
	D3D12_RECT createScissor();
private:
	HWND m_window = nullptr;

	UINT m_width = 0;
	UINT m_height = 0;

	UINT m_frameIndex = 0;

	Microsoft::WRL::ComPtr<ID3D12Device> m_device;
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_factory;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapChain;
	static constexpr UINT FrameCount = 2;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_backBuffers[FrameCount];

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_directQueue; // Rendering 
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_computeQueue; // Compute / Particle simulation
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_copyQueue; // Uploads / resources
	
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_srvUavHeap; // Descriptor Heap for shader resources,
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap; // Descriptor Heap for rendertargetviews
	D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandles[FrameCount];

	UINT m_SrvUavdescriptorSize = 0;
	UINT m_SrvUavHeapOffset = 0;

	UINT m_rtvdescriptorSize = 0;
	UINT m_rtvHeapOffset = 0;

	Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
	UINT m_fenceValue = 0;
	UINT m_frameFenceValues[FrameCount] = {};
	HANDLE m_fenceEvent;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;

	std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputElement;
	D3D12_INPUT_LAYOUT_DESC m_inputLayout;
	Shader m_vertexShader;
	Shader m_pixelShader;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;

	vertexBuffer m_vertexBuffer;

	Camera m_camera;
	CameraConstantBuffer m_cameraData;
	constantBuffer m_cameraConstantBuffer[FrameCount];
};