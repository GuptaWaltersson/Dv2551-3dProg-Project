#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include "structuredBuffer.hpp"
struct ParticleCollectionData
{
	DirectX::XMFLOAT4 InitialPosition;
	DirectX::XMFLOAT3 Velocity;
	UINT nrParticle = 0;
	float lifetime = -1.0f;
};

class ParticleCollection {
public:
	ParticleCollection();
	~ParticleCollection();

	bool Initialize(ID3D12Device* device, ParticleCollectionData& data, D3D12_CPU_DESCRIPTOR_HANDLE srvHandle, D3D12_CPU_DESCRIPTOR_HANDLE uavHandle);
private:
	UINT m_particleCount = 0;
	struct ParticleData
	{
		DirectX::XMFLOAT4 InitialPosition;
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT3 Velocity;
		float lifetime;
	};
	std::vector<ParticleData> m_initialData;
	StructuredBuffer m_structureBuffer;
};