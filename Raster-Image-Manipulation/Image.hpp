/**
 * @file Image.hpp
 * @brief Defines the abstract base class for all raster image formats.
 */

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <vector>
#include <string>
#include <fstream>
#include "Pixel.hpp"

/**
 * @brief abstract base class represening a raster image
 * @details This class represents the polymorphic interface for all specific image formats
 * (e.g. PPM - Portable Pixmap Format | PGM - portable Grey map | PBM - Portable BitMap | BMP - Bitmap)
 * It encapsulates the core data (width, height and pixels) and defines virtual methods that derived classes must implement
 * Architectural Note: 
 * The `pixelData` is stored in a 1D vector rather than a 2D array
 * This boosts performance when applying transformations to large images
 */

class Image
{
public:
    /**
     * @brief Constructor initializing core dimensions and filename.
     * @param w Width of the image.
     * @param h Height of the image.
     * @param filename Original filename of the loaded image.
     */
    Image(int w, int h, const std::string& filename) 
        : width(w), height(h), originalFilename(filename) {}
        
    /**
     * @brief Virtual destructor for proper polymorphic cleanup.
     */
    virtual ~Image() = default;

    //METHODS

    /**
     * @brief Saves the image to disk.
     * @param filename The path where the new image should be saved.
     */
    virtual void save(const std::string& filename) const = 0;

    /**
     * @brief Creates a deep copy of the image (Prototype pattern).
     * @return Pointer to the newly cloned Image object.
     */
    virtual Image* clone() const = 0;

    /**
     * @brief Prints format-specific information for the session list.
     */
    virtual void printSessionInfo() const = 0;
    
    /**
     * @brief Checks if the image contains only grayscale pixels.
     * @return True if grayscale, false otherwise.
     */
    virtual bool isGrayscale() const = 0; 

    //GETTERS

    /**
     * @brief Retrieves the image width.
     * @return Width in pixels.
     */
    int getWidth() const { return width; }

    /**
     * @brief Retrieves the image height.
     * @return Height in pixels.
     */
    int getHeight() const { return height; }

    /**
     * @brief Provides read-only access to the pixel data.
     * @return Const reference to the 1D pixel vector.
     */
    const std::vector<Pixel>& getPixels() const { return pixelData; }

    /**
     * @brief Provides mutable access to the pixel data.
     * @return Reference to the 1D pixel vector.
     */
    std::vector<Pixel>& getPixels() { return pixelData; }

    // We use protected so that the "children" can inherit the traits that the abstract class possesses
    // had to watch a lot of videos to understand how to work with string, because for now we hvae been working with char* only
protected:

    int width; /**< Image width in pixels */
    int height; /**< Image height in pixels */
    std::string originalFilename; /**< The source file name of the image */
    std::vector<Pixel> pixelData; /**< 1D array of pixels representing the 2D image */
};

#endif //IMAGE_HPP
