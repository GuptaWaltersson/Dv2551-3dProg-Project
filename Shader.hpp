#pragma once

#include "renderer.hpp"
class Shader {
public:
	bool Load(const wchar_t* file, const char* entry, const char* target);
	D3D12_SHADER_BYTECODE GetShaderByteCode();

private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_shaderblob;
};