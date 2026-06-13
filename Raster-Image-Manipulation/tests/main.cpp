/**
 * @file main.cpp
 * @brief Integration tests for validation of PBM, PGM, and PPM image parser frameworks.
 */

#include "../PBM/PBM.hpp"
#include "../PGM/PGM.hpp"
#include "../PPM/PPM.hpp"
#include <iostream>
#include <fstream>
#include <cassert>

void generateDummyPBM(const std::string& filename) {
    std::ofstream file(filename);
    file << "P1\n";
    file << "# Tiny test matrix\n";
    file << "3 3\n";
    file << "1 0 1\n";
    file << "0 1 0\n";
    file << "1 0 1\n";
}

void generateDummyPGM(const std::string& filename) {
    std::ofstream file(filename);
    file << "P2\n";
    file << "# Test scaling math with MaxVal = 100\n";
    file << "3 3\n";
    file << "100\n"; 
    file << "0 50 100\n";
    file << "50 100 50\n";
    file << "100 50 0\n";
}

void generateDummyPPMColor(const std::string& filename) {
    std::ofstream file(filename);
    file << "P3\n";
    file << "# Test true color PPM channel distribution\n";
    file << "2 2\n";
    file << "255\n";
    file << "255 0 0    0 255 0\n";   // Red, Green
    file << "0 0 255    255 255 255\n"; // Blue, White
}

void generateDummyPPMGray(const std::string& filename) {
    std::ofstream file(filename);
    file << "P3\n";
    file << "# Test grayscale PPM (Color format holding only gray pixels)\n";
    file << "2 2\n";
    file << "255\n";
    file << "100 100 100    200 200 200\n";
    file << "50 50 50        255 255 255\n";
}

int main() {
    // Filenames definitions
    const std::string pbmInput = "input_prime.pbm";
    const std::string pbmOutput = "output_serialized.pbm";
    const std::string pgmInput = "input_prime.pgm";
    const std::string pgmOutput = "output_serialized.pgm";
    const std::string ppmColorInput = "input_color.ppm";
    const std::string ppmColorOutput = "output_color_serialized.ppm";
    const std::string ppmGrayInput = "input_gray.ppm";
    const std::string ppmGrayOutput = "output_gray_serialized.ppm";

    try {
        // ====================================================================
        // PBM VALIDATION
        // ====================================================================
        std::cout << "[TEST] Generating mock PBM asset...\n";
        generateDummyPBM(pbmInput);

        std::cout << "[TEST] Parsing PBM file...\n";
        PBM pbmImage(pbmInput);
        pbmImage.printSessionInfo(); 
        
        assert(pbmImage.getWidth() == 3);
        assert(pbmImage.getHeight() == 3);
        assert(pbmImage.isGrayscale() == true);

        PBM* pbmClone = pbmImage.clone();
        assert(pbmClone != nullptr);
        delete pbmClone;

        pbmImage.save(pbmOutput);
        std::cout << "-> PBM validation passed.\n\n";

        // ====================================================================
        // PGM VALIDATION
        // ====================================================================
        std::cout << "[TEST] Generating mock PGM asset (MaxVal = 100)...\n";
        generateDummyPGM(pgmInput);

        std::cout << "[TEST] Parsing PGM file...\n";
        PGM pgmImage(pgmInput);
        pgmImage.printSessionInfo(); 

        assert(pgmImage.getWidth() == 3);
        assert(pgmImage.getHeight() == 3);
        assert(pgmImage.isGrayscale() == true);
        assert(pgmImage.getPixels()[1].r >= 127 && pgmImage.getPixels()[1].r <= 128); // Normalization check

        PGM* pgmClone = pgmImage.clone();
        assert(pgmClone != nullptr);
        delete pgmClone;

        pgmImage.save(pgmOutput);
        std::cout << "-> PGM validation passed.\n\n";

        // ====================================================================
        // PPM TRUE COLOR VALIDATION
        // ====================================================================
        std::cout << "[TEST] Generating true color PPM asset...\n";
        generateDummyPPMColor(ppmColorInput);

        std::cout << "[TEST] Parsing true color PPM file...\n";
        PPM ppmColorImage(ppmColorInput);
        ppmColorImage.printSessionInfo(); 

        assert(ppmColorImage.getWidth() == 2);
        assert(ppmColorImage.getHeight() == 2);
        
        // A true color image must evaluate to false for grayscale properties 
        std::cout << "[TEST] Verifying color-field grayscale evaluation rejection...\n";
        assert(ppmColorImage.isGrayscale() == false); 

        PPM* ppmColorClone = ppmColorImage.clone();
        assert(ppmColorClone != nullptr);
        delete ppmColorClone;

        ppmColorImage.save(ppmColorOutput);
        std::cout << "-> PPM True Color validation passed.\n\n";

        // ====================================================================
        // PPM GRAYSCALE-IN-COLOR VALIDATION (Theme 7 Edge Case)
        // ====================================================================
        std::cout << "[TEST] Generating grayscale-in-color PPM asset...\n";
        generateDummyPPMGray(ppmGrayInput);

        std::cout << "[TEST] Parsing grayscale-in-color PPM file...\n";
        PPM ppmGrayImage(ppmGrayInput);
        ppmGrayImage.printSessionInfo(); 

        assert(ppmGrayImage.getWidth() == 2);
        assert(ppmGrayImage.getHeight() == 2);
        
        // This color format file contains only gray shades, it must evaluate to true 
        std::cout << "[TEST] Verifying color-field grayscale evaluation acceptance...\n";
        assert(ppmGrayImage.isGrayscale() == true); 

        ppmGrayImage.save(ppmGrayOutput);
        std::cout << "-> PPM Grayscale-in-Color validation passed.\n\n";

        std::cout << "====================================================\n";
        std::cout << "ALL PARSER DATA LAYER PIPELINE TESTS PASSED CLEANLY\n";
        std::cout << "====================================================\n";

    } catch (const std::exception& error) {
        std::cerr << "\n[CRITICAL FAILURE] Validation halted: " << error.what() << "\n";
        return 1;
    }

    return 0;
}