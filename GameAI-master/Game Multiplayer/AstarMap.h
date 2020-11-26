#pragma once
#include <array>

#include "GameDefine.h"

class AstarMap {
public:
	// This is how clients can access the single instance
	static AstarMap* getInstance();

	std::array < std::array<int, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> mapInt;

private:
	static AstarMap* inst_;   // The one, single instance
	AstarMap() {} // private constructor
	AstarMap(const AstarMap&);
	AstarMap& operator=(const AstarMap&);
};

// Define the static Singleton pointer
AstarMap* AstarMap::inst_ = NULL;

AstarMap* AstarMap::getInstance() {
	if (inst_ == NULL) {
		inst_ = new AstarMap();
	}
	return(inst_);
}
