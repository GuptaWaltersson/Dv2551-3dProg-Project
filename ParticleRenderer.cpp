#include "ParticleRenderer.hpp"

particleRenderer::particleRenderer()
{
}

particleRenderer::~particleRenderer()
{
}

bool particleRenderer::Initialize()
{
	if (!createInputLayout())
	{
		std::cout << "[PARTICLE RENDERER] Failed to create input layout" << std::endl;
		return false;
	}

	if (!m_particleGeometry.Load(L"GeometryParticleShader.hlsl", "main", "gs_5_0"))
	{
		std::cout << "[PARTICLE RENDERER] Failed to load geometry shader" << std::endl;
		return false;
	}

	if (!m_particleVertex.Load(L"VertexParticleShader.hlsl", "main", "vs_5_0"))
	{
		std::cout << "[PARTICLE RENDERER] Failed to load vertex shader" << std::endl;
		return false;
	}

	if (!m_particlePixel.Load(L"PixelParticleShader.hlsl", "main", "ps_5_0"))
	{
		std::cout << "[PARTICLE RENDERER] Failed to load pixel shader" << std::endl;
		return false;
	}

	return true;
}

bool particleRenderer::createParticlePipelineState(ID3D12Device* device)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc = {};

	psDesc.pRootSignature = m_rootSignature.Get();

	psDesc.InputLayout = m_inputLayout;
	psDesc.GS = m_particleGeometry.GetShaderByteCode();
	psDesc.VS = m_particleVertex.GetShaderByteCode();
	psDesc.PS = m_particlePixel.GetShaderByteCode();
	psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	psDesc.SampleMask = UINT_MAX;

	psDesc.NumRenderTargets = 1;
	psDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psDesc.SampleDesc.Count = 1;
	psDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;

	D3D12_RASTERIZER_DESC raster = {};
	raster.FillMode = D3D12_FILL_MODE_SOLID;
	raster.CullMode = D3D12_CULL_MODE_NONE;
	raster.DepthClipEnable = TRUE;

	psDesc.RasterizerState = raster;

	D3D12_BLEND_DESC blend = {};

	blend.AlphaToCoverageEnable = FALSE;
	blend.IndependentBlendEnable = FALSE;

	D3D12_RENDER_TARGET_BLEND_DESC rt = {};

	rt.BlendEnable = TRUE;
	rt.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	rt.BlendOp = D3D12_BLEND_OP_ADD;
	rt.SrcBlendAlpha = D3D12_BLEND_ONE;
	rt.DestBlendAlpha = D3D12_BLEND_ZERO;
	rt.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	rt.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blend.RenderTarget[0] = rt;
	psDesc.BlendState = blend;
	psDesc.DepthStencilState.DepthEnable = FALSE;
	psDesc.DepthStencilState.StencilEnable = FALSE;

	HRESULT hr = device->CreateGraphicsPipelineState(&psDesc, IID_PPV_ARGS(&m_pipelinestate));
	return SUCCEEDED(hr);

}

bool particleRenderer::createRootSignature(ID3D12Device* device)
{
	D3D12_ROOT_PARAMETER params[2] = {};

	//b0
	params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	params[0].Descriptor.RegisterSpace = 0;
	params[0].Descriptor.ShaderRegister = 0;

	params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_GEOMETRY;

	//b1
	params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	params[1].Descriptor.RegisterSpace = 0;
	params[1].Descriptor.ShaderRegister = 1;
	params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_GEOMETRY;

	D3D12_ROOT_SIGNATURE_DESC desc;
	desc.NumParameters = 2;
	desc.pParameters = params;

	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3DBlob> sig;
	Microsoft::WRL::ComPtr<ID3DBlob> err;

	HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig, &err);
	if (FAILED(hr))
	{
		std::cout << "[PARTICLE RENDERER] [ROOT SIGNATURE] failed to create root signature" << std::endl;
		return false;
	}
	
	hr = device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	return SUCCEEDED(hr);
}

bool particleRenderer::createInputLayout()
{
	m_inputElement =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};
	m_inputLayout.pInputElementDescs = m_inputElement.data();
	m_inputLayout.NumElements = m_inputElement.size();

	return true;
}

void particleRenderer::draw(ID3D12GraphicsCommandList* commandList, ParticleCollection& particles)
{
	commandList->SetPipelineState(m_pipelinestate.Get());
	commandList->SetGraphicsRootSignature(m_rootSignature.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	//commandList->IASetVertexBuffers(0,1,)

}
