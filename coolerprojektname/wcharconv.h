#pragma once
#include <string>


class wcharconv
{
public:
	static const char* wc2c(std::wstring) noexcept;
	static const wchar_t* c2wc(std::string oristr) noexcept;
	static char* uc_wc2c(std::wstring) noexcept;
	static wchar_t* uc_c2wc(std::string oristr) noexcept;
	mutable std::string outputbuffer;
	mutable std::wstring woutputbuffer;
};