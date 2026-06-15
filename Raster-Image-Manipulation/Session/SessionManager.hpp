/**
 * @file SessionManager.hpp
 * @brief Coordinates multiple concurrent image processing workspaces and routes user commands.
 */

#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include "Session.hpp"

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
    SessionManager(const SessionManager& other) = delete;
    SessionManager& operator=(const SessionManager& other) = delete;

    /**
     * @brief Spawns a new Session container, loads all target file paths, and switches focus to it.
     * @param filepaths List of strings representing paths to the target images.
     */
    void loadSession(const std::vector<std::string>& filepaths);

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

    // Command Routing Proxies (Forwards execution straight to the currently focused active session)
    void makeNegative();
    void makeGrayscale();
    void makeMonochrome();
    void printCurrentSessionInfo() const;

private:
    std::vector<Session*> activeSessions; /**< Collection of all concurrent workspace instances */
    int currentActiveId;                 /**< ID of the session currently focused (-1 if none exist) */
    int idCounter;                       /**< Monotonically increasing ID ticker generator */

private:
    /**
     * @brief Helper to locate the index of a session within the vector tracking array.
     * @return The index slot if found, or -1 if the ID does not match any open session.
     */
    int findSessionIndex(int sessionId) const;

    void free();
};

#endif // SESSION_MANAGER_HPP