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
	static float findnumberfloat(std::string& line, int scale);
	static unsigned int findnumberuint(std::string& line);

};