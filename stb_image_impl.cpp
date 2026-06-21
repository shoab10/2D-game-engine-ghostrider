// Single translation unit that compiles the stb_image implementation.
// stb_image replaces the SDL_image PNG/JPG loader, which produces blank
// (all-zero) surfaces when built against sdl12-compat.
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wsign-compare"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_BMP
#include "stb_image.h"
