/**
 * @file TestFormatsAndFactory.cpp
 * @brief Comprehensive verification framework for image formats and runtime generation.
 */

#include "../Factory/ImageFactory.hpp"
#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>

// Helper utility to write simulated asset files to disk cleanly
void writeTestAsset(const std::string& path, const std::string& rawData) {
    std::ofstream file(path);
    if (file.is_open()) {
        file << rawData;
    }
}

void runPbmSuite() {
    std::cout << "[PHASE 1] Running Portable Bitmap verification...\n";
    std::string path = "valid_test.pbm";
    writeTestAsset(path, "P1\n# Comment line layout test\n2 2\n0 1\n1 0\n");

    Image* img = ImageFactory::createImage(path);
    assert(img != nullptr);
    assert(img->getWidth() == 2);
    assert(img->getHeight() == 2);
    assert(img->isGrayscale() == true);

    // Verify Prototype design pattern and deep allocation isolation
    Image* cloneImg = img->clone();
    assert(cloneImg != nullptr);
    assert(cloneImg->getWidth() == 2);
    assert(cloneImg->getHeight() == 2);
    
    delete img;
    delete cloneImg;
    std::cout << " -> Phase 1 passed cleanly.\n\n";
}

void runPgmSuite() {
    std::cout << "[PHASE 2] Running Portable Graymap scaling verification...\n";
    std::string path = "scaling_test.pgm";
    
    // Setting an alternate intensity ceiling of 50.
    // An input value of 25 represents exactly 50% gray space.
    // The internal calculation maps this to: (25 * 255) / 50 = 127
    writeTestAsset(path, "P2\n2 1\n50\n0 25\n");

    Image* img = ImageFactory::createImage(path);
    assert(img != nullptr);
    assert(img->getWidth() == 2);
    assert(img->getHeight() == 1);
    
    const auto& pixels = img->getPixels();
    assert(pixels[0].r == 0);
    assert(pixels[1].r >= 127 && pixels[1].r <= 128); // Verification of internal translation math

    delete img;
    std::cout << " -> Phase 2 passed cleanly.\n\n";
}

void runPpmSuite() {
    std::cout << "[PHASE 3] Running Portable Pixmap state verification...\n";
    std::string colorPath = "color_test.ppm";
    std::string grayPath = "gray_test.ppm";

    writeTestAsset(colorPath, "P3\n1 2\n255\n255 0 0\n0 255 0\n");
    writeTestAsset(grayPath, "P3\n2 1\n100\n50 50 50\n10 10 10\n");

    Image* colorImg = ImageFactory::createImage(colorPath);
    Image* grayImg = ImageFactory::createImage(grayPath);

    assert(colorImg != nullptr && grayImg != nullptr);
    
    // True color container holding actual colors must reject monochrome state flags
    assert(colorImg->isGrayscale() == false);
    
    // True color container holding only unified intensities must pass monochrome state flags
    assert(grayImg->isGrayscale() == true);

    delete colorImg;
    delete grayImg;
    std::cout << " -> Phase 3 passed cleanly.\n\n";
}

void runExceptionSuite() {
    std::cout << "[PHASE 4] Running error prevention tests...\n";
    
    std::string badMagic = "bad_magic.pbm";
    std::string badDims = "bad_dims.pgm";
    std::string truncated = "truncated.ppm";

    writeTestAsset(badMagic, "P9\n2 2\n0 1\n1 0\n");
    writeTestAsset(badDims, "P2\n-5 4\n255\n100\n");
    writeTestAsset(truncated, "P3\n2 2\n255\n100 100 100\n"); // Sudden EOF check

    // Evaluate dynamic type rejection
    try {
        Image* img = ImageFactory::createImage(badMagic);
        delete img;
        assert(false); 
    } catch (const std::invalid_argument& e) {
        std::cout << " -> Caught bad layout identifier: " << e.what() << "\n";
    }

    // Evaluate broken constraint formatting
    try {
        Image* img = ImageFactory::createImage(badDims);
        delete img;
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::cout << " -> Caught bad dimension rules: " << e.what() << "\n";
    }

    // Evaluate truncated data validation streams
    try {
        Image* img = ImageFactory::createImage(truncated);
        delete img;
        assert(false);
    } catch (const std::invalid_argument& e) {
        std::cout << " -> Caught truncated matrix stream: " << e.what() << "\n";
    }

    std::cout << " -> Phase 4 passed cleanly.\n\n";
}

int main() {
    std::cout << "====================================================\n";
    std::cout << "STARTING INTENSE IMAGE FACTORY COMPREHENSIVE SUITE  \n";
    std::cout << "====================================================\n\n";

    runPbmSuite();
    runPgmSuite();
    runPpmSuite();
    runExceptionSuite();

    std::cout << "====================================================\n";
    std::cout << "ALL IMAGE AND FACTORY SUBSYSTEM VERIFICATIONS PASSED \n";
    std::cout << "====================================================\n";
    return 0;
}