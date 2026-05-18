#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
struct ParticleCollectionData
{
	DirectX::XMFLOAT4 InitialPosition;
	DirectX::XMFLOAT4 Velocity;
	UINT nrParticle =0;
	float lifetime = -1.0f;
};

class ParticleCollection {
public:
	ParticleCollection();
	~ParticleCollection();
private:

};