#include "constantBuffer.hpp"

bool constantBuffer::Initialize(ID3D12Device* device, UINT size)
{
    size = (size + 255) & ~255;

    D3D12_HEAP_PROPERTIES heap = {};
    heap.Type = D3D12_HEAP_TYPE_UPLOAD;
    
    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = size;
    desc.Height = 1;
    desc.SampleDesc.Count = 1;
    desc.MipLevels = 1;
    desc.DepthOrArraySize = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    HRESULT hr = device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_resource));

    if (FAILED(hr))
    {
        return false;
    }

    m_resource->Map(0, nullptr, &m_mappedData);
    return true;
}

void constantBuffer::copyData(void* data, UINT size)
{
    memcpy(m_mappedData, data, size);
}

D3D12_GPU_VIRTUAL_ADDRESS constantBuffer::getVirtualAddress()
{
    return m_resource->GetGPUVirtualAddress();
}
