#include "structuredBuffer.hpp"

StructuredBuffer::StructuredBuffer()
{

}

StructuredBuffer::~StructuredBuffer()
{
}

bool StructuredBuffer::Initialize(ID3D12Device* device, UINT elementCount, UINT elementSize, bool hasUAV, bool hasSRV, D3D12_CPU_DESCRIPTOR_HANDLE srvHandle, D3D12_CPU_DESCRIPTOR_HANDLE uavHandle)
{
	m_elementCount = elementCount;
	m_elementSize = elementSize;
	m_srvHandle = srvHandle;
	m_uavHandle = uavHandle;

	UINT64 bufferSize = m_elementCount * m_elementSize;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = bufferSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	if (hasUAV)
	{
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}
	else
	{
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	}

	HRESULT hr = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&m_resource));

	if (FAILED(hr))
	{
		return false;
	}

	if (hasUAV)
	{

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = m_elementCount;
		uavDesc.Buffer.StructureByteStride = m_elementSize;

		device->CreateUnorderedAccessView(
			m_resource.Get(),
			nullptr,
			&uavDesc,
			m_uavHandle);

	}

	if (hasSRV)
	{
		
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = m_elementCount;
		srvDesc.Buffer.StructureByteStride = m_elementSize;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		device->CreateShaderResourceView(
			m_resource.Get(),
			&srvDesc,
			m_srvHandle);
	}

	return true;
}

ID3D12Resource* StructuredBuffer::GetResource() const
{
	return this->m_resource.Get();
}
