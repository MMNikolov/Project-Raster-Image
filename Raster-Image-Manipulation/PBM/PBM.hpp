/**
 * @file PBM.hpp
 * @brief Defines the concrete PBM (Portable BitMap) image format parser and serializer.
 */

#ifndef PBM_HPP
#define PBM_HPP

#include "../Image/Image.hpp"
#include <iostream>

/**
 * @brief Concrete class handling P1 ASCII PBM files.
 * @details In PBM format, images are strictly monochrome (black and white).
 * The specification dictates that '0' is a white pixel and '1' is a black pixel.
 */

class PBM : public Image
{
public:
/**
     * @brief Constructor that opens and parses a PBM file from disk.
     * @param filename Path to the source .pbm file.
     * @throws std::invalid_argument If the file cannot be opened or contains invalid formatting.
     */
    PBM(const std::string& filename);

    //no need for a destrcutor
    //~PBM();

    //overriden methods
    /**
     * @brief Saves the current image state back to disk in P1 ASCII format.
     * @param filename Path where the file will be written.
     */
    void save(const std::string& filename) const override;

    /**
     * @brief Clones the current PBM object using a deep copy (Prototype Pattern). This will be useful for the polymorphic structure.
     * @return A pointer to the newly allocated polymorphic Image object.
     */
    PBM* clone() const override;

    /**
     * @brief Prints the session state and format details of this file.
     */
    void printSessionInfo() const override;

    /**
     * @brief Identifies if the format is grayscale.
     * @return Constantly returns true since monochrome is a subset of grayscale.
     */
    bool isGrayscale() const override; 

    /**
     * @brief Inverts the black and white fields of the monochrome image.
     */
    void makeNegative() override;
    /**
     * @brief Does nothing here.
     */
    void makeGrayscale() override;
    /**
     * @brief Does nothing here.
     */
    void makeMonochrome() override;

    void rotateLeft() override;
    void rotateRight() override;
    void flipTop() override;
    void flipLeft() override;
};

#endif // PBM_HPP