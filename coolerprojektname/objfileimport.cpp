#include "objfileimport.h"
#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include <cmath>
#include "Graphics.h"
#include "Scene.h"


objfileimport::objfileimport(std::string& filename, Scene &scene)
{
	std::ifstream objfile(filename);
	std::string currentline;
	while (getline(objfile, currentline))
	{
		if (currentline[0] == 'v')
		{
			vertexcount++;
		}
		if (currentline[0] == 'f')
		{
			trianglecount++;
		}
	}

	vertexcount++; //Es gibt keinen nullten Vertex -> platzschaffen
	scene.vertices = std::make_unique<Graphics::Vertex[]>(vertexcount);
	scene.vertices[0] = { 0, 0, 0, 0, 0, 0, 0 }; //Es gibt keinen null Index in der Datei -> leerer Punkt bei [0]
	scene.indices = std::make_unique<Graphics::Index[]>(trianglecount);

	currentline = ""; //von dem letzten Iterieren reseten
	size_t iVertex = 1;
	size_t iIndex = 0;

	objfile.clear(); //leert die Flags für "EndOfFile" und Error Flags
	objfile.seekg(0, std::ios::beg); //setzt das lesen auf den Fileanfang zurück.

	while(getline(objfile, currentline)) //über die Zeilen der Datei erneut iterieren
	{
		if (currentline.empty()) continue;
		if (currentline[0] == 'v' && currentline[1] == ' ') //wenn die Zeile einen Vertex enthält:
		{
			float x, y, z; //x, y, z Koordinatenvariablen
			currentline.erase(0, 2); //das "v " entfernen
			std::string copyofline = currentline; //eine Kopie von der Zeile machen
		
			x = findnumberfloat(copyofline, scene.scale); //x durch den Zahlalgrorithmus werfen (reduziert copyofline zu den relevanten Zahlen)
			(x < 0) ? currentline.erase(0, copyofline.length() + 3) : currentline.erase(0, copyofline.length() + 2);  //Zahl entfernen (das Leerzeichen, den Dezimalpunkt und das Minuszeichen (wenn Vorhanden))
			copyofline = currentline; //die Kopie auf die nächste Zahl zurück setzen
		
			y = findnumberfloat(copyofline, scene.scale); //der ganze Spaß für y
			(y < 0) ? currentline.erase(0, copyofline.length() + 3) : currentline.erase(0, copyofline.length() + 2);
			copyofline = currentline;
		
			z = findnumberfloat(copyofline, scene.scale); //der ganze Spaß für z
		
			scene.vertices[iVertex] = { x, y, z, 0, 0, 0, 0 }; //die Vertex Struktur vervollständigen und Appenden
			iVertex++; //Iterator Variable
		}
		if (currentline[0] == 'f' && currentline[1] == ' ')
		{
			unsigned int a, b, c; //a, b und c Index
			currentline.erase(0, 2); //das "f " entfernen

			if (currentline.empty()) { continue; }
			c = findnumberuint(currentline);
			if (currentline.empty()) { continue; }
			b = findnumberuint(currentline);
			if (currentline.empty()) { continue; }
			a = findnumberuint(currentline);

			scene.indices[iIndex] = { a, b, c }; //die Vertex Struktur vervollständigen und Appenden
			iIndex++; //Iterator Variable
		}
	}
}

bool objfileimport::isobjfile(std::string& filename)
{
	size_t length = filename.length()-1;
	std::ostringstream reversefileformat;
	for (size_t i = length; i > 0; i--) //den Namen von hinten bis zum ersten '.' durchgehen
	{
		reversefileformat << filename[i]; //alle gefundenen Buchstaben appenden
		if (filename[i] == '.') //wenn der erste Punkt von hinten kommt, aufhören
		{
			break;
		}
	}
	std::string comparestring =	reversefileformat.str();
	return (comparestring == "jbo.") ? true : false; //wenn reversefileformat "jbo." gilt, dann ist das Ende der Datei .obj
}

float objfileimport::findnumberfloat(std::string& line, int scale)
{
	size_t decimalpoint = 0;
	double returnfloat = 0;
	bool signbit = false;

	if (line[0] == '-')
	{
		signbit = true;
		line.erase(0, 1);
	}

	for (size_t i = 0; i < line.length(); i++) //durch die Kopie iterieren, bis zum ersten Leerzeichen
	{
		if (decimalpoint) //wenn decimalpoint != 0 gilt, dann werden die Nachkommastellen gebildet -> verhindert doppeltes iterieren über diese Zahlen
		{
			if ((unsigned char)line[i] != 0) //Berechnung von 0 ignorieren -> erzeugt sowieso 0 im output
			{
				returnfloat = returnfloat + (double)((unsigned char)line[i] - 48) * std::pow(10.0, ((double)decimalpoint - (double)i + (double)scale));  //Bit-shift der '0' und berechnung der Dezimalzahl
			}
		}
		if (line[i] == '.') //markieren der Kommastelle -> nach der Berechnung weil sonst '.' berechnet werden würde -> Bit-Shift throwt
		{
			decimalpoint = i;
		}
		if (line[i] == ' ') //abbruch wenn Leerzeichen -> beginn nächster Zahl
		{
			line.erase(i, line.length() - i);
			break;
		}
	}
	if (returnfloat < 0) 
	{ 
		returnfloat = -returnfloat; 
	}
	line.erase(decimalpoint,1); //das Komma '.' entfernen
	for (int i = 0; i < (int)decimalpoint; i++) //über die Vorkommastellen iterieren
	{
		if ((unsigned char)line[i] != '0') //Berechnung für 0 ignorieren
		{
			returnfloat = returnfloat + (double)((unsigned char)line[i] - 48) * std::pow(10.0, ((double)decimalpoint - (double)i - 1.0 + (double)scale)); //Bit-shift der '0' und berechnung der Dezimalzahl
		}
	}
	returnfloat = returnfloat / std::pow(10, scale);
	if (signbit) { return (float)-returnfloat; }
	return (float)returnfloat;
}

unsigned int objfileimport::findnumberuint(std::string& line)
{
	unsigned int returnuint = 0;
	size_t spacelocation = 0;

	if (line.length() == 1)
	{
		return line[0] - 48;
	}
	for (size_t i = line.length(); i > 0; i--)
	{
		if (line[i-1] == ' ')
		{
			spacelocation = i-1;
			break;
		}
		if (line[i-1] != 0)
		{
			returnuint = (unsigned int)(returnuint + (((unsigned char)line[i-1] - 48) * std::pow(10.0, line.length() - (i))));
		}
	}
	line.erase(spacelocation);
	return returnuint;
}