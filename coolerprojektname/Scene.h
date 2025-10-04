#pragma once
#include "window.h"

class Scene
{
public:
	static void DoFrame(Window &wnd);
	Scene(Window &wnd);
public:
	Graphics::Vertex* pVertices = nullptr;
	Graphics::Index* pIndices = nullptr;
	unsigned int vertexcount = 0;
	unsigned int trianglecount = 0;
	std::unique_ptr<Graphics::Vertex[]> vertices;
	std::unique_ptr<Graphics::Index[]> indices;
	int scale;
};
