#pragma once

#include <iostream>
#include <memory.h>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <cerrno>

#include "Sol/Core/Log.h"

#include "Sol/Debug/Instrumentor.h"

#ifdef SOL_PLATFORM_WINDOWS
	#include<Windows.h>
#endif