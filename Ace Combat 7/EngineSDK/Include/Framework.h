#pragma once


#include "d3d11.h"
#include <functional>
#include <string>
#include <vector>
#include <list>
#include <thread>
#include <memory>
#include <map>

#include "Enums.h"
#include "EngineTypename.h"

#ifdef ENGINE_EXPORT
#define ENGINEDLL _declspec(dllexport)
#else
#define ENGINEDLL _declspec(dllimport)
#endif // ENGINE
