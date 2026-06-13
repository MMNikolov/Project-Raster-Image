/**
 * @file SessionManager.hpp
 * @brief Handles active engine state processing and image lifecycle ownership.
 */

#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include "../Image/Image.hpp"
#include "../Factory/ImageFactory.hpp"

class SessionManager
{
public:
    /**
     * @brief Constructor initializing default inactive states.
     */
    SessionManager();

    SessionManager(const SessionManager& other) = delete;
    SessionManager& operator=(const SessionManager& other) = delete;

    /**
     * @brief Destructor guaranteeing memory cleanup.
     */
    ~SessionManager();

    //METHODS

    /**
     * @brief Requests an image from the factory and takes ownership of its allocation.
     * @param filepath The source path of the target image file.
     */
    void openSession(const std::string& filepath);

    /**
     * @brief Safely deletes the active image resource and resets the tracking state.
     */
    void closeSession();

    /**
     * @brief Serializes the active image back to its original file path.
     */
    void saveSession() const;

    /**
     * @brief Serializes the active image to a new location and updates the tracking file path.
     * @param filepath The new destination path on disk.
     */
    void saveSessionAs(const std::string& filepath);

    // GETTERS
    bool isActive() const { return isSessionActive; }
    const std::string& getFilename() const { return currentFilename; }

private:
    Image* currentImage;          /**< Polymorphic pointer to the active image resource */
    std::string currentFilename;  /**< Tracks the current file path on disk */
    bool isSessionActive;         /**< State flag indicating if a session is currently running */

private:
    /**
     * @brief Internal clean-up routine to delete loaded image resources safely.
     */
    void free();
};

#endif // SESSION_MANAGER_HPP