#include "CommandParser.hpp"

CommandParser::CommandParser()
    : manager()
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
                while (ss >> path)
                {
                    filepaths.push_back(path);
                }
                
                if (filepaths.empty())
                {
                    throw std::invalid_argument("Cannot load load the session if there are no filepaths");
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
                std::string dummy;
                if (!(ss >> dummy))
                {
                    throw std::invalid_argument("cant the read the rest of the command");
                }
                this->manager.closeSessionById(-1);
            }
            else if (command == "negative")
            {
                this->manager.makeNegative();
                std::cout << "Negative filter transaction logged.\n";
            }
            else if (command == "monochrome")
            {
                this->manager.makeMonochrome();
                std::cout << "Grayscale filter transaction logged.\n";
            }
            else if (command == "grayscale")
            {
                this->manager.makeGrayscale();
                std::cout << "Monochrome filter transaction logged.\n";
            }
            else if (command == "rotate")
            {
                std::string orientation;
                if (!(ss >> orientation))
                {
                    throw std::invalid_argument("Cant read where the images are needed to rotate");
                }
                
                if (orientation == "left")
                {
                    this->manager.rotateLeft();
                }
                else if (orientation == "right")
                {
                    this->manager.rotateRight();
                }
                else
                {
                    throw std::invalid_argument("Cant rotate if the orientation isnt correct");
                }
            }
            else if (command == "flip")
            {
                std::string orientation;
                if (!(ss >> orientation))
                {
                    throw std::invalid_argument("Cant read where the images are needed to flip");
                }
                
                if (orientation == "top")
                {
                    this->manager.flipTop();
                }
                else if (orientation == "left")
                {
                    this->manager.flipLeft();
                }
                else
                {
                    throw std::invalid_argument("Cant rotate if the orientation isnt correct");
                }
            }
            else if (command == "undo")
            {
                this->manager.undo();
            }
            else if (command == "redo")
            {
                this->manager.redo();
            }
            else if (command == "save")
            {
                this->manager.save();
            }
            else if (command == "saveas")
            {
                std::string subCommand;
                if (!(ss >> subCommand))
                {
                    throw std::invalid_argument("You need to tell me where to save the file if u choose the save as functionality");
                }
                this->manager.saveAs(subCommand);
            }
            else if (command == "paste")
            {
                std::string sourcePath, destinationPath;
                int posX, posY;
                if (!(ss >> sourcePath >> destinationPath >> posX >> posY))
                {
                    throw std::invalid_argument("couldnt read what was the source and what was the destionation path");
                }
                
                this->manager.paste(sourcePath, destinationPath, posX, posY);
                std::cout << "Sub-image embedded successfuly!\n";
            }
            else if (command == "list" || command == "session")
            {
                std::string subCommand;
                if (!(ss >> subCommand))
                {
                    throw std::invalid_argument("Cant read the subcommand");
                }
                this->manager.printCurrentSessionInfo();
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