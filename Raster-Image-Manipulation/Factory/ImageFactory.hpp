#ifndef IMAGE_FACTORY_HPP
#define IMAGE_FACTORY_HPP

#include "../Image/Image.hpp"
#include "../PGM/PGM.hpp"
#include "../PBM/PBM.hpp"
#include "../PPM/PPM.hpp"
#include "../BMP/BMP.hpp"

class ImageFactory
{
public:
    static Image* createImage(const std::string& filename);
};

#endif // IMAGE_FACTORY_HPP