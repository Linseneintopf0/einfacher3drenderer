#include "Ausnahmen.h"
#include <sstream>

Ausnahmen::Ausnahmen(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* Ausnahmen::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Ausnahmen::GetType() const noexcept
{
	return "Sonder Ausnahme";
}

int Ausnahmen::GetLine() const noexcept
{
	return line;
}

const std::string& Ausnahmen::GetFile() const noexcept
{
	return file;
}

std::string Ausnahmen::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}