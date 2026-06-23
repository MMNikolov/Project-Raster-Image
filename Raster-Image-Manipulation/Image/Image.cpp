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

void Image::paste(const Image *source, int posX, int posY)
{
    if (!source)
    {
        throw std::invalid_argument("Cant paste any image if it doesnt exist :DDD");
    }
    if (posX < 0 || posY < 0)
    {
        throw std::invalid_argument("Positions cannot be negative");
    }
    
    int screenWidth = source->getWidth();
    int screenHeight = source->getHeight();

    int requiredWidth = (posX + screenWidth > this->width) ? (posX + screenWidth) : this->width;
    int requiredHeight = (posY + screenHeight > this->height) ? (posY + screenHeight) : this->height;

    if (requiredHeight > this->height || requiredWidth > this->width)
    {
        // po uslovie e kazano che trqbva da se zapulnqt s beli pixelcheta
        std::vector<Pixel> expandedPixels(requiredWidth * requiredHeight, {255, 255, 255});

        for (size_t i = 0; i < this->height; i++)
        {
            for (size_t j = 0; j < this->width; j++)
            {
                expandedPixels[i * requiredWidth + j] = this->pixelData[i * this->width + j];
            }
            
        }
        
        this->pixelData = expandedPixels;
        this->width = requiredWidth;
        this->height = requiredHeight;
    }
    
    for (int i = 0; i < screenHeight; i++)
    {
        for (int j = 0; j < screenWidth; j++)
        {
            int targetRow = posY + i;
            int targetCol = posX + j;

            int sourceIndex = i * screenWidth + j;
            int targetIndex = targetRow * this->width + targetCol;

            this->pixelData[targetIndex] = source->getPixels()[sourceIndex];
        }
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
