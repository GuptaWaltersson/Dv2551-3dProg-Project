#include "ParticleRenderer.hpp"

particleRenderer::particleRenderer()
{
}

particleRenderer::~particleRenderer()
{
}

bool particleRenderer::Initialize()
{
	return false;
}

bool particleRenderer::createParticlePipelineState()
{
	return false;
}

bool particleRenderer::createRootSignature()
{
	return false;
}

void particleRenderer::draw(ID3D12GraphicsCommandList* commandList, ParticleCollection& particles)
{
}
