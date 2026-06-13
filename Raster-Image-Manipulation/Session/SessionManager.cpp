#include "SessionManager.hpp"

//the correct use of std::string needs to be an empty string, not like the char* = nullptr
SessionManager::SessionManager()
    : currentImage(nullptr), currentFilename(""), isSessionActive(false)
{}

SessionManager::~SessionManager()
{
    if (currentImage != nullptr)
    {
        free();
    }
}

void SessionManager::openSession(const std::string &filepath)
{
    if (isSessionActive)
    {
        throw std::invalid_argument("Cannot open another session when the current one is in progress!");
    }
    
    this->currentImage = ImageFactory::createImage(filepath);
    //everything is good here, because std::string handles its copy ctor :D
    this->currentFilename = filepath;
    this->isSessionActive = true;

    std::cout << "Succesdully opened the session" << this->currentFilename << '\n';
}

void SessionManager::closeSession()
{
    if (!isSessionActive)
    {
        throw std::invalid_argument("There is no current session to close :D");
    }
    
    free();

    this->currentImage = nullptr;
    this->currentFilename = "";
    this->isSessionActive = false;

    std::cout << "Succesdully closed the session" << this->currentFilename << '\n';
}

void SessionManager::saveSession() const
{
    if (!isSessionActive)
    {
        throw std::invalid_argument("Cant save a session when there is nothing in it :D");
    }
    
    //polymorphic save
    this->currentImage->save(this->currentFilename);

    std::cout << "Succesdully saved (default) the session" << this->currentFilename << '\n';
}

void SessionManager::saveSessionAs(const std::string &filepath)
{
    if (!isSessionActive)
    {
        throw std::invalid_argument("Cant save a session when there is nothing in it :D");
    }
    
    this->currentImage->save(filepath);
    // Once again the copy ctor of the std::string handles the situation
    this->currentFilename = filepath;

    std::cout << "Succesdully saved (by FilePath) the session" << this->currentFilename << '\n';
}

void SessionManager::free()
{
    delete this->currentImage;
}
