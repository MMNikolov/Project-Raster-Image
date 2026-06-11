/**
 * @file PGM.hpp
 * @brief Defines the concrete PGM (Portable Graymap) image format parser and serializer.
 */

#ifndef PGM_HPP
#define PGM_HPP

#include "../Image.hpp"
#include <iostream>

#define MAX_VALUE 255

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

    void save(const std::string& filename) const override;
    PGM* clone() const override;
    void printSessionInfo() const override;
    bool isGrayscale() const override;
    
private:
    int scalableValue; ///< Maximum gray value parameter parsed from the file header */
};

#endif // PGM_HPP