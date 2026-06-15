#include "Session.hpp"

Session::Session(int sessionId)
    : loadedImages(), count(0)
{    
    if (sessionId < 0)
    {
        throw std::invalid_argument("Cannot have a negative Id");
    }
    
    this->sessionId = sessionId;
}

Session::~Session()
{
    free();
}

void Session::addImage(Image *img)
{
    if (!img)
    {
        throw std::invalid_argument("There isnt an image to be added int Session");
    }
    
    this->loadedImages.push_back(img);
    this->count++;
}

void Session::makeNegative()
{
    if (this->count <= 0)
    {
        throw std::invalid_argument("Cant have a makeNegative operation when there arent any images in the session");
    }

    for (size_t i = 0; i < this->count; i++)
    {
        this->loadedImages[i]->makeNegative();
    }
}

void Session::makeGrayscale()
{
    if (this->count <= 0)
    {
        throw std::invalid_argument("Cant have a makeGrayscale operation when there arent any images in the session");
    }
    
    for (size_t i = 0; i < this->count; i++)
    {
        this->loadedImages[i]->makeGrayscale();
    }
}

void Session::makeMonochrome()
{
    if (this->count <= 0)
    {
        throw std::invalid_argument("Cant have a makeMonochrome operation when there arent any images in the session");
    }
    
    for (size_t i = 0; i < this->count; i++)
    {
        this->loadedImages[i]->makeMonochrome();
    }
}

void Session::printSessionInfo() const
{
    std::cout << "Files in session with ID " << sessionId << ":\n";

    for (size_t i = 0; i < this->count; i++)
    {
        this->loadedImages[i]->printSessionInfo();
    }
}

void Session::free()
{
    for (size_t i = 0; i < this->count; i++)
    {
        delete this->loadedImages[i];
    }
}
