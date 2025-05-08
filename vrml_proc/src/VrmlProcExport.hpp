#pragma once

#ifdef _WIN32
#define VRMLPROC_API __declspec(dllexport)
#else
#define VRMLPROC_API
#endif