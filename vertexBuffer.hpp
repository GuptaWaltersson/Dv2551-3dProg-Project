#pragma once
#include <d3d12.h>
#include <wrl.h>
class vertexBuffer
{
public:
	vertexBuffer();
	~vertexBuffer();
	bool Initialize(ID3D12Device* device, void* vertices, UINT stride, UINT count);
	const D3D12_VERTEX_BUFFER_VIEW& GetView() const;
private:

	Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;

	D3D12_VERTEX_BUFFER_VIEW m_view;
	UINT m_vertexSize = 0;
	UINT m_vertexCount = 0;
};