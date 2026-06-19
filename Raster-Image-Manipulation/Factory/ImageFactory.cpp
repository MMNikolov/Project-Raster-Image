#include "ImageFactory.hpp"

Image *ImageFactory::createImage(const std::string &filename)
{
    std::ifstream checkFile(filename, std::ios::binary);
    if (!checkFile.is_open())
    {
        throw std::invalid_argument("Cannot open the file in ImageFactory");
    }

    char magicBytes[2];
    if (checkFile.read(magicBytes, 2))
    {
        if (magicBytes[0] == 'B' && magicBytes[1] == 'M')
        {
            checkFile.close();
            return new BMP(filename);
        }
    }
    checkFile.close();

    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::invalid_argument("Cannot open the file in ImageFactory");
    }

    Image::skipComments(file);
    std::string magicNumber;
    if (!(file >> magicNumber))
    {
        throw std::invalid_argument("cant read the number in ImageFactory");
    }

    if (magicNumber == "P1")
    {
        return new PBM(filename);
    }
    else if (magicNumber == "P2")
    {
        return new PGM(filename);
    }
    else if (magicNumber == "P3")
    {
        return new PPM(filename);
    }

    throw std::invalid_argument("The magic number isnt one of the specified");
}