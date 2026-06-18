#include "Image.hpp"

Image::Image(int w, int h, const std::string& name)
    : width(w), height(h), originalFilename(name) {}

void Image::skipComments(std::ifstream& file)
{
    while (file >> std::ws && file.peek() == '#')
    {
        std::string dummy;
        std::getline(file, dummy);
    }
}

void Image::rotateLeft()
{
    std::vector<Pixel> newPixelData(this->height * this->width);

    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            int oldIndex = this->width * i + j;
            int newIndex = (this->width - 1 - j) * this->height + i;

            newPixelData[newIndex] = this->pixelData[oldIndex];
        }
    }
    
    this->pixelData = newPixelData;
    int temp = this->width;
    this->width = this->height;
    this->height = temp;
}

void Image::rotateRight()
{
    std::vector<Pixel> newPixelData(this->width * this->height);

    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            int oldIndex = this->width * i + j;
            int newIndex = (this->height - 1 - i) + this->height * j;

            newPixelData[newIndex] = this->pixelData[oldIndex];
        }
    }
    
    this->pixelData = newPixelData;
    int temp = this->height;
    this->height = this->width;
    this->width = temp;
}

void Image::flipTop()
{
    for (int i = 0; i < this->height / 2; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            int topIndex = this->width * i + j;
            int bottomIndex = (this->height - 1 - i) * this->width + j;
            std::swap(this->pixelData[topIndex], this->pixelData[bottomIndex]);
        }
    }
}

void Image::flipLeft()
{
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width / 2; j++)
        {
            int leftIndex = this->width * i + j;
            int rightIndex = (this->width - 1 - j) + this->width * i;
            std::swap(this->pixelData[leftIndex], this->pixelData[rightIndex]);
        }
    }
}
