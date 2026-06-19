#include "BMP.hpp"
#include <iostream>

BMP::BMP(const std::string &filename)
    : Image(0, 0, filename), reserved1(0), reserved2(0),
      biXPixelsPerMeter(0), biYPixelsPerMeter(0), biClrUsed(0), biClrImportant(0)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::invalid_argument("Cant open the BMP gfile");
    }

    // Now here comes the fun part
    // I have put in the header file where I got my information from
    char fileHeader[14];
    if (!file.read(fileHeader, 14))
    {
        throw std::invalid_argument("Failed to read BMP file header");
    }

    if (fileHeader[0] != 'B' || fileHeader[1] != 'M')
    {
        throw std::invalid_argument("Not a valid windows bitmap file");
    }

    uint32_t bfOffBits = *reinterpret_cast<uint32_t *>(&fileHeader[10]);

    uint32_t biSize;
    if (!file.read(reinterpret_cast<char *>(&biSize), 4))
    {
        throw std::invalid_argument("Failed to read BMP info header size");
    }

    if (biSize != 40)
    {
        throw std::invalid_argument("Unsupported BMP info header type. Only standard 40-byte headers allowed.");
    }

    int32_t bmpWidth = 0;
    int32_t bmpHeight = 0;
    uint16_t biPlanes = 0;
    uint16_t biBitCount = 0;
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 0;

    file.read(reinterpret_cast<char *>(&bmpWidth), 4);
    file.read(reinterpret_cast<char *>(&bmpHeight), 4);
    file.read(reinterpret_cast<char *>(&biPlanes), 2);
    file.read(reinterpret_cast<char *>(&biBitCount), 2);
    file.read(reinterpret_cast<char *>(&biCompression), 4);
    file.read(reinterpret_cast<char *>(&biSizeImage), 4);
    file.read(reinterpret_cast<char *>(&biXPixelsPerMeter), 4);
    file.read(reinterpret_cast<char *>(&biYPixelsPerMeter), 4);
    file.read(reinterpret_cast<char *>(&biClrUsed), 4);
    file.read(reinterpret_cast<char *>(&biClrImportant), 4);

    if (bmpWidth <= 0 || bmpHeight <= 0)
    {
        throw std::invalid_argument("BMP dimensions must be strictly positive");
    }
    if (biBitCount != 24)
    {
        throw std::invalid_argument("Only uncompressed 24-bit true color BMP file formats are supported");
    }
    if (biCompression != 0)
    {
        throw std::invalid_argument("Compressed BMP format profiles are not supported");
    }

    this->width = bmpWidth;
    this->height = bmpHeight;
    this->pixelData.resize(this->width * this->height);

    // Jump to the pixel array offset position specified in the header block
    file.seekg(bfOffBits, std::ios::beg);

    // Calculate row padding metric to safely cross boundary alignments
    int paddingSize = (4 - (this->width * 3) % 4) % 4;

    // Standard BMP layouts are saved bottom-row-first.
    // Read backwards to populate our top-down 1D vector seamlessly.
    for (int i = this->height - 1; i >= 0; i--)
    {
        for (int j = 0; j < this->width; j++)
        {
            uint8_t bgr[3];
            file.read(reinterpret_cast<char *>(bgr), 3);

            int targetIndex = i * this->width + j;
            // Windows BMP maps channels in Blue-Green-Red sequence
            this->pixelData[targetIndex] = {bgr[2], bgr[1], bgr[0]};
        }
        if (paddingSize > 0)
        {
            file.seekg(paddingSize, std::ios::cur);
        }
    }
}

void BMP::save(const std::string &filename) const
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::invalid_argument("Cannot create the serialization destination file");
    }

    int paddingSize = (4 - (this->width * 3) % 4) % 4;
    uint32_t biSizeImage = (this->width * 3 + paddingSize) * this->height;
    uint32_t bfSize = 14 + 40 + biSizeImage;
    uint32_t bfOffBits = 14 + 40;

    // Assemble the 14-byte file header allocation frame block
    char fileHeader[14] = {'B', 'M', 0};
    *reinterpret_cast<uint32_t *>(&fileHeader[2]) = bfSize;
    *reinterpret_cast<uint16_t *>(&fileHeader[6]) = reserved1;
    *reinterpret_cast<uint16_t *>(&fileHeader[8]) = reserved2;
    *reinterpret_cast<uint32_t *>(&fileHeader[10]) = bfOffBits;

    file.write(fileHeader, 14);

    // Assemble the standard 40-byte info header parameters frame block
    uint32_t biSize = 40;
    int32_t bmpWidth = this->width;
    int32_t bmpHeight = this->height;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 24;
    uint32_t biCompression = 0;

    file.write(reinterpret_cast<const char *>(&biSize), 4);
    file.write(reinterpret_cast<const char *>(&bmpWidth), 4);
    file.write(reinterpret_cast<const char *>(&bmpHeight), 4);
    file.write(reinterpret_cast<const char *>(&biPlanes), 2);
    file.write(reinterpret_cast<const char *>(&biBitCount), 2);
    file.write(reinterpret_cast<const char *>(&biCompression), 4);
    file.write(reinterpret_cast<const char *>(&biSizeImage), 4);
    file.write(reinterpret_cast<const char *>(&biXPixelsPerMeter), 4);
    file.write(reinterpret_cast<const char *>(&biYPixelsPerMeter), 4);
    file.write(reinterpret_cast<const char *>(&biClrUsed), 4);
    file.write(reinterpret_cast<const char *>(&biClrImportant), 4);

    uint8_t zeroPadding[3] = {0, 0, 0};

    // Invert memory array index sequences back into bottom-up arrangement logic
    for (int i = this->height - 1; i >= 0; i--)
    {
        for (int j = 0; j < this->width; j++)
        {
            int sourceIndex = i * this->width + j;
            const Pixel &px = this->pixelData[sourceIndex];
            uint8_t bgr[3] = {px.b, px.g, px.r};
            file.write(reinterpret_cast<const char *>(bgr), 3);
        }
        if (paddingSize > 0)
        {
            file.write(reinterpret_cast<const char *>(zeroPadding), paddingSize);
        }
    }
}

// EVERYTHING FROM HERE ON NOW IS TRIVIAL AND DOESNT REQUIRE ADVANCED THINKING
BMP *BMP::clone() const
{
    return new BMP(*this);
}

void BMP::printSessionInfo() const
{
    std::cout << originalFilename << " 24bit Bitmap format\n";
}

bool BMP::isGrayscale() const
{
    int count = this->width * this->height;
    for (int i = 0; i < count; i++)
    {
        if (this->pixelData[i].r != this->pixelData[i].g || this->pixelData[i].g != this->pixelData[i].b)
        {
            return false;
        }
    }
    return true;
}

void BMP::makeNegative()
{
    int size = this->width * this->height;
    for (int i = 0; i < size; i++)
    {
        this->pixelData[i].r = MAX_VALUE - this->pixelData[i].r;
        this->pixelData[i].g = MAX_VALUE - this->pixelData[i].g;
        this->pixelData[i].b = MAX_VALUE - this->pixelData[i].b;
    }
}

void BMP::makeGrayscale()
{
    int size = this->width * this->height;
    for (int i = 0; i < size; i++)
    {
        uint8_t average = static_cast<uint8_t>((this->pixelData[i].r + this->pixelData[i].g + this->pixelData[i].b) / 3);
        this->pixelData[i] = {average, average, average};
    }
}

void BMP::makeMonochrome()
{
    int size = this->width * this->height;
    for (int i = 0; i < size; i++)
    {
        uint8_t average = static_cast<uint8_t>((this->pixelData[i].r + this->pixelData[i].g + this->pixelData[i].b) / 3);
        if (average > 127)
        {
            this->pixelData[i] = {MAX_VALUE, MAX_VALUE, MAX_VALUE};
        }
        else
        {
            this->pixelData[i] = {0, 0, 0};
        }
    }
}