#include "vertexBuffer.hpp"
#include <iostream>
vertexBuffer::vertexBuffer()
{
}

vertexBuffer::~vertexBuffer()
{
}

bool vertexBuffer::Initialize(ID3D12Device* device, void* vertexData, UINT vertexSize, UINT vertexCount)
{
	m_vertexSize = vertexSize;
	m_vertexCount = vertexCount;
	UINT bufferSize = m_vertexSize * m_vertexCount;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;


	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = bufferSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.MipLevels = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	HRESULT hr = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_resource));
	
	if (FAILED(hr))
	{
		std::cout << "[VERTEX BUFFER] Failed to create vertex buffer resource" << std::endl;
		return false;
	}
	
	void* mappedMemory;
	D3D12_RANGE range = {};

	hr = m_resource->Map(0, &range, &mappedMemory);
	if (FAILED(hr))
	{
		std::cout << "[VERTEX BUFFER] Failed to map resource" << std::endl;
		return false;
	}
	memcpy(mappedMemory, vertexData, bufferSize);
	m_resource->Unmap(0, nullptr);

	m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
	m_view.StrideInBytes = vertexSize;
	m_view.SizeInBytes = bufferSize;

	return true;
}

const D3D12_VERTEX_BUFFER_VIEW& vertexBuffer::GetView() const
{
	return m_view;
}
