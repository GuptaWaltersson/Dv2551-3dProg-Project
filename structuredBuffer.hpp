#pragma once
#include <d3d12.h>
#include <vector>
#include <wrl.h> // For the smart pointer

class StructuredBuffer
{
public:
	StructuredBuffer();
	~StructuredBuffer();

	bool Initialize(ID3D12Device* device, UINT elementCount, UINT elementSize, bool hasUAV, bool hasSRV);

	ID3D12Resource* GetResource() const;
	

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> m_resource; // Smart Pointer

	UINT m_elementCount = 0;
	UINT m_elementSize = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE m_srvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_uavHandle;
};