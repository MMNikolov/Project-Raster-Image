/**
 * @file SessionManager.hpp
 * @brief Coordinates multiple concurrent image processing workspaces and routes user commands.
 */

#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include "Session.hpp"

/**
 * @class SessionManager
 * @brief Global state registry controller routing terminal data pipelines to focused workspace tabs.
 */
class SessionManager
{
public:
    /**
     * @brief Constructor initializing the session registry with an inactive default state.
     */
    SessionManager();

    /**
     * @brief Destructor guaranteeing deallocation of all active sessions and their images.
     */
    ~SessionManager();

    // Disable copy semantics to protect ownership of deep session pointer buffers
    SessionManager(const SessionManager &other) = delete;
    SessionManager &operator=(const SessionManager &other) = delete;

    /**
     * @brief Spawns a new Session container, loads all target file paths, and switches focus to it.
     * @param filepaths List of strings representing paths to the target images.
     */
    void loadSession(const std::vector<std::string> &filepaths);

    /**
     * @brief Shifts active focus to a different loaded session workspace by its unique ID.
     * @param sessionId The targeted identifier integer.
     */
    void switchSession(int sessionId);

    /**
     * @brief Closes a targeted session workspace and releases all its embedded allocations.
     * @param sessionId The targeted identifier integer to terminate.
     */
    void closeSessionById(int sessionId);

    /**
     * @brief Forwards negative pixel calculation execution to the focused active session.
     */
    void makeNegative();

    /**
     * @brief Forwards grayscale pixel calculation execution to the focused active session.
     */
    void makeGrayscale();

    /**
     * @brief Forwards monochrome pixel calculation execution to the focused active session.
     */
    void makeMonochrome();

    /**
     * @brief Prints format profile statistics for files inside the currently active session focus.
     */
    void printCurrentSessionInfo() const;

    /**
     * @brief Directs the focused active session to save all its images back to disk.
     */
    void save();

    /**
     * @brief Directs the focused active session to save an image to a new path.
     * @param newFilename Target path string.
     */
    void saveAs(const std::string& newFilename);

    /**
     * @brief Looks up the target destination file and applies the sub-image embedding logic.
     */
    void paste(const std::string& srcPath, const std::string& destPath, int posX, int posY);

    //REDO AND UNDO FUNCTIONALITIES

    /**
     * @brief Requests an undo reversion operation on the active focused workspace tracking block.
     */
    void undo();

    /**
     * @brief Requests a redo recovery operation on the active focused workspace tracking block.
     */
    void redo();

    //ROTATE AND FLIP FUNCTIONALITIES

    /**
     * @brief Forwards a counter-clockwise rotation request to the active session.
     */
    void rotateLeft();

    /**
     * @brief Forwards a clockwise rotation request to the active session.
     */
    void rotateRight();

    /**
     * @brief Forwards a vertical axis mirror flip request to the active session.
     */
    void flipTop();

    /**
     * @brief Forwards a horizontal axis mirror flip request to the active session.
     */
    void flipLeft();

private:
    std::vector<Session *> activeSessions; /**< Collection of all concurrent workspace instances */
    int currentActiveId;                   /**< ID of the session currently focused (-1 if none exist) */
    int idCounter;                         /**< Monotonically increasing ID ticker generator */

private:
    /**
     * @brief Helper to locate the array index position slot matching a given numeric workspace tracker key.
     * @param sessionId The identification key sought.
     * @return Array index offset value position if located, or -1 if the ID does not match an open tab.
     */
    int findSessionIndex(int sessionId) const;

    /**
     * @brief System dump call purging all allocations tracked in the registry stack.
     */
    void free();
};

#endif // SESSION_MANAGER_HPP