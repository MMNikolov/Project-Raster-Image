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