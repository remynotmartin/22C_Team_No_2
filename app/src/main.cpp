// ***********************************
// CIS 22C Team 2 Project Driver File
// ***********************************
// Prepared by: Remy Dinh

#include <iostream>
#include <fstream>  // We are reading from a file after all.

void splashScreen();
char mainMenu();

int main()
{
    splashScreen();
    char userChoice = '';
    while (userChoice != 'X' && userChoice != 'x')
    {
        userChoice = mainMenu();
        // Then execute the respective selected option below.
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
    std::cout << "######################################################" << std::endl;
    std::cout << "#                                                    #" << std::endl;
    std::cout << "#        ====                                        #" << std::endl;
    std::cout << "#        |                                           #" << std::endl;
    std::cout << "#        |==  |  | r== === === === === | |           #" << std::endl;
    std::cout << "#        |    |  | |   | | | | |=  |=| |N|           #" << std::endl;
    std::cout << "#        ===+ ==== |   === === === | | | |           #" << std::endl;
    std::cout << "#                          |                         #" << std::endl;
    std::cout << "#                          |                         #" << std::endl;
    std::cout << "#        ===                                         #" << std::endl;
    std::cout << "#        |   === | | | | === t=c | |                 #" << std::endl;
    std::cout << "#        |   | | | | |N|  |  |    |                  #" << std::endl;
    std::cout << "#        === === === | |  |  |    |                  #" << std::endl;
    std::cout << "#                                                    #" << std::endl;
    std::cout << "#                                                    #" << std::endl;
    std::cout << "#         D=                                         #" << std::endl;
    std::cout << "#         | |  A  ===  A  |=)  A  (^/ ===            #" << std::endl;
    std::cout << "#         | | |_|  |  |_| |<  |-|  S  |-             #" << std::endl;
    std::cout << "#         D=  | |  |  | | |=) | | /-) ===            #" << std::endl;
    std::cout << "#                                                    #" << std::endl;
    std::cout << "######################################################" << std::endl;
    std::cout << std::endl;
}

void mainMenu()
{
    std::cout << "----------" << std::endl;
    std::cout << "Main Menu:" << std::endl;
    std::cout << "----------" << std::endl;

    std::cout << "A. Add new data"               << std::endl;
    std::cout << "B. Delete data"                << std::endl; // Need to figure out how this will work
    std::cout << "C. Search by country name"     << std::endl; // Primary key, handled by Hash Table
    std::cout << "D. Search by language"         << std::endl; // Secondary key, display all matches (BST)
    std::cout << "E. List countries by language" << std::endl; // Print inOrder traversal of the BST
    std::cout << "F. Write data to file"         << std::endl; // File I/O baby.
    std::cout << "G. Hash Table Statistics"      << std::endl; // Print # of collisions, longest collision list, load factor, etc.
    std::cout << "H. Display Menu"               << std::endl; // Show this menu once after start, then only upon request.
    std::cout << "X. Exit"                       << std::endl;
    
    // Hidden menu options to implement
    // 1. Display indented tree
    // 2. Display names of team members 
    //
    // Additional functionality we need to implement since we're a team of four
    //
    //
}
