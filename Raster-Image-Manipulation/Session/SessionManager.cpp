#include "SessionManager.hpp"

SessionManager::SessionManager()
    : currentActiveId(-1), idCounter(1), activeSessions()
{
}

SessionManager::~SessionManager()
{
    free();
}

void SessionManager::loadSession(const std::vector<std::string> &filepaths)
{
    if (filepaths.empty())
    {
        throw std::invalid_argument("Cant have a load session functionality if there isnt a file path");
    }

    Session *newSession = new Session(idCounter);
    size_t filepathsCount = filepaths.size();

    try
    {
        for (size_t i = 0; i < filepathsCount; i++)
        {
            Image *img = ImageFactory::createImage(filepaths[i]);
            newSession->addImage(img);
        }
    }
    catch (const std::exception &e)
    {
        // we are doing a rethrow to delete the dynamic memory safely and then rethrow the exception to catch it again :D
        delete newSession;
        throw;
    }

    activeSessions.push_back(newSession);
    std::cout << "Session with ID: " << idCounter << " started\n";

    this->currentActiveId = idCounter;
    idCounter++;
}

void SessionManager::addImagetoActiveSession(const std::string &filename)
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("There has to be an active session to add image to it");
    }
    
    int index = this->findSessionIndex(currentActiveId);
    this->activeSessions[index]->addByFilename(filename);
}

void SessionManager::switchSession(int sessionId)
{
    if (findSessionIndex(sessionId) == -1)
    {
        throw std::invalid_argument("Couldnt find the session id");
    }

    this->currentActiveId = sessionId;
    std::cout << "Switched active focus to session " << sessionId << "\n";
}

void SessionManager::closeSessionById(int sessionId)
{   
    if (sessionId == -1)
    {
        if (this->currentActiveId == -1)
        {
            throw std::invalid_argument("Cannot close a session when there are no active sessions open.");
        }
        sessionId = this->currentActiveId;
    }
    
    int index = findSessionIndex(sessionId);
    if (index == -1)
    {
        throw std::invalid_argument("Couldnt find the session id");
    }

    delete activeSessions[index];
    activeSessions.erase(activeSessions.begin() + index);

    if (this->currentActiveId == sessionId)
    {
        if (!activeSessions.empty())
        {
            this->currentActiveId = activeSessions[0]->getId();
            std::cout << "Active focus automatically shifted to session " << this->currentActiveId << "\n";
        }
        else
        {
            this->currentActiveId = -1;
        }
    }
    std::cout << "Session " << sessionId << " closed successfully.\n";
}

void SessionManager::makeNegative()
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("Cant make somehing negative if it isnt there");
    }

    int index = findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->makeNegative();
}

void SessionManager::makeGrayscale()
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("Cant make somehing grayscale if it isnt there");
    }

    int index = findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->makeGrayscale();
}

void SessionManager::makeMonochrome()
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("Cant make somehing monochrome if it isnt there");
    }

    int index = findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->makeMonochrome();
}

void SessionManager::printCurrentSessionInfo() const
{
    if (this->currentActiveId == -1)
    {
        std::cout << "No active sessions running in workspace engine.\n";
        return;
    }

    int index = findSessionIndex(this->currentActiveId);
    std::cout << "Files in session with ID " << this->currentActiveId << ":\n";
    this->activeSessions[index]->printSessionInfo();
}

void SessionManager::save()
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("Cannot save when there are no active sessions open");
    }
    
    int index = this->findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->save();
}

void SessionManager::saveAs(const std::string &originalFilename, const std::string &newFilename)
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("Cannot save as when there are no active sessions open");
    }
    
    int index = this->findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->saveAs(originalFilename, newFilename);
}

void SessionManager::paste(const std::string &srcPath, const std::string &destPath, int posX, int posY)
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("Cannot paste when there are no active sessions open");
    }
    
    int index = this->findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->paste(srcPath, destPath, posX, posY);
}

void SessionManager::undo()
{
    if (this->currentActiveId == -1)
    {
        
        throw std::invalid_argument("Cannot execute undo when there are no active sessions open.");
    }
    
    int index = findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->undo();
}

void SessionManager::redo()
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("Cannot execute redo when there are no active sessions open.");
    }
    
    int index = findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->redo();
}

void SessionManager::rotateLeft()
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("Cant rotate all the images to the left if there are NONE :D");
    }
    
    int index = this->findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->rotateLeft();
}

void SessionManager::rotateRight()
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("Cant rotate all the images that are in the active session if there are none!!");
    }
    
    int index = this->findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->rotateRight();
}

void SessionManager::flipTop()
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("Cannot flip the images horizontally in the active session if there are none");
    }
    
    int index = this->findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->flipTop();
}


void SessionManager::flipLeft()
{
    if (this->currentActiveId == -1)
    {
        throw std::invalid_argument("Cannot flip the images vertically in the active session if there are none");
    }
    
    int index = this->findSessionIndex(this->currentActiveId);
    this->activeSessions[index]->flipLeft();
}

bool SessionManager::hasAnyUnsavedWork() const
{
    size_t sessionCount = this->activeSessions.size();
    for (size_t i = 0; i < sessionCount; i++)
    {
        if (this->activeSessions[i]->hasUnsavedChanges() == true)
        {
            return true;
        }
    }
    
    return false;
}

void SessionManager::saveAllUnsaved()
{
    if (!hasAnyUnsavedWork())
    {
        return;
    }
    
    size_t sessionCount = this->activeSessions.size();
    for (size_t i = 0; i < sessionCount; i++)
    {
        if (this->activeSessions[i]->hasUnsavedChanges())
        {
            this->activeSessions[i]->save();
        }
    }
}

int SessionManager::findSessionIndex(int sessionId) const
{
    if (sessionId < 0)
    {
        throw std::invalid_argument("Cant have a sessionId that is lower then 0");
    }

    size_t count = this->activeSessions.size();
    for (size_t i = 0; i < count; i++)
    {
        if (this->activeSessions[i]->getId() == sessionId)
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

void SessionManager::free()
{
    size_t count = activeSessions.size();

    for (size_t i = 0; i < count; i++)
    {
        delete this->activeSessions[i];
    }
    activeSessions.clear();
}
