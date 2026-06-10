/**
 * @file Pixel.hpp
 * @brief Defines the atomic Pixel structure used across the image manipulation project.
 */

//We use #ifndef instead of pragma once, because it can work on every single C++ compiler the so called standart
//It relies on a preprocessor defining a macro

#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <cstdint>

/**
    * @brief Represents a single RGB Pixel in a raster image
    * @details This structure encapsulates the red, green and blue color channels
    * Architectural note on the data types:
    * We use `uint8_t` (from <cstdint>), because in the 8 bits that we have given it represents the color channels in a perfect manor (0 - 255)
    * Also we use it because it is MUCH MORE memory efficient, if we were to use `int` or `unsigned` this would have cost us 4 bytes (32 bits)
    * we hvae a 75% effieciency increse only by using this variable instead of int
*/

struct Pixel
{
    uint8_t r; /**< Red color channel intensity (0-255) */
    uint8_t g; /**< Green color channel intensity (0-255) */
    uint8_t b; /**< Blue color channel intensity (0-255) */
};

#endif // PIXEL_HPP;