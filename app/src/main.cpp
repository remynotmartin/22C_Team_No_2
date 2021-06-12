// ***********************************
// CIS 22C Team 2 Project Driver File
// ***********************************
// Prepared by: Remy Dinh (Team Lead)
// Fellow team members:
//   - Shun "Shun-san" Furuya
//   - Mahik Kaur
//   - Taeyoon "Yoon" Kim

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include "LinkedList.h"
#include "Country.h"
#include "Stack.h"      // For the undo-stack
#include "HashTable.h"

void splashScreen ();
void mainMenu     ();
bool fileRead     (LinkedList<Country> &coreDataList);
void teamNames    ();
void removeData   (LinkedList<Country> &coreDataList, Stack<Country> &undoStack);

// These paths assume program is run from project root.
const std::string  inputFilename = "./data/primaryData.csv";
const std::string outputFilename = "./data/database.csv";

int main()
{
    splashScreen();
    LinkedList<Country> coreDataList;
    Stack<Country>      undoStack;

    // Read primary input data
    if (!fileRead(coreDataList))
        return 1;

    int userChoice = -1; 
    mainMenu();
    while (userChoice != 0)
    {
        std::cout << "[Type '9' to display menu options again.]" << std::endl;
        std::cout << "Please select an option from the menu: ";
        std::cin >> userChoice;
        // Then execute the respective selected option below.
        switch (userChoice)
        {
            case 1: // Add new Data
                //addData(coreDataList); // Function not ready yet
            case 2: // Delete Data
                removeData(coreDataList, undoStack);
                break;
            case 9: // Display Main Menu
                mainMenu();
                break;
            case 0:
                std::cout << "Terminating program..." << std::endl;
                break;
            case 42: // Display group member names (Hidden option)
                teamNames();
                break;
            case 99: // Testing: Display coreDataList
                coreDataList.displayList();
                break;
            default:
                std::cout << "Invalid input detected. Please try again." << std::endl;
        }
    }
    return 0;
}

void splashScreen()
{
    std::cout << std::endl;
    std::cout << "######################################################" << std::endl;
    std::cout << "#   CIS 22C Data Structures Team Project: Group 2    #" << std::endl;
    std::cout << "######################################################" << std::endl;
    std::cout << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << "                    Group Members                     " << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << "    Remy Dinh   : Project Coordination & Integration  " << std::endl;
    std::cout << "    Shun Furuya : Hash List Algorithms                " << std::endl;
    std::cout << "    Mahik Kaur  : Screen Output                       " << std::endl;
    std::cout << "    Taeyoon Kim : BST Algorithms                      " << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "######################################################" << std::endl;
    std::cout << "#                                                    #" << std::endl;
    std::cout << "#             European Country Database              #" << std::endl;
    std::cout << "#                                                    #" << std::endl;
    std::cout << "######################################################" << std::endl;
    std::cout << std::endl;
}

void mainMenu()
{
    std::cout << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << " Main Menu " << std::endl;
    std::cout << "-----------" << std::endl;

    std::cout << "1. Add new data"               << std::endl;
    std::cout << "2. Delete data"                << std::endl;
    std::cout << "3. Search by country name"     << std::endl; // Primary key, uses Hash Table
    std::cout << "4. Search by language"         << std::endl; // Secondary key, uses (BST)
    std::cout << "5. List countries by language" << std::endl; // Print inOrder traversal of the BST
    std::cout << "6. Write data to file"         << std::endl; // More File I/O baby.
    std::cout << "7. Hash Table Statistics"      << std::endl; // Print # of collisions, longest collision list, load factor, etc.
    std::cout << "9. Display Menu"               << std::endl; // Show this menu once after start, then only upon request.

    std::cout << "99. Print coreDataList (testing)" << std::endl;
    std::cout << "0. Exit"                       << std::endl << std::endl;
}

// Little easter egg as per project requirements.
// Menu Option: 42
void teamNames()
{
    std::cout << "##########################################" << std::endl;
    std::cout << "#                 Team 2                 #" << std::endl;
    std::cout << "##########################################" << std::endl;
    std::cout << "  Remy Dinh - Coordination and Integration" << std::endl;
    std::cout << "Shun Furuya - Hash List Algorithms"         << std::endl;
    std::cout << " Mahik Kaur - Screen Output"                << std::endl;
    std::cout << "Taeyoon Kim - BST Algorithms"               << std::endl;
    std::cout << "##########################################" << std::endl;
}

// Reads in data from the primaryData.csv input file.
// File to be read is currently hard-coded, but could relatively easily
// be changed to accept a filename from a user.
//
bool fileRead(LinkedList<Country> &coreDataList)
{
    std::ifstream inputFile(inputFilename);
    if (!inputFile) // Failure to open standard input file
    {
        std::cerr << "Input file " << inputFilename << " could not be read." << std::endl;
        std::cerr << "Please check for path and filename. Program will now terminate." << std::endl;
        return false;
    }

    while (!inputFile.eof())
    {
        std::string lineBuffer;
        std::getline(inputFile, lineBuffer);
        if (inputFile.eof()) break;

        // INPUT FILE RECORD FORMAT:
        //
        // countryName,language,population,majorReligion,GDP,surfaceArea,capitalCity
        //
        // (total of 6 delimiting commas)
        
        // Grab all delimiter positions
        size_t comma_1 = lineBuffer.find(',', 0),
               comma_2 = lineBuffer.find(',', comma_1 + 1),
               comma_3 = lineBuffer.find(',', comma_2 + 1),
               comma_4 = lineBuffer.find(',', comma_3 + 1),
               comma_5 = lineBuffer.find(',', comma_4 + 1),
               comma_6 = lineBuffer.find(',', comma_5 + 1);

        std::string name       = lineBuffer.substr(0, comma_1),
                    language   = lineBuffer.substr(comma_1  + 1, comma_2 - comma_1 - 1),
                    religion   = lineBuffer.substr(comma_3  + 1, comma_4 - comma_3 - 1),
                    capital    = lineBuffer.substr(comma_6  + 1);
        unsigned    population = std::stoul(lineBuffer.substr(comma_2 + 1, comma_3 - comma_2 - 1));
        double      GDP        = std::stod(lineBuffer.substr(comma_4 + 1, comma_5 - comma_4 - 1)),
                    area       = std::stod(lineBuffer.substr(comma_5 + 1, comma_6 - comma_5 - 1));

        Country temp(name, language, population, religion, GDP, area, capital);
        
        // Insert into coreDataList
        if (!coreDataList.insertNode(temp))
        {
            std::cerr << "Duplicate primary key found in file " << inputFilename << "!" << std::endl;
            std::cerr << "Duplicate record "<< temp.getName() << " has been rejected, but file read will continue." << std::endl;
        }
    }
    inputFile.close();

    return true;
}

// Removes item from coreDataList and places it onto the undoStack.
// Will also call BST and HashTable's respective functions to break the links
// to their respective indirect nodes.
//
// Menu OPCODE: 2
//
void removeData (LinkedList<Country> &coreDataList, Stack<Country> &undoStack)
{
    std::string query("");
    std::cout << "Please input the name of the country you want to remove: ";
    std::cin.ignore();
    std::getline(std::cin, query);
    //
    ListNode<Country> *holder = coreDataList.removeItem(query);
    if (!holder) // Holder == nullptr;
    {
        std::cout << query << " was not found in the database." << std::endl;
        return;
    }

    undoStack.push(holder); // Push removed item onto stack
    std::cout <<  query << " was removed from the database." << std::endl;
}





