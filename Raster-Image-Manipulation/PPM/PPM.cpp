#include "PPM.hpp"

PPM::PPM(const std::string &filename)
    : Image(0, 0, filename), maxColorValue(MAX_VALUE)
{
    std::ifstream file(filename);
    if (!(file.is_open()))
    {
        throw std::invalid_argument("Cannot open the PPM file format");
    }
    
    std::string magicNumber;
    file >> magicNumber;
    if (magicNumber != "P3")
    {
        throw std::invalid_argument("Expected magic Number to be P2");
    }
    
    skipComments(file);
    if (!(file >> this->width >> this->height))
    {
        throw std::invalid_argument("Invalid reading of the dimensions");
    }
    
    if (this->width <= 0 || this->height <= 0)
    {
        throw std::invalid_argument("Cant have dimensions like these in P3");
    }
    
    skipComments(file);
    if (!(file >> maxColorValue) || maxColorValue <= 0)
    {
        throw std::invalid_argument("Cant read the maxColorValue in the PPM format");
    }
    

    pixelData.resize(this->width * this->height);
    int val;

    //we made a seperate variable consuming 4 more bytes, because through each itteration it has to do an arithmetic
    //if we do this.width * this.height its much more inneficient
    int count = this->width * this->height;
    for (int i = 0; i < count; i++)
    {
        u_int8_t colors[ITTERATIONS_TO_READ_PIXEL];

        for (int j = 0; j < ITTERATIONS_TO_READ_PIXEL; j++)
        {
            if (!(file >> val))
            {
                throw std::invalid_argument("Invalid pixel space");
            }
            else if (val < 0 || val > maxColorValue)
            {
                throw std::invalid_argument("Cant have pixels out of this range");
            }

            colors[j] = static_cast<u_int8_t>((val * MAX_VALUE) / maxColorValue);
        }

        this->pixelData[i] = {colors[0], colors[1], colors[2]};
    }
    
}

void PPM::save(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::invalid_argument("Cannot open the serializiation file in the PPM");
    }
    
    file << "P3\n" << this->width << ' ' << this->height << '\n' << MAX_VALUE << '\n';

    int count = this->width * this->height;
    for (int i = 0; i < count; i++)
    {
        file << static_cast<int>(pixelData[i].r) << ' ' 
             << static_cast<int>(pixelData[i].g) << ' ' 
             << static_cast<int>(pixelData[i].b);
        
        if ((i + 1) % this->width == 0)
        {
            file << '\n';
        }
        else
        {
            file << ' ';
        }
    }
}

PPM *PPM::clone() const
{
    return new PPM(*this);
}

void PPM::printSessionInfo() const
{
    std::cout << originalFilename << "\n portable pixmap format \n";
}

bool PPM::isGrayscale() const
{
    int count = width * height;
    for (int i = 0; i < count; i++)
    {
        if (pixelData[i].r != pixelData[i].g || pixelData[i].g != pixelData[i].b)
        {
            return false;
        }
    }
    return true;
}
