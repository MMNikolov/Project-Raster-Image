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

    this->undoHistory.push_back(cloneCurrentState());

    size_t redoSize = this->redoHistory.size();
    for (size_t i = 0; i < redoSize; i++)
    {
        clearImageVector(this->redoHistory[i]);
    }
    this->redoHistory.clear();

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

    this->undoHistory.push_back(cloneCurrentState());
    
    size_t redoSize = this->redoHistory.size();
    for (size_t i = 0; i < redoSize; i++)
    {
        clearImageVector(this->redoHistory[i]);
    }
    this->redoHistory.clear();

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

    this->undoHistory.push_back(cloneCurrentState());
    
    size_t redoSize = this->redoHistory.size();
    for (size_t i = 0; i < redoSize; i++)
    {
        clearImageVector(this->redoHistory[i]);
    }
    this->redoHistory.clear();

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

void Session::undo()
{
    if (this->undoHistory.empty())
    {
        std::cout << "There is nothing to be undone\n";
        return;
    }
    
    this->redoHistory.push_back(cloneCurrentState());
    clearImageVector(this->loadedImages);
    this->loadedImages = this->undoHistory.back();
    this->undoHistory.pop_back();
    this->count = this->loadedImages.size();
}

void Session::redo()
{
    if (this->redoHistory.empty())
    {
        std::cout << "There is nothing to be redone\n";
        return;
    }
    
    this->undoHistory.push_back(cloneCurrentState());
    clearImageVector(this->loadedImages);
    this->loadedImages = this->redoHistory.back();
    this->redoHistory.pop_back();
    this->count = this->loadedImages.size();
}

void Session::free()
{
    clearImageVector(this->loadedImages);
    this->count = 0;

    size_t undoSize = this->undoHistory.size();
    for (size_t i = 0; i < undoSize; i++)
    {
        clearImageVector(this->undoHistory[i]);
    }
    this->undoHistory.clear();

    size_t redoSize = this->redoHistory.size();
    for (size_t i = 0; i < redoSize; i++)
    {
        clearImageVector(this->redoHistory[i]);
    }
    this->redoHistory.clear();
}

std::vector<Image *> Session::cloneCurrentState() const
{
    std::vector<Image*> newImages;
    //this function allocates (reserves) memory
    //allocates exactly the amount of memory we need for the new Image
    newImages.reserve(this->count);

    for (size_t i = 0; i < this->count; i++)
    {
        newImages.push_back(this->loadedImages[i]->clone());
    }

    return newImages;
}

void Session::clearImageVector(std::vector<Image *> &images)
{
    size_t imagesSize = images.size();

    for (size_t i = 0; i < imagesSize; i++)
    {
        delete images[i];
    }
    
    images.clear();
}
