/**
 * @file CommandParser.hpp
 * @brief Defines the interactive command-line interface parser for the image engine.
 */

#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include "../Session/SessionManager.hpp"
#include <sstream>

/**
 * @brief Handles user console input, tokenizes input text, and drives the session state machine.
 */
class CommandParser
{
public: 
    /**
     * @brief Constructor initializing the command engine subsystem.
     */
    CommandParser();

    /**
     * @brief Launches the infinite terminal line processing loop.
     */
    void run();

private:
    SessionManager session; /**< The central state manager holding active image resources */
};

#endif // COMMAND_PARSER_HPP