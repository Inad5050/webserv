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

	DebugSink& operator<<(std::ostream& (*)(std::ostream&)) //para tragarse los manipuladores de flujo, como std::endl
	{
		return *this;
	}
};

#ifdef DEBUG
	#define debug std::cout
#else
	#define debug DebugSink()
#endif
