#include "ParticleCollection.hpp"

ParticleCollection::ParticleCollection()
{
}

ParticleCollection::~ParticleCollection()
{
}

bool ParticleCollection::Initialize(ID3D12Device* device, ParticleCollectionData& data, D3D12_CPU_DESCRIPTOR_HANDLE srvHandle, D3D12_CPU_DESCRIPTOR_HANDLE uavHandle)
{
	m_particleCount = data.nrParticle;
	m_initialData.resize(m_particleCount);

	for (int i = 0; i < m_particleCount; i++)
	{
		m_initialData[i].InitialPosition = data.InitialPosition;
		m_initialData[i].Position = data.InitialPosition;
		m_initialData[i].Velocity = data.Velocity;
		m_initialData[i].lifetime = data.lifetime;
	}
	bool initSuccess = m_structureBuffer.Initialize(device, m_particleCount, sizeof(ParticleData), true, true, srvHandle, uavHandle);

	return initSuccess;
}
