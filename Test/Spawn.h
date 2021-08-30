#pragma once

#include <iostream>
class Spawn
{
private:
	std::string _name;

public:
	Spawn(std::string name) {
		_name = name;
	}
	std::string getName() {
		return _name;
	}

};

