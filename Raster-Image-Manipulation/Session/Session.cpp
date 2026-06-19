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

    this->bakeTransformations();

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

    this->bakeTransformations();

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

    this->bakeTransformations();

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

void Session::save()
{
    if (this->count <= 0)
    {
        throw std::invalid_argument("There is nothing to save my guy...");
    }
    
    this->bakeTransformations();

    for (size_t i = 0; i < this->count; i++)
    {
        this->loadedImages[i]->save(this->loadedImages[i]->getFilename());
        std::cout << "Sucessfully save the image: " << this->loadedImages[i]->getFilename() << '\n';
    }
    
}

void Session::saveAs(const std::string &newFilename)
{
    if (this->count <= 0)
    {
        throw std::invalid_argument("there is nothing to save as my guy... :D");
    }
    
    if (newFilename.empty())
    {
        throw std::invalid_argument("Cant save into the nothingness");
    }
    
    this->bakeTransformations();

    this->loadedImages[0]->save(newFilename);
    std::cout << "Successfully saved state as: " << newFilename << "\n";
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

    this->pendingOps.clear();
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

    this->pendingOps.clear();
}

void Session::rotateLeft() { applyOptimizedOp(Operations::ROTATE_LEFT); }
void Session::rotateRight() { applyOptimizedOp(Operations::ROTATE_RIGHT); }
void Session::flipTop() { applyOptimizedOp(Operations::FLIP_TOP); }
void Session::flipLeft() { applyOptimizedOp(Operations::FLIP_LEFT); }

void Session::bakeTransformations()
{
    if (this->pendingOps.empty())
    {
        return;
    }

    int prendingOpsSize = this->pendingOps.size();
    for (size_t i = 0; i < count; i++)
    {
        for (size_t j = 0; j < prendingOpsSize; j++)
        {
            switch (pendingOps[j])
            {
                case (Operations)0:
                    this->loadedImages[i]->rotateLeft();
                    break;
                case (Operations)1:
                    this->loadedImages[i]->rotateRight();
                    break;
                case (Operations)2:
                    this->loadedImages[i]->flipTop();
                    break;
                case (Operations)3:
                    this->loadedImages[i]->flipLeft();
                    break;
                default:
                    throw std::invalid_argument("Dont have such an operation, there is an error somewhere");
                    break;
            }
        }
    }

    pendingOps.clear();
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
    std::vector<Image *> newImages;
    // this function allocates (reserves) memory
    // allocates exactly the amount of memory we need for the new Image
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

void Session::applyOptimizedOp(Operations newOp)
{
    if (this->pendingOps.empty())
    {
        this->pendingOps.push_back(newOp);
        return;
    }

    Operations lastOp = this->pendingOps.back();

    if ((newOp == Operations::ROTATE_LEFT && lastOp == Operations::ROTATE_RIGHT) ||
        (newOp == Operations::ROTATE_RIGHT && lastOp == Operations::ROTATE_LEFT) ||
        (newOp == Operations::FLIP_TOP && lastOp == Operations::FLIP_TOP) ||
        (newOp == Operations::FLIP_LEFT && lastOp == Operations::FLIP_LEFT))
    {
        this->pendingOps.pop_back();
        std::cout << "Optimization: Redundant consecutive operations cancelled out.\n";
        return;
    }

    this->pendingOps.push_back(newOp);

    if (this->pendingOps.size() >= 4)
    {
        size_t n = this->pendingOps.size();

        if (this->pendingOps[n - 1] == newOp && this->pendingOps[n - 2] == newOp &&
            this->pendingOps[n - 3] == newOp && this->pendingOps[n - 4] == newOp)
        {
            if (newOp == Operations::ROTATE_LEFT || newOp == Operations::ROTATE_RIGHT)
            {
                this->pendingOps.pop_back();
                this->pendingOps.pop_back();
                this->pendingOps.pop_back();
                this->pendingOps.pop_back();
                std::cout << "Optimization: 4 consecutive rotations skipped (Identity State).\n";
            }
        }
    }
}
