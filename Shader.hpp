#pragma once

#include <wrl.h>
#include <iostream>
#include <d3dcompiler.h>
#include <d3d12.h>

#pragma comment(lib, "d3dcompiler.lib")
class Shader{
public:
	Shader() = default;
	~Shader() = default;
	bool Load(const wchar_t* file, const char* entry, const char* target);
	D3D12_SHADER_BYTECODE GetShaderByteCode();

private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_shaderblob;
};