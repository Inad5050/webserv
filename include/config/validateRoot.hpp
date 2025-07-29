#ifndef VALIDATE_ROOT_HPP
#define VALIDATE_ROOT_HPP

#include "../libraries.hpp"

class validateRoot {
	private:
		std::string configFilePath; 
	public:
	void validationRoot();
		validateRoot(const std::string& configFilePath);
};

#endif