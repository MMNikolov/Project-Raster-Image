#include "PGM.hpp"

PGM::PGM(const std::string &filename)
    : Image(0, 0, filename), scalableValue(MAX_VALUE)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::invalid_argument("Cant open the file in PGM");
    }
    
    std::string magicNumber;
    file >> magicNumber;
    if (magicNumber != "P2")
    {
        throw std::invalid_argument("Not a valid portable greymap");
    }
    
    skipComments(file);
    if (!(file >> width >> height))
    {
        throw std::invalid_argument("Cannot read the dimensions of the PGM");
    }
    
    if (this->width <= 0 || this->height <= 0)
    {
        throw std::invalid_argument("The dimensions cannot be below zero in PGM");
    }
    
    skipComments(file);
    if (!(file >> scalableValue) || scalableValue <= 0 || scalableValue > 255)
    {
        throw std::invalid_argument("Cant read the grey factor in PGM");
    }
    
    pixelData.resize(this->width * this->height);

    int val;

    //we made a seperate variable consuming 4 more bytes, because through each itteration it has to do an arithmetic
    //if we do this.width * this.height its much more inneficient
    int count = this->width * this->height;
    for (int i = 0; i < count; i++)
    {
        if (!(file >> val))
        {
            throw std::invalid_argument("Invalid pixel space");
        }
        else if (val < 0 || val > scalableValue)
        {
            throw std::invalid_argument("Cant have pixels out of this range");
        }
        
        uint8_t normalisedValue = static_cast<uint8_t>((val * MAX_VALUE) / scalableValue);

        pixelData[i] = { normalisedValue, normalisedValue, normalisedValue};
    }
}

void PGM::save(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::invalid_argument("Cant open the PGM file");
    }
    
    file << "P2\n" << this->width << ' ' << this->height << '\n' << MAX_VALUE << '\n';

    for (int i = 0; i < this->height * this->width; i++)
    {
        //We get a random value, because everything in the pxielData[element] is the same so we get red for example
        //channel inetensity [0-255]
        file << static_cast<int>(pixelData[i].r);

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

PGM *PGM::clone() const
{
    return new PGM(*this);
}

void PGM::printSessionInfo() const
{
    std::cout << originalFilename << " portable greymap format \n";
}

bool PGM::isGrayscale() const
{
    return true;
}

void PGM::makeNegative()
{
    int size = this->width * this->height;
    for (int i = 0; i < size; i++)
    {
        this->pixelData[i].b = (MAX_VALUE - this->pixelData[i].b);
        this->pixelData[i].r = (MAX_VALUE - this->pixelData[i].r);
        this->pixelData[i].g = (MAX_VALUE - this->pixelData[i].g);
    }
    
}

//does nothing
//--------------------------
void PGM::makeGrayscale()
{
    return;
}
//--------------------------

void PGM::makeMonochrome()
{
    int size = this->width * this->height;
    for (int i = 0; i < size; i++)
    {
        //we can take one of the cholors because everysingle one is the same color intensity
        if (this->pixelData[i].b > 127)
        {
            this->pixelData[i] = {MAX_VALUE, MAX_VALUE, MAX_VALUE};
            continue;
        }
        
        this->pixelData[i] = {0, 0, 0};
    }
}