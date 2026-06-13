/**
 * @file PGM.hpp
 * @brief Defines the concrete PGM (Portable Graymap) image format parser and serializer.
 */

#ifndef PGM_HPP
#define PGM_HPP

#include "../Image.hpp"
#include <iostream>

/**
 * @brief Concrete class handling P2 ASCII Portable Graymap files.
 * @details Handles gray shading images. Normalizes internal memory pixels 
 * to a standard 24-bit True Color representation across all three RGB channels.
 */

class PGM : public Image
{
public:
    /**
     * @brief Constructor that opens and parses a PGM file from disk.
     * @param filename Path to the source .pgm file.
     * @throws std::invalid_argument If the file cannot be opened or contains invalid formatting.
     */
    PGM(const std::string& filename);

    //No need for a destructor as the std::string manages memory automatically

    /**
     * @brief Saves the current grayscale image state back to disk in P2 ASCII format.
     * @param filename Path where the file will be written.
     */
    void save(const std::string& filename) const override;

    /**
     * @brief Clones the current PGM object using a deep copy (Prototype Pattern).
     * @return A pointer to the newly allocated polymorphic Image object.
     */
    PGM* clone() const override;

    /**
     * @brief Prints the session state and format details exactly as required by the specification.
     */
    void printSessionInfo() const override;

    /**
     * @brief Identifies if the format is grayscale.
     * @return Constantly returns true since graymaps are strictly grayscale fields.
     */
    bool isGrayscale() const override;
    
private:
    int scalableValue; ///< Maximum gray value parameter parsed from the file header */
};

#endif // PGM_HPP