/**
 * @file PPM.hpp
 * @brief Defines the concrete PPM (Portable Pixmap) image format parser and serializer.
 */

#ifndef PPM_HPP
#define PPM_HPP

#include "../Image.hpp"
#include <iostream>

#define ITTERATIONS_TO_READ_PIXEL 3

/**
 * @brief Concrete class handling P3 ASCII Portable Pixmap files.
 * @details Manages full 24-bit color processing by storing distinct individual Red,
 * Green, and Blue color channel data streams for every coordinate mapped.
 */
class PPM : public Image
{
public:
    /**
     * @brief Constructor that opens, validates, and parses a PPM file from disk.
     * @param filename Path to the source .ppm file.
     * @throws std::invalid_argument If the file stream fails or channel data is out of bounds.
     */
    PPM(const std::string& filename);

    //no destructor

    //overriden methods

    /**
     * @brief Saves the current color image state back to disk in P3 ASCII format.
     * @param filename Path where the file will be written.
     */
    void save(const std::string& filename) const override;

    /**
     * @brief Clones the current PPM object using a deep copy (Prototype Pattern).
     * @return A pointer to the newly allocated polymorphic Image object.
     */
    PPM* clone() const override;

    /**
     * @brief Prints the session state and format details exactly as required by the specification.
     */
    void printSessionInfo() const override;

    /**
     * @brief Scans individual pixels to determine if the active image is monochrome or gray.
     * @details Runs a evaluation pass through the pixel sequence. Returns true only if
     * every single color coordinate has an identical mix across its R, G, and B components.
     * @return True if the image contains exclusively grayscale shades, false if true color is present.
     */
    bool isGrayscale() const override;
    
private:
    int maxColorValue; /**< Maximum color intensity range parsed from the header */
};

#endif //PPM_HPP