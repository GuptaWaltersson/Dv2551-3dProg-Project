#pragma once
#include <d3d12.h>
#include <wrl.h>

class constantBuffer{
public:
	constantBuffer() = default;
	~constantBuffer() = default;

	bool Initialize(ID3D12Device* device, UINT size);
	void copyData(void* data, UINT size);

	D3D12_GPU_VIRTUAL_ADDRESS getVirtualAddress();
private:

	Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
	void* m_mappedData = nullptr;
};