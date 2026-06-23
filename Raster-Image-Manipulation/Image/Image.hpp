/**
 * @file Image.hpp
 * @brief Defines the abstract base class for all raster image formats.
 */

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <vector>
#include <string>
#include <fstream>
#include "../Pixel.hpp"

#define MAX_VALUE 255

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
    Image(int w, int h, const std::string& filename);
        
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

    /**
     * @brief Inverts the color intensities of the active image pixels.
     */
    virtual void makeNegative() = 0;
    /**
     * @brief Makes the image in the given session grayscale
     */
    virtual void makeGrayscale() = 0;
    /**
     * @brief Makes the image in the given session monochrome
     */
    virtual void makeMonochrome() = 0;

    /**
     * @brief Rotates the image 90 degrees counter-clockwise.
     * @details Allocates a temporary buffer on the heap, maps the flat 1D pixel coordinates 
     * to their new spatial orientation, updates the internal memory buffer, and swaps the 
     * image width and height dimensions.
     */
    virtual void rotateLeft();

    /**
     * @brief Rotates the image 90 degrees clockwise.
     * @details Allocates a temporary buffer on the heap, maps the flat 1D pixel coordinates 
     * to their new spatial orientation, updates the internal memory buffer, and swaps the 
     * image width and height dimensions.
     */
    virtual void rotateRight();

    /**
     * @brief Flips the image vertically across the horizontal axis (top-to-bottom mirror).
     * @details Reverses the row sequence in place within the flat 1D memory array without 
     * changing the overall image width or height dimensions.
     */
    virtual void flipTop();

    /**
     * @brief Flips the image horizontally across the vertical axis (left-to-right mirror).
     * @details Reverses the column elements within each individual row in place inside the 
     * flat 1D memory array without changing the overall image dimensions.
     */
    virtual void flipLeft();

    /**
     * @brief Helper function to skip Netpbm style comment lines during file parsing.
     * @param file The active input file stream.
     */
    static void skipComments(std::ifstream& filename);

    /**
     * @brief Stitches pixel blocks from a source image into this image matrix.
     * @param source Pointer to the polymorphic source image to copy from.
     * @param posX The horizontal coordinate offset (top-left column entry point).
     * @param posY The vertical coordinate offset (top-left row entry point).
     */
    void paste(const Image* source, int posX, int posY);

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

    /**
     * @brief Retrieves the original filename of the image resource.
     * @return The source file path string.
     */
    std::string getFilename() const { return originalFilename; }

    // We use protected so that the "children" can inherit the traits that the abstract class possesses
    // Has to read a lot of documentation on how to work with std::string for cleaner work flow, because till this moment we have been working only with char*
protected:

    int width; /**< Image width in pixels */
    int height; /**< Image height in pixels */
    std::string originalFilename; /**< The source file name of the image */
    std::vector<Pixel> pixelData; /**< 1D array of pixels representing the 2D image */
};

#endif //IMAGE_HPP
