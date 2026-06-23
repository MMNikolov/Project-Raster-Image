/**
 * @file Session.hpp
 * @brief Management container for a group of images loaded inside a single workspace session.
 */

#ifndef SESSION_HPP
#define SESSION_HPP

#include "../Image/Image.hpp"
#include "../Factory/ImageFactory.hpp"
#include <iostream>
#include <ctime>

/**
 * @enum Operations
 * @brief Represents the supported geometric transformations for lazy evaluation.
 */
enum class Operations
{
    ROTATE_LEFT = 0,
    ROTATE_RIGHT = 1,
    FLIP_TOP = 2,
    FLIP_LEFT = 3
};

/**
 * @class Session
 * @brief Handles isolated image transformations and historical tracking for a single workspace window.
 */
class Session
{
public:
    /**
     * @brief Constructor initializing the session with a unique identification number.
     * @param sessionId The auto-incremented session identification number.
     * @throws std::invalid_argument If the provided sessionId is negative.
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
     * @throws std::invalid_argument If the provided image pointer is null.
     */
    void addImage(Image* img);

    /**
     * @brief Appends an image into the session by loading it from a disk path.
     * @param filename String containing the destination file path.
     * @throws std::invalid_argument If the file cannot be opened or parsed.
     */
    void addByFilename(const std::string& filename);

    /**
     * @brief Applies the negative transformation to all images in this session.
     * @throws std::invalid_argument If there are no images loaded in the session.
     */
    void makeNegative();

    /**
     * @brief Applies the grayscale transformation to all color images in this session.
     * @throws std::invalid_argument If there are no images loaded in the session.
     */
    void makeGrayscale();

    /**
     * @brief Applies the monochrome transformation to all images in this session.
     * @throws std::invalid_argument If there are no images loaded in the session.
     */
    void makeMonochrome();

    /**
     * @brief Outputs comprehensive details of all files within this session.
     */
    void printSessionInfo() const;

    /**
     * @brief Saves all loaded images in the session back to their original disk locations with a timestamp.
     * @throws std::invalid_argument If there are no images available to save.
     */
    void save();

    /**
     * @brief Saves the first image in the session to a completely new specified path.
     * @param newFilename The target output path.
     * @throws std::invalid_argument If the session is empty or the provided path is blank.
     */
    void saveAs(const std::string &originalFilename, const std::string &newFilename);

    /**
     * @brief Looks up the target destination file and applies the sub-image embedding logic.
     * @param srcPath Path to the source patch image.
     * @param destPath Path to the destination target image currently in the workspace.
     * @param posX The horizontal starting coordinate pixel offset.
     * @param posY The vertical starting coordinate pixel offset.
     * @throws std::invalid_argument If coordinates are invalid or images cannot be found.
     */
    void paste(const std::string& srcPath, const std::string& destPath, int posX, int posY);

    //UNDO AND REDO FUNCTIONALITIES

    /**
     * @brief Reverts the last transformation applied in this workspace session.
     */
    void undo();

    /**
     * @brief Re-applies the last undone transformation in this workspace session.
     */
    void redo();

    // ROTATING AND FLIPPING FUNCTIONALITIES

    /**
     * @brief Queues a request to rotate the image 90 degrees counter-clockwise.
     */
    void rotateLeft();

    /**
     * @brief Queues a request to rotate the image 90 degrees clockwise;
     */
    void rotateRight();

    /**
     * @brief Queues a request to flip the image on the horizontal axus
     */
    void flipTop();

    /**
     * @brief Queues a request to flip the image on the vertical axis
     */
    void flipLeft();

    // APPLIES THE ALREADY OPTIMIZED FUNCTION FOR THE ROTATE AND FLIP GIMICS

    /**
     * @brief Applies all optimized pending geometric transformations to the raw pixel matrices.
     */
    void bakeTransformations();

    // GETTERS

    /**
     * @brief Gets the unique session identification key.
     * @return Integer workspace identifier.
     */
    int getId() const { return sessionId; }

    /**
     * @brief Provides read-only access to the internal image collection tracking vector.
     * @return Const reference to the polymorphic image pointer vector.
     */
    const std::vector<Image*>& getImages() const { return loadedImages; }

private:
    int sessionId;                                  /**< Unique session identification number */
    std::vector<Image*> loadedImages;               /**< Collection of image resources in this workspace */
    size_t count;                                   /**< Helper tracker counting total active images */

    //UNDO AND REDO FUNCTIONALITIES 
    std::vector<std::vector<Image*>> undoHistory;   /**< History stack tracking deep copies for undo actions */
    std::vector<std::vector<Image*>> redoHistory;   /**< History stack tracking deep copies for redo actions */

    std::vector<Operations> pendingOps;             /**< Queue of deferred operations awaiting physical execution */
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

    /**
     * @brief Analyzes incoming commands in real-time and eliminates redundant consecutive operations.
     * @param newOp The incoming operation to be evaluated.
     */
    void applyOptimizedOp(Operations newOp); 
};

#endif // SESSION_HPP