#pragma once

#include <iostream>
#include <ostream>

class DebugSink 
{
	public:

	template<typename T>
	DebugSink& operator<<(const T&) 
	{
		return *this;
	}

	DebugSink& operator<<(std::ostream& (*)(std::ostream&)) 
	{
		return *this;
	}
};

#ifdef NDEBUG
	#define debug std::cout
#else
	#define debug DebugSink()
#endif
