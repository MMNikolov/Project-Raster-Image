#include "CommandParser.hpp"

CommandParser::CommandParser()
    : session()
{}

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

        //stringsstream is a beautiful beautiful and beautiful library that allows
        //string to int conversion
        //int to string conversion
        //It lets us take a string and extract data from it (like from cin), It also lets us build strings by inserting data into it (like into cout). 
        std::stringstream ss(line);
        std::string command;
        if (!(ss >> command))
        {
            continue;
        }
        
        try
        {
            if (command == "open")
            {
                std::string filepath;
                if (!(ss >> filepath))
                {
                    throw std::invalid_argument("Missing parameter. Synthax open <filepath>");
                }
                this->session.openSession(filepath);
            }
            else if (command == "close")
            {
                this->session.closeSession();
            }
            else if (command == "save")
            {
                this->session.saveSession();
            }
            else if (command == "saveas")
            {
                std::string filepath;
                if (!(ss >> filepath))
                {
                    throw std::invalid_argument("Cant read the filepath where u want to save the file");
                }
                this->session.saveSessionAs(filepath);
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
        catch(const std::exception& e)
        {
            std::cerr << "Session aborted" << e.what() << '\n';
        }
    }
}

//sources used:
// - https://www.geeksforgeeks.org/cpp/stringstream-c-applications/