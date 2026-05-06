#include "renderer.hpp"
#include "windowhelper.hpp"
#include <Windows.h>
#include <iostream>
Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Setup(HINSTANCE instance, int nCmdShow, size_t window_width, size_t window_height)
{
	if (!SetupWindow(instance, window_width, window_height, nCmdShow,this->m_window))
	{
		std::cout << "Failed to setup window" << std::endl;
	}

	return false;
}
