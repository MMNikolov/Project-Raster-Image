/**
 * @file main.cpp
 * @brief Integration tests for validation of PBM and PGM image parser frameworks.
 */

#include "../PBM/PBM.hpp"
#include "../PGM/PGM.hpp"
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

int main() {
    const std::string pbmInput = "input_prime.pbm";
    const std::string pbmOutput = "output_serialized.pbm";
    const std::string pgmInput = "input_prime.pgm";
    const std::string pgmOutput = "output_serialized.pgm";

    try {
        // --- PBM VALIDATION TARGET ---
        std::cout << "[TEST] Generating mock PBM pattern asset...\n";
        generateDummyPBM(pbmInput);

        std::cout << "[TEST] Executing PBM parsing phase...\n";
        PBM pbmImage(pbmInput);
        pbmImage.printSessionInfo();
        
        assert(pbmImage.getWidth() == 3);
        assert(pbmImage.getHeight() == 3);
        assert(pbmImage.isGrayscale() == true);

        PBM* pbmClone = pbmImage.clone();
        assert(pbmClone != nullptr);
        delete pbmClone;

        pbmImage.save(pbmOutput);
        std::cout << "PBM pipeline completed successfully.\n\n";

        // --- PGM VALIDATION TARGET ---
        std::cout << "[TEST] Generating mock PGM pattern asset (MaxVal = 100)...\n";
        generateDummyPGM(pgmInput);

        std::cout << "[TEST] Executing PGM parsing phase...\n";
        PGM pgmImage(pgmInput);
        pgmImage.printSessionInfo();

        std::cout << "Dimensions Verified: " << pgmImage.getWidth() << "x" << pgmImage.getHeight() << "\n";
        assert(pgmImage.getWidth() == 3);
        assert(pgmImage.getHeight() == 3);
        assert(pgmImage.isGrayscale() == true);

        // Verify scaling integrity: 50 out of 100 must map to 127/128 internal intensity
        std::cout << "Verifying middle gray channel normalization value: " 
                  << static_cast<int>(pgmImage.getPixels()[1].r) << " (Expected ~127)\n";
        assert(pgmImage.getPixels()[1].r >= 127 && pgmImage.getPixels()[1].r <= 128);

        std::cout << "[TEST] Executing PGM Prototype replication routine...\n";
        PGM* pgmClone = pgmImage.clone();
        assert(pgmClone != nullptr && pgmClone->getWidth() == 3);
        delete pgmClone;

        std::cout << "[TEST] Executing PGM output writing serialization script...\n";
        pgmImage.save(pgmOutput);
        std::cout << "PGM pipeline completed successfully.\n";

    } catch (const std::exception& error) {
        std::cerr << "\n[CRITICAL FAILURE] Pipeline halted: " << error.what() << "\n";
        return 1;
    }

    return 0;
}