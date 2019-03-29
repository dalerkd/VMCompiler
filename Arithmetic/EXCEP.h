#pragma once
#include <exception>

class EXCEP : public std::exception
{
public:
	EXCEP(const char* const msg);
	~EXCEP();

	const char * what() const throw ()
	{
		return data;
	}
	const char* const data;
};
