#include <iostream>
#include <stdexcept>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include "PerlinNoise/PerlinNoise.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

static const auto s_help = "usage: executable.exe filename width height (seed)";

// program arguments filename, width, height, (seed)
int main(int argc, char** argv) try
{
    if (argc < 4) throw std::runtime_error(s_help);
    auto dstFilename = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
	
    uint32_t seed = 420;
    if (argc >= 5) seed = atoi(argv[4]);
	
    if (width < 0 || height < 0) throw std::runtime_error("invalid width or height");

	// storage
    std::vector<uint8_t> pixels;
    pixels.resize(width * height);

    // generate samples
    auto curPixel = pixels.begin();
    siv::BasicPerlinNoise<float> perlin(seed);

    std::cout << "generating texture:\n";
    for (int v = 0; v < height; ++v)
    {
        for (int u = 0; u < width; ++u)
        {
            auto& pixel = *curPixel;

            // convert u, v to direction
            float theta = float(u) * 2.0f * float(M_PI) / float(width);
            float phi = float(v) * float(M_PI) / float(height);

            float y = cos(phi);
            float x = sin(phi) * cos(theta);
            float z = sin(phi) * sin(theta);

            // lookup perlin noise
            float perlinValue = perlin.accumulatedOctaveNoise3D_0_1(x, y, z, 32);
            pixel = uint8_t(perlinValue * 255.0f);

            ++curPixel;
        }
        std::cout << '\r' << v + 1 << "/" << height;
    }
	
    stbi_write_png(dstFilename, width, height, 1, pixels.data(), width);
    
    return 0;
}
catch (const std::exception& e)
{
    std::cerr << "Error: " << e.what();
    return -1;
}