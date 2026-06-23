/**
 * @file ImageFactory.hpp
 * @brief Defines the ImageFactory class for generating polymorphic image objects.
 */

#ifndef IMAGE_FACTORY_HPP
#define IMAGE_FACTORY_HPP

#include "../Image/Image.hpp"
#include "../PGM/PGM.hpp"
#include "../PBM/PBM.hpp"
#include "../PPM/PPM.hpp"
#include "../BMP/BMP.hpp"

/**
 * @class ImageFactory
 * @brief Implements the Factory Method pattern to instantiate concrete image subclasses.
 */
class ImageFactory
{
public:
    /**
     * @brief Creates a concrete Image instance by analyzing file format magic numbers.
     * @param filename Path to the image file to be loaded.
     * @return A pointer to a dynamically allocated concrete Image object (PPM, PGM, PBM, or BMP).
     * @throws std::invalid_argument If the file fails to open or contains an invalid magic identifier.
     */
    static Image* createImage(const std::string& filename);
};

#endif // IMAGE_FACTORY_HPP