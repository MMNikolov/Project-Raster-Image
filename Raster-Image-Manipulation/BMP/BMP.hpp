/**
 * @file BMP.hpp
 * @brief Defines the concrete BMP (Windows Bitmap) image format parser and serializer.
 */

#ifndef BMP_HPP
#define BMP_HPP

#include "../Image/Image.hpp"

/**
 * @class BMP
 * @brief Concrete class handling uncompressed 24-bit true color Windows Bitmap files.
 */
class BMP : public Image
{
public:
    /**
     * @brief Constructor that opens, validates, and parses a BMP file from disk.
     * @param filename Path to the source .bmp file.
     * @throws std::invalid_argument If the file stream fails or formatting is unsupported.
     */
    BMP(const std::string &filename);

    // THERE IS NO NEED FOR A DESTRUCTOR AS WE DONT HAVE DYNAMICALLY ALLOCATED MEMORY TO WORK WITH

    /**
     * @brief Saves the current image state back to disk in 24-bit binary BMP format.
     * @param filename Path where the file will be written.
     */
    void save(const std::string &filename) const override;

    /**
     * @brief Clones the current BMP object using a deep copy (Prototype Pattern).
     * @return A pointer to the newly allocated polymorphic Image object.
     */
    BMP *clone() const override;

    /**
     * @brief Prints format profile details exactly as required by the specification.
     */
    void printSessionInfo() const override;

    /**
     * @brief Scans individual pixels to determine if the active image is grayscale shades.
     * @return True if the image contains exclusively gray balances, false if color is present.
     */
    bool isGrayscale() const override;

    /**
     * @brief Inverts the BMP color intensity channels.
     */
    void makeNegative() override;

    /**
     * @brief Converts all pixels to their calculated grayscale averages.
     */
    void makeGrayscale() override;

    /**
     * @brief Forces pixels into absolute white or absolute black states based on intensity.
     */
    void makeMonochrome() override;

private:
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t biXPixelsPerMeter;
    uint32_t biYPixelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

#endif // BMP_HPP

//where I got my information from:
// - https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/
//this is written for the C language, but the main idea is still there
// - https://elcharolin.wordpress.com/2018/11/28/read-and-write-bmp-files-in-c-c/
