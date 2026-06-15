/**
 * @file Session.hpp
 * @brief Management container for a group of images loaded inside a single workspace session.
 */

#ifndef SESSION_HPP
#define SESSION_HPP

#include "../Image/Image.hpp"
#include "../Factory/ImageFactory.hpp"
#include <iostream>

class Session
{
public:
    /**
     * @brief Constructor initializing the session with a unique identification number.
     * @param id The auto-incremented session identification number.
     */
    Session(int sessionId);

    //disable copy semantic
    Session(const Session& other) = delete;
    Session& operator=(const Session& other) = delete;

    /**
     * @brief Destructor that deallocates all loaded images to prevent memory leaks.
     */
    ~Session();

    /**
     * @brief Appends a new image pointer into this session's collection.
     * @param img Polymorphic pointer to an allocated Image object.
     */
    void addImage(Image* img);

    /**
     * @brief Applies the negative transformation to all images in this session.
     */
    void makeNegative();

    /**
     * @brief Applies the grayscale transformation to all color images in this session.
     */
    void makeGrayscale();

    /**
     * @brief Applies the monochrome transformation to all images in this session.
     */
    void makeMonochrome();

    /**
     * @brief Outputs comprehensive details of all files within this session.
     */
    void printSessionInfo() const;

    // GETTERS
    int getId() const { return sessionId; }
    const std::vector<Image*>& getImages() const { return loadedImages; }

private:
    int sessionId;
    std::vector<Image*> loadedImages;
    size_t count;

private:
    void free();
};

#endif // SESSION_HPP