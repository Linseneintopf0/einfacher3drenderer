#include "wcharconv.h"
#include <vector>
#include <sstream>
#include "Window.h"

const char* wcharconv::wc2c(std::wstring oristr) noexcept
{
	int size = WideCharToMultiByte(CP_UTF8, 0, oristr.c_str(), (int)oristr.size(), NULL, 0, NULL, NULL);
	std::string outputbuffer(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, oristr.c_str(), (int)oristr.size(), &outputbuffer[0], size, NULL, NULL);
	const char* output = outputbuffer.c_str();
	return output;
}

const wchar_t* wcharconv::c2wc(std::string oristr) noexcept
{	
	int size = MultiByteToWideChar(CP_UTF8, 0, oristr.c_str(), (int)oristr.size(), NULL, 0);
	std::wstring woutputbuffer(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, oristr.c_str(), (int)oristr.size(), &woutputbuffer[0], size);
	const wchar_t* output = woutputbuffer.c_str();
	return output;
}
char* wcharconv::uc_wc2c(std::wstring oristr) noexcept
{
	int size = WideCharToMultiByte(CP_UTF8, 0, oristr.c_str(), (int)oristr.size(), NULL, 0, NULL, NULL);
	std::string outputbuffer(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, oristr.c_str(), (int)oristr.size(), &outputbuffer[0], size, NULL, NULL);
	char* output = &outputbuffer[0];
	return output;
}

wchar_t* wcharconv::uc_c2wc(std::string oristr) noexcept
{
	int size = MultiByteToWideChar(CP_UTF8, 0, oristr.c_str(), (int)oristr.size(), NULL, 0);
	std::wstring woutputbuffer(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, oristr.c_str(), (int)oristr.size(), &woutputbuffer[0], size);
	wchar_t* output = &woutputbuffer[0];
	return output;
}