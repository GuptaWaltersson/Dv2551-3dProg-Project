#pragma once
#include "Shader.hpp"
#include <d3d12.h>
#include <iostream>
#include <wrl.h>
#include "ParticleCollection.hpp"

class particleRenderer
{
public:
	particleRenderer();
	~particleRenderer();

	bool Initialize();

	bool createParticlePipelineState();
	bool createRootSignature();
	void draw(ID3D12GraphicsCommandList* commandList, ParticleCollection& particles);

private:
	Shader m_particleVertex;
	Shader m_particleGeometry;
	Shader m_particlePixel;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelinestate;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
	D3D12_INPUT_LAYOUT_DESC m_inputLayout;
};