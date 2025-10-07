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
			size_t size = 0; //Größe der Zahl -> wird per pointer zrückgegeben
			currentline.erase(0, 2); //das "v " entfernen
			std::string copyofline = currentline; //eine Kopie von der Zeile machen
		
			x = findnumberfloataccurate(copyofline, size); //x durch den Zahlalgrorithmus werfen (reduziert copyofline zu den relevanten Zahlen)
			currentline.erase(0, size); //size wurde als Länger der ersten Zahl zurück gegeben -> entfernen dieser ersten Zahl
			copyofline = currentline; //die Kopie auf die nächste Zahl zurück setzen
		
			y = findnumberfloataccurate(copyofline, size); //der ganze Spaß für y
			currentline.erase(0, size);
			copyofline = currentline;
		
			z = findnumberfloataccurate(copyofline, size); //der ganze Spaß für z
		
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

float objfileimport::findnumberfloataccurate(std::string& line, size_t& size) 
{
	unsigned char signbit = 0; //signbit ist eine Zahl, weil es auch, wenn es 1 ist als Startwert von Zählschleifen dient -> ignorieren des '-'
	size_t decimalpoint = 0; //Stelle des Komma
	size_t numberlength = 0; //Iterator und Speicher der Nummernlänge (wichtig für Exponent)

	unsigned int afterkommadigits = 0; //Nachkommazahlen gepeichert als Nachkommazahlen*10^(Zahlenlänge) -> Integer, keine Float (Fehleranfälliger)
	unsigned int beforecommadigits = 0; //Integer der Vorkommastellen

	if (line[0] == '-')
	{
		signbit = 1; //Wenn ein Minus am Anfang steht -> negativer Returnwert -> effizienter und ermöglicht die folgenden Methoden
	}

	for (numberlength = signbit; numberlength < line.length(); numberlength++) //iterieren mit numberlength über die Zeile
	{
		if (line[numberlength] == '.') //bei dem Punkt der Kommazahl, die Position merken
		{
			decimalpoint = numberlength;
		}
		else if (line[numberlength] == ' ') //bei einem Leerzeichen abbrechen
		{
			line.erase(numberlength, line.length() - numberlength);
			break;
		}
		else if (decimalpoint) //wenn ein Dezimalpunkt gefunden wurde -> Zahlen in int wandeln
		{
			afterkommadigits = afterkommadigits * 10 + (unsigned char)(line[numberlength] - 48); //alte Zahl mit 10 multiplizieren, dann aktuell gelsene Zahl addieren
		}
	}

	numberlength--; //das Leerzeichen am Ende zählt nicht zur Zahlen länge

	for (size_t i = signbit; i < decimalpoint; i++)
	{
		beforecommadigits = beforecommadigits + (unsigned int)((unsigned char)(line[i] - 48) * std::pow(10, i - signbit));
		///bilden der Vorkommastellen "std::pow(10, i-signbit)"
		///steht für 10^(aktuelle 10er Potenz) -> durch addieren 
		///des Signbit im Iterator, hier entfernen
	}

	float returnfloat = (float)beforecommadigits; //erstellen der return Float mit den Vorkommastellen
	int exp = (int)decimalpoint - (int)numberlength; //bilden des Exponent -> "negative Nummernlänge + decimal Punkt" entspricht "decimalpunkt - numernlänge"

	returnfloat = returnfloat + (float)(std::pow(10.0, (double)(exp)) * afterkommadigits); //addieren der Nachkommazahlen -> Kommazahl wird gebildet

	size = line.length()+1; //returnen der Länge

	if (signbit)
	{
		return -returnfloat; //returnen der negativen Werte
	}
	return returnfloat; //returnen aller anderen Werte (positive Floats)
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