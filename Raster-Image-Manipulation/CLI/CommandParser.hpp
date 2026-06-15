/**
 * @file CommandParser.hpp
 * @brief Defines the interactive command-line interface parser for the multi-session image engine.
 */

#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include "../Session/SessionManager.hpp"
#include <sstream>

/**
 * @brief Handles user console input line tokenization and drives the multi-session state machine.
 */
class CommandParser
{
public: 
    /**
     * @brief Constructor initializing the command engine interface.
     */
    CommandParser();

    /**
     * @brief Launches the main text processing console loop.
     */
    void run();

private:
    SessionManager manager; /**< The global session registry coordinating active tabs */
};

#endif // COMMAND_PARSER_HPP