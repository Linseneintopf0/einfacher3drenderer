#include "Graphics.h"
#include "Window.h"
#include "Scene.h"
#include "Camera.h"

void Scene::DoFrame(Window &wnd)
{
	//Kamera Bewegen
	wnd.stf = Camera::UpDatePosition(wnd.stf, wnd.currentpressedkey);
	wnd.stf.rx = Camera::RotationLoop(wnd.stf.rx);
	wnd.stf.ry = Camera::RotationLoop(wnd.stf.ry);
	wnd.stf.rz = Camera::RotationLoop(wnd.stf.rz);

	//Malfläche leeren
	wnd.pGraphics().ClearBuffer(wnd.sGfx->colorobj);
	//Kamera Buffer aktualisieren
	wnd.pGraphics().UpdateConstantBuffer(wnd.width, wnd.height, wnd.stf);
	//Das Bild Malen
	wnd.pGraphics().Draw();
	//Das Bild absenden
	wnd.pGraphics().EndFrame();
}

void Scene::SetupFrame(Window &wnd) 
{
	//Model Data Variables
	Graphics::Vertex* pVertices = nullptr;
	Graphics::Index* pIndices = nullptr;
	unsigned short vertexcount = 0;
	unsigned short trianglecount = 0;

	//If Liste für Modelldaten
	if (wnd.sGfx->dateipfad == "")
	{
		
	}
	else if (wnd.sGfx->dateipfad == "RAINBOW")
	{
		
	}
	else if (wnd.sGfx->dateipfad == "CUBE")
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

	//Setup Befehle
	wnd.pGraphics().SetupVertexAndIndexBuffer(pIndices, pVertices, trianglecount, vertexcount);
	wnd.pGraphics().SetupConstantBufferAndShaders();
	wnd.pGraphics().SetupOutputmerger(wnd.width, wnd.height);
}