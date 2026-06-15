#include "CommandParser.hpp"

CommandParser::CommandParser()
    : manager()
{
}

void CommandParser::run()
{
    std::string line;
    std::cout << "CLI workspace active";

    while (true)
    {
        std::cout << "Image-engine> ";
        if (!std::getline(std::cin, line))
        {
            break;
        }

        // stringsstream is a beautiful beautiful and beautiful library that allows
        // string to int conversion
        // int to string conversion
        // It lets us take a string and extract data from it (like from cin), It also lets us build strings by inserting data into it (like into cout).
        std::stringstream ss(line);
        std::string command;
        if (!(ss >> command))
        {
            continue;
        }

        try
        {
            if (command == "load")
            {
                std::vector<std::string> filepaths;
                std::string path;
                if (ss >> path)
                {
                    filepaths.push_back(path);
                }

                if (filepaths.empty())
                {
                    throw std::invalid_argument("Missing parameters. Syntax: load <file1> <file2> ...");
                }
                this->manager.loadSession(filepaths);
            }
            else if (command == "switch")
            {
                int targetId;
                if (!(ss >> targetId))
                {
                    throw std::invalid_argument("Missing the targetId parameter");
                }

                this->manager.switchSession(targetId);
            }
            else if (command == "close")
            {
                int targetId;
                if (!(ss >> targetId))
                {
                    throw std::invalid_argument("Missing the targetId parameter");
                }

                this->manager.closeSessionById(targetId);
            }
            else if (command == "negative")
            {
                this->manager.makeNegative();
            }
            else if (command == "monochrome")
            {
                this->manager.makeMonochrome();
            }
            else if (command == "grayscale")
            {
                this->manager.makeGrayscale();
            }
            else if (command == "list")
            {
                std::string subCommand;
                if (ss >> subCommand && subCommand == "session")
                {
                    this->manager.printCurrentSessionInfo();
                }
                else
                {
                    std::cout << "Unknown command configuration. Did you mean 'list session'?\n";
                }
            }
            else if (command == "session")
            {
                std::string subCommand;
                if (ss >> subCommand && subCommand == "info")
                {
                    this->manager.printCurrentSessionInfo();
                }
                else
                {
                    std::cout << "Unknown command configuration. Did you mean 'session info'?\n";
                }
            }
            else if (command == "exit")
            {
                std::cout << "leaving the session? Goodbye :D";
                break;
            }
            else
            {
                std::cout << "Unknown operational command token. Choose from: open, close, save, saveas, exit\n";
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Session aborted" << e.what() << '\n';
        }
    }
}

// sources used:
//  - https://www.geeksforgeeks.org/cpp/stringstream-c-applications/