#pragma once
#include "windowhelper.hpp"
class Renderer
{
public:
	Renderer();
	~Renderer();
	
	bool Setup(HINSTANCE hinstance, int nCmdShow, size_t window_width, size_t window_height);
private:
	HWND m_window = nullptr;

};