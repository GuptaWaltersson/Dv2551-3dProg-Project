#pragma once
#include "Shader.hpp"
#include <d3d12.h>
#include <iostream>

class particleRenderer
{
public:
	particleRenderer();
	~particleRenderer();

	bool Initialize();

private:
	Shader m_particleVertex;
	Shader m_particleGeometry;
	Shader m_particlePixel;

};