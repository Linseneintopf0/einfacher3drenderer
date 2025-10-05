#pragma once
#include <string>
#include <memory>
#include "Graphics.h"
#include "Scene.h"

class objfileimport
{
public:
	static bool isobjfile(std::string& filename);
	objfileimport(std::string &filename, Scene &scene);
	unsigned int vertexcount = 0;
	unsigned int trianglecount = 0;
private:
	static float findnumberfloataccurate(std::string& line, size_t& size); //konvertiert String Floats zu float Floats
	static unsigned int findnumberuint(std::string& line); //konvertiert String uints zu uints
};