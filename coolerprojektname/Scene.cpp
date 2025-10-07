#include "Graphics.h"
#include "Window.h"
#include "Scene.h"
#include "Camera.h"
#include "objfileimport.h"
#include <cmath>

void Scene::DoFrame(Window &window)
{
	//Kamera Position anhand der gedrückten Tasten aktualisieren
	window.pCameraGet().UpdatePosition(window.HeldKeys, 0.025f, 0.005f); //ValuePos und -Rot sind Werte, um die die Kamera pro Frame transformiert wird
	//Malfläche leeren
	window.pGraphicsGet().ClearBuffer(window.structureGraphics->ColorObj.get());
	//Kamera Buffer aktualisieren
	window.pGraphicsGet().UpdateConstantBuffer(window.Width, window.Height, window.pCameraGet());
	//Das Bild Malen
	window.pGraphicsGet().Draw();
	//Das Bild absenden
	window.pGraphicsGet().EndFrame();
}

Scene::Scene(Window& wnd)
	:
	scale(wnd.structureGraphics->scale)
{
	//If Liste für Modelldaten
	if (wnd.structureGraphics->FilePath == "")
	{
		Graphics::Vertex vertices = {1.0f, 1.0f, 1.0f, 0, 0, 0, 0};
		Graphics::Index indices = { 0, 0, 0 };
	}
	else if (wnd.structureGraphics->FilePath == "RAINBOW")
	{
		
	}
	else if (wnd.structureGraphics->FilePath == "CUBE")
	{
		Graphics::Vertex vertices[] =
		{//    x      y		z     r    g    b    a (wird eh überschrieben, allerdings braucht die PrimitiveTopology den Alpha Wert)
			{ 0.0f,  0.0f, 0.0f, 255,   0,   0,   0},	//0 rot
			{ 1.0f,  0.0f, 0.0f, 255, 255,   0,   0},	//1 gelb
			{ 0.0f,  1.0f, 0.0f, 255, 255,   0,   0},	//2 gelb
			{ 1.0f,  1.0f, 0.0f,   0, 255,   0,   0},	//3 grün
			{ 0.0f,  0.0f, 1.0f, 255, 255,   0,   0},	//4 gelb
			{ 1.0f,  0.0f, 1.0f,   0, 255,   0,   0},	//5 grün
			{ 0.0f,  1.0f, 1.0f,   0, 255,   0,   0},	//6 grün
			{ 1.0f,  1.0f, 1.0f,   0,   0, 255,   0},	//7 blau
		};

		Graphics::Index indices[] =
		{
			{0, 3, 1,},	{0, 2, 3,},		//down
			{0, 6, 2,},	{0, 4, 6,},		//back
			{0, 1, 5,}, {0, 5, 4,},		//left

			{7, 4, 5,},	{7, 6, 4,},		//up
			{7, 5, 1,},	{7, 1, 3,},		//front
			{7, 2, 6,},	{7, 3, 2,},		//right
		};

		pVertices = &vertices[0];
		pIndices = &indices[0];
		trianglecount = sizeof(indices) / sizeof(Graphics::Index);
		vertexcount = sizeof(vertices) / sizeof(Graphics::Vertex);
	}
	//.obj Modelle
	else 
	{
		//OBJ File Test
		if (objfileimport::isobjfile(wnd.structureGraphics->FilePath))
		{
			objfileimport objfile(wnd.structureGraphics->FilePath, *this); //erstellen eine obj Objektes wenn die Bedingungen gelten
			pVertices = vertices.get();
			pIndices = indices.get();
			vertexcount = objfile.vertexcount;
			trianglecount = objfile.trianglecount;
		}
	}

	//Setup Befehle
	wnd.pGraphicsGet().SetupVertexAndIndexBuffer(pIndices, pVertices, trianglecount, vertexcount);
	wnd.pGraphicsGet().SetupConstantBufferAndShaders();
	wnd.pGraphicsGet().SetupOutputmerger(wnd.Width, wnd.Height);
}