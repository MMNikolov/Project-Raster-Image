/**
 * @file main.cpp
 * @brief Integration tests for verification of the PBM image parser framework.
 */

#include "../PBM/PBM.hpp"
#include <iostream>
#include <fstream>
#include <cassert>

// Utility function to generate a valid test file programmatically
void generateDummyPBM(const std::string& filename) {
    std::ofstream file(filename);
    file << "P1\n";
    file << "# Tiny test matrix mapping cross pattern\n";
    file << "3 3\n";
    file << "1 0 1\n";
    file << "0 1 0\n";
    file << "1 0 1\n";
}

int main() {
    const std::string testInput = "input_prime.pbm";
    const std::string testOutput = "output_serialized.pbm";

    try {
        std::cout << "[TEST] Generating mock PBM test pattern asset...\n";
        generateDummyPBM(testInput);

        std::cout << "[TEST] Executing PBM constructor parsing phase...\n";
        PBM image(testInput);

        std::cout << "\n--- Target Metadata Verification ---\n";
        image.printSessionInfo();
        std::cout << "Dimensions Verified: " << image.getWidth() << "x" << image.getHeight() << "\n";

        // Assert core structural integrity directly
        assert(image.getWidth() == 3);
        assert(image.getHeight() == 3);
        assert(image.isGrayscale() == true);

        std::cout << "\n[TEST] Executing Prototype deep-copy replication routine...\n";
        PBM* polymorphicClone = image.clone();
        
        if (polymorphicClone != nullptr && polymorphicClone->getWidth() == 3) {
            std::cout << "Polymorphic Clone verification succeeded at heap location: " << polymorphicClone << "\n";
        }
        delete polymorphicClone; // Free memory immediately

        std::cout << "\n[TEST] Executing output writing serialization script...\n";
        image.save(testOutput);
        std::cout << "Success! Transformed copy saved under destination marker: " << testOutput << "\n";

    } catch (const std::exception& error) {
        std::cerr << "\n[CRITICAL FAILURE] Pipeline execution halted: " << error.what() << "\n";
        return 1;
    }

    return 0;
}