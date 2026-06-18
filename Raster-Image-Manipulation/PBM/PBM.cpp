/**
 * @file PBM.cpp
 * @brief Implements the PBM file parsing, serialization, and polymorphic behaviors.
 */

#include "PBM.hpp"

PBM::PBM(const std::string &filename)
    : Image(0, 0, filename) // will be polpulated in the pasring
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::invalid_argument("Cant open the file in PBM");
    }

    std::string magicNumber;
    file >> magicNumber;
    if (magicNumber != "P1")
    {
        throw std::invalid_argument("Not a valid portable bitmap");
    }

    skipComments(file);
    if (!(file >> width >> height))
    {
        throw std::invalid_argument("Invalid dimensions");
    }

    if (this->width <= 0 || this->height <= 0)
    {
        throw std::invalid_argument("Cant have dimensions below zero");
    }

    pixelData.resize(this->width * this->height);

    int val;
    for (int i = 0; i < this->width * this->height; i++)
    {
        if (!(file >> val))
        {
            throw std::invalid_argument("Invalid pixel");
        }
        else if (val != 0 && val != 1)
        {
            throw std::invalid_argument("There cant be any pixels other than 0 or 1");
        }

        if (val == 0)
        {
            pixelData[i] = {255, 255, 255}; // White
        }
        else if (val == 1)
        {
            pixelData[i] = {0, 0, 0}; // Black
        }
    }
}

void PBM::save(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::invalid_argument("Cant open the file in PBM");
    }

    file << "P1\n"
         << this->width << ' ' << this->height << '\n';

    // we made a seperate variable consuming 4 more bytes, because through each itteration it has to do an arithmetic
    // if we do this.width * this.height its much more inneficient
    int count = this->width * this->height;
    for (int i = 0; i < count; i++)
    {
        const Pixel &pixel = this->pixelData[i];

        uint8_t intensity = (pixel.b + pixel.g + pixel.r) / 3;

        if (intensity > 127)
        {
            file << '0'; // because when the inetensity gets bigger it gets closer to the
                         // absolute value of the white which is { 255, 255, 255 }
        }
        else
        {
            file << '1'; // same here when the intensity is < 127 its gets closer to {0, 0, 0}
        }

        // we read till the end of the file and when we reach it we go to the next row
        // and when the file is not in the end still we just put a ws
        if ((i + 1) % width == 0)
        {
            file << '\n';
        }
        else
        {
            file << ' ';
        }
    }
}

PBM *PBM::clone() const
{
    return new PBM(*this);
}

void PBM::printSessionInfo() const
{
    std::cout << originalFilename << " portable bitmap format \n";
}

bool PBM::isGrayscale() const
{
    return true;
}

void PBM::makeNegative()
{
    int size = this->width * this->height;
    for (int i = 0; i < size; i++)
    {
        this->pixelData[i].r = 255 - this->pixelData[i].r;
        this->pixelData[i].g = 255 - this->pixelData[i].g;
        this->pixelData[i].b = 255 - this->pixelData[i].b;
    }
}

// these two do nothing
//------------------------------
void PBM::makeGrayscale()
{
    return;
}

void PBM::makeMonochrome()
{
    return;
}
//------------------------------
