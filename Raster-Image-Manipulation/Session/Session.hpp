/**
 * @file Session.hpp
 * @brief Management container for a group of images loaded inside a single workspace session.
 */

#ifndef SESSION_HPP
#define SESSION_HPP

#include "../Image/Image.hpp"
#include "../Factory/ImageFactory.hpp"
#include <iostream>

/**
 * @class Session
 * @brief Handles isolated image transformations and historical tracking for a single workspace window.
 */
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

    //UNDO AND REDO FUNCTIONALITIES

    /**
     * @brief Reverts the last transformation applied in this workspace session.
     */
    void undo();

    /**
     * @brief Re-applies the last undone transformation in this workspace session.
     */
    void redo();

    // GETTERS
    int getId() const { return sessionId; }

    /**
     * @brief Provides read-only access to the internal image collection tracking vector.
     * @return Const reference to the polymorphic image pointer vector.
     */
    const std::vector<Image*>& getImages() const { return loadedImages; }

private:
    int sessionId;                      /**< Unique session identification number */
    std::vector<Image*> loadedImages;   /**< Collection of image resources in this workspace */
    size_t count;                       /**< Helper tracker counting total active images */

    //UNDO AND REDO FUNCTIONALITIES 
    std::vector<std::vector<Image*>> undoHistory;   /**< History stack tracking deep copies for undo actions */
    std::vector<std::vector<Image*>> redoHistory;   /**< History stack tracking deep copies for redo actions */

private:
    /**
     * @brief Fully deallocates all embedded image memory and clears out state vector tracking grids.
     */
    void free();

    /**
     * @brief Generates a deep clone vector list copy of the current image storage buffer array.
     * @return New independent vector holding deep allocated copies of the image pool.
     */
    std::vector<Image*> cloneCurrentState() const;

    /**
     * @brief Safely clears and drops polymorphic heap structures packed inside an image array.
     * @param images Targeted tracking array to scrub and reset.
     */
    void clearImageVector(std::vector<Image*>& images);
};

#endif // SESSION_HPP