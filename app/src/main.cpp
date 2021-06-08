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

void splashScreen();
void mainMenu();

// These paths assume program is run from project root.
const std::string  inputFile = "./data/primaryData.csv";
const std::string outputFile = "./data/database.csv";

int main()
{
    splashScreen();
    int userChoice = -1; 
    mainMenu();
    while (userChoice != 0)
    {
        std::cout << "[Type '8' to display menu options again.]" << std::endl;
        std::cout << "Please select an option from the menu: ";
        std::cin >> userChoice;
        // Then execute the respective selected option below.
        switch (userChoice)
        {
            case 8:
                mainMenu();
                break;
            case 0:
                std::cout << "Terminating program..." << std::endl;
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
    std::cout << "2. Delete data"                << std::endl; // Need to figure out how this will work
    std::cout << "3. Search by country name"     << std::endl; // Primary key, uses Hash Table
    std::cout << "4. Search by language"         << std::endl; // Secondary key, uses (BST)
    std::cout << "5. List countries by language" << std::endl; // Print inOrder traversal of the BST
    std::cout << "6. Write data to file"         << std::endl; // File I/O baby.
    std::cout << "7. Hash Table Statistics"      << std::endl; // Print # of collisions, longest collision list, load factor, etc.
    std::cout << "8. Display Menu"               << std::endl; // Show this menu once after start, then only upon request.
    std::cout << "0. Exit"                       << std::endl << std::endl;

    
    // Hidden menu options to implement
    // 1. Display indented tree
    // 2. Display names of team members 
    //
    // Additional functionality we need to implement since we're a team of four.
    //
    // Undo deletion of records using a stack, which gets cleared whenever the database is written to an output file.
}
