#pragma once

#ifdef _WIN32
#define TOGEOM_API __declspec(dllexport)
#else
#define TOGEOM_API
#endif