#pragma once
#include <string>
#include <unordered_map>
#include "windowsexcept.h"

//Klasse für das Debugen von Nachichten und das Erstellen eines Log-Fensters

class Outputlog
{
public:
	static void AttachConsoleWindow();
	static std::string msgIdConv(UINT msg);
	static std::unordered_map<unsigned int, std::string> msglist;
private:
};