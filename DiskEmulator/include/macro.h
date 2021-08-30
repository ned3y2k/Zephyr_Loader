#pragma once

#define DECLSPEC_EXPORT __declspec(dllexport)

#ifdef DEBUG
#define DEBUG_EXPORT DECLSPEC_EXPORT
#else
#define DEBUG_EXPORT
#endif