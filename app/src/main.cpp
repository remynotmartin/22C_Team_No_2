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
#include <limits>

#include "LinkedList.h"
#include "Country.h"
#include "Stack.h"      // For the undoStack
#include "HashTable.h"  // Includes HashNode.h
#include "BinarySearchTree.h" // Includes BinaryNode.h

void splashScreen ();
void mainMenu     ();
void teamNames    ();
bool fileRead     (LinkedList<Country>&, HashTable<Country>&, BinarySearchTree<Country>&);
void removeData   (LinkedList<Country>&, HashTable<Country>&, BinarySearchTree<Country>&, Stack<Country>&);
void addData      (LinkedList<Country>&, HashTable<Country>&, BinarySearchTree<Country>&);
void searchName   (HashTable<Country>&);
void searchLang   (BinarySearchTree<Country>&);
void hashStats    (HashTable<Country>&);
void undoRemove   (Stack<Country> &undoStack, LinkedList<Country> &coreDataList, HashTable<Country>&, BinarySearchTree<Country>&);
void writeToFile  (HashTable<Country> &nameTable, Stack<Country> &undoStack);
void endProgram   (HashTable<Country> &nameTable);
void levelIndent  (const unsigned level);

// These paths assume program is run from project root.
const std::string  stdInputFile = "./data/primaryData.csv";
const std::string databaseFile = "./data/database.csv";

// Function to be passed into the BST functions so that they make proper comparisons.
unsigned compareLang          (const Country&, const Country&);
unsigned compareLangQuery     (const Country&, const std::string &);
void     visitBST_Node        (const BinaryNode<Country>*);
void     visitBST_Node_indent (const BinaryNode<Country>*, const unsigned level);

int main()
{
    splashScreen();

    LinkedList<Country>       coreDataList;
    Stack<Country>            undoStack;
    HashTable<Country>        nameTable;
    BinarySearchTree<Country> langTree;


    // Read primary input data
    if (!fileRead(coreDataList, nameTable, langTree))
        return 1;


    int userChoice = -1; 
    mainMenu();
    while (userChoice != 0)
    {
        std::cout << std::endl;
        std::cout << "[Type '9' to display menu options again.]" << std::endl;
        std::cout << "Please select an option from the menu: ";
        std::cin >> userChoice;
        // Then execute the respective selected option below.
        switch (userChoice)
        {
            case 1: // Add new Data
                addData(coreDataList, nameTable, langTree);
                break;
            case 2: // Delete Data
                removeData(coreDataList, nameTable, langTree, undoStack);
                break;
            case 3: // Search by country name (use the hash table)
                searchName(nameTable);
                break;
            case 4: // Search by language (use the BST)
                searchLang(langTree);
                break;
            case 5: // List countries sorted by language (use BST in-order traversal)
                langTree.inOrder();
                break;
            case 6: // Write Data to File
                writeToFile(nameTable, undoStack);
                break;
            case 7: // Output Hash Table Statistics
                hashStats(nameTable);
                break;
            case 8:
                undoRemove(undoStack, coreDataList, nameTable, langTree);
                break;
            case 9: // Display Main Menu
                mainMenu();
                break;
            case 0:
                endProgram(nameTable);
                break;
            case 42: // Display group member names (Hidden option)
                teamNames();
                break;
            case 47: // Secret option to display indented langTree
                langTree.printTree(visitBST_Node_indent);
                break;
            case 99: // DEBUG: Display coreDataList
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
    std::cout << "    Mahik Kaur  : Screen Output & File I/O            " << std::endl;
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
    std::cout << "6. Write data to file"         << std::endl; // Last bit of I/O baby.
    std::cout << "7. Hash Table Statistics"      << std::endl; // Print # of collisions, longest collision list, load factor, etc.
    std::cout << "8. Undo Last Deletion"         << std::endl;
    std::cout << "9. Display Menu"               << std::endl; // Show this menu once after start, then only upon request.

    std::cout << "0. Exit"                       << std::endl << std::endl;
}


// Little easter egg as per project requirements.
// MENU OPCODE: 42
void teamNames()
{
    std::cout << "##########################################" << std::endl;
    std::cout << "#                 Team 2                 #" << std::endl;
    std::cout << "##########################################" << std::endl;
    std::cout << "  Remy Dinh - Coordination and Integration" << std::endl;
    std::cout << "Shun Furuya - Hash List Algorithms"         << std::endl;
    std::cout << " Mahik Kaur - Screen & File Output"         << std::endl;
    std::cout << "Taeyoon Kim - BST Algorithms"               << std::endl;
    std::cout << "##########################################" << std::endl;
}


// Reads in data from the primaryData.csv input file.
// File to be read is currently hard-coded, but could relatively easily
// be changed to accept a filename from a user.
//
bool fileRead(LinkedList<Country> &coreDataList, HashTable<Country> &nameTable, BinarySearchTree<Country> &langTree)
{ 
    // This should only work after the first time the program is run, or if a database.csv file is provided.
    std::ifstream inputFile(databaseFile);
    std::string usedFile = databaseFile;

    // Failure to open database file, so attempt to read the standard input file
    if (!inputFile)
    {
        inputFile.open(stdInputFile);
        if (!inputFile) // Failure to open standard input file
        {
            std::cerr << "Input file " << stdInputFile << " could not be read." << std::endl;
            std::cerr << "Please check for path and filename. Program will now terminate." << std::endl;
            return false;
        }
        usedFile = stdInputFile;
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
                    language   = lineBuffer.substr(comma_1 + 1, comma_2 - comma_1 - 1),
                    religion   = lineBuffer.substr(comma_3 + 1, comma_4 - comma_3 - 1),
                    capital    = lineBuffer.substr(comma_6 + 1);
        unsigned    population = std::stoul(lineBuffer.substr(comma_2 + 1, comma_3 - comma_2 - 1));
        double      GDP        = std::stod (lineBuffer.substr(comma_4 + 1, comma_5 - comma_4 - 1)),
                    area       = std::stod (lineBuffer.substr(comma_5 + 1, comma_6 - comma_5 - 1));

        Country temp(name, language, population, religion, GDP, area, capital);
        ListNode<Country> *newDataNode = new ListNode<Country>(temp, nullptr);

        
        // Insert into coreDataList
        if (!coreDataList.insert(newDataNode))
        {
            std::cerr << "Duplicate primary key found in file " << usedFile << "!" << std::endl;
            std::cerr << "Duplicate record "<< temp.getName() << " has been rejected, but file read will continue." << std::endl;

            // Memory leaks make Mr. Bentley an unhappy camper!
            // Record was rejected, so since we allocated a node for this record, we need to clean it up.
            delete newDataNode; 
        }

        if (!nameTable.insert(newDataNode))
            std::cout << "Could not insert " << temp.getName() << " into the hash table." << std::endl;

        if (!langTree.insertBST(newDataNode, compareLang))
            std::cout << "Could not insert " << temp.getName() << " into the BST." << std::endl;

    }
    inputFile.close();

    return true;
}

// Adds data to the coreDataList, which will in turn be linked to by the BST
// and Hash Tables later on as they're implemented.
//
// MENU OPCODE: 1
//
void addData (LinkedList<Country> &coreDataList, HashTable<Country> &nameTable, BinarySearchTree<Country> &langTree)
{
    // Buffer variables
    std::string nameIn(""),
                langIn(""),
                relgIn(""),
                captIn("");
    unsigned    popIn  = 0u;
    double      gdpIn  = 0.0,
                areaIn = 0.0;

    std::cout << "What is the name of the country you want to add to the database? ";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, nameIn);

    if (nameTable.search(nameIn))
    {
        std::cout << nameIn << " already exists here in the database. Aborting operation." << std::endl;
        return;
    }

    std::cout << "What is the capital of " << nameIn << "? ";
    std::cin.clear();
    std::getline(std::cin, captIn);

    std::cout << "What is one of the recognized languages of this country? ";
    std::cin.clear();
    std::getline(std::cin, langIn);

    std::cout << "What is the major religion in this country? ";
    std::cin.clear();
    std::getline(std::cin, relgIn);

    std::cout << "How many people live in " << nameIn << "? ";
    std::cin.clear();
    std::cin >> popIn;

    std::cout << "What is the annual GDP of " << nameIn << " in USD? ";
    std::cin.clear();
    std::cin >> gdpIn;

    std::cout << "What is the surface area of " << nameIn << " in square kilometres? ";
    std::cin.clear();
    std::cin >> areaIn;

    Country tempObj(nameIn, langIn, popIn, relgIn, gdpIn, areaIn, captIn); 

    ListNode<Country>* newDataNode = new ListNode<Country>(tempObj, nullptr);

    if (!coreDataList.insert(newDataNode))
    {
        std::cout << "Failure to insert new record into the coreDataList!" << std::endl;
        return;
    }
    if (!nameTable.insert(newDataNode))
    {
        std::cout << "Failure to insert new record into the hash table!" << std::endl;
        return;
    }
    if (!langTree.insertBST(newDataNode, compareLang))
    {
        std::cout << "Failure to insert new record into the BST!" << std::endl;
        return;
    }

    std::cout << "Okay, " << nameIn << " has been added to the database.\n" << std::endl;
}


// Removes item from coreDataList and places it onto the undoStack.
// Will also call BST and HashTable's respective functions to break the links
// to their respective indirect nodes.
//
// Menu OPCODE: 2
//
void removeData (LinkedList<Country> &coreDataList, HashTable<Country> &nameTable, BinarySearchTree<Country> &langTree, Stack<Country> &undoStack)
{
    std::string query("");
    std::cout << "Please input the name of the country you want to remove: ";
    std::cin.ignore();
    std::getline(std::cin, query);

    HashNode<Country> *checker = nameTable.search(query);
    if (!checker) // checker == nullptr;
    {
        std::cout << query << " was not found in the database." << std::endl;
        return;
    }

    ListNode<Country> *holder = coreDataList.searchList(query);
    // Break the links in the BST and Hash Table first
    if (!nameTable.remove(query))
    {
        std::cout << "Unable to remove " << query << " from hash table." << std::endl;
        return;
    }
    if (!langTree.removeBST(holder, compareLang))
    {
        std::cout << "Unable to remove " << query << " from BST." << std::endl;
        return;
    }

    holder = coreDataList.removeItem(query);
    undoStack.push(holder); // Push removed item onto stack
    std::cout << query << " was removed from the database." << std::endl;
}


// Goes through hash table with queried Country Name
//
// OPCODE 3
//
void searchName (HashTable<Country> &nameTable)
{
    std::string query("");
    std::cout << "What country would you like to search for? ";
    std::cin.ignore();
    std::getline(std::cin, query);
    HashNode<Country> *holder = nameTable.search(query);

    if (!holder)
    {
        std::cout << query << " was not found in the database." << std::endl;
        return;
    }

    std::cout << holder->getItem();
    return;
}


// This function invokes a special in-order traversal of the BST that only visits a node
// if its language key matches that of the query.
// OPCODE 4
void searchLang (BinarySearchTree<Country> &langTree)
{
    std::string query("");
    std::cout << "What language would you like to query? ";
    std::cin.ignore();
    std::getline(std::cin, query);

    langTree.inOrder_query(compareLangQuery, query);
}


void hashStats (HashTable<Country> &nameTable)
{
    std::cout << ":---------------------:" << std::endl;
    std::cout << ":Hash Table Statistics:" << std::endl;
    std::cout << ":---------------------:" << std::endl;
    std::cout << "          Total Items: " << nameTable.getCount()      << std::endl;
    std::cout << "           Table Size: " << nameTable.getSize()       << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "          Load Factor: " << nameTable.getLoadFactor() << std::endl;
    std::cout << "    Space Utilization: " << nameTable.getSpaceUtil() << "%" << std::endl;
    std::cout << " Longest Chain Length: " << nameTable.getLongestChain() << " item(s)" << std::endl;
}


// Pops latest removed item from the undoStack, and reinserts it into the coreDataList,
// and will call upon the BST and Hash Table functions to reestablish links.
//
void undoRemove (Stack<Country> &undoStack, LinkedList<Country> &coreDataList, HashTable<Country> &nameTable, BinarySearchTree<Country> &langTree)
{
    ListNode<Country> *dataPtr = undoStack.pop();
    if (!dataPtr) // dataPtr == nullptr, so undo stack was empty.
        std::cout << "The deletion stack is empty!" << std::endl;
    else
    {
        // Reinsert into coreDataList
        coreDataList.insert(dataPtr);

        // Relink
        nameTable.insert(dataPtr);
        langTree.insertBST(dataPtr, compareLang);

        std::cout << dataPtr->getItem().getName() << " has been restored from the deletion stack." << std::endl;
    }
}

// This function writes all records in the hash table to an output file as specified by its
// argument, then wipes the deletion stack.
//
void writeToFile (HashTable<Country> &nameTable, Stack<Country> &undoStack)
{
    if (!nameTable.outputItems(databaseFile)) // databaseFile is a named constant here in main.cpp
    {
        std::cerr << "Database file write failure. Terminating process." << std::endl;
        return; // Failure to write to file
    }
    undoStack.clear();

    std::cout << std::endl;
    std::cout << "Database successfully written to file: " << databaseFile << std::endl << std::endl;

    std::cout << "***********************************************";
    std::cout << "Note: the undo deletion stack has been cleared." << std::endl << std::endl;
    std::cout << "***********************************************";
}


void endProgram (HashTable<Country> &nameTable)
{
    std::cout << std::endl;
    std::cout << "Writing records to output file: " << databaseFile << "..." << std::endl;
    if (nameTable.outputItems(databaseFile))
    {
        std::cout << "Write successful." << std::endl;
        std::cout << "Terminating application." << std::endl;
        std::cout << std::endl;
        std::cout << "Have a great day!" << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cerr << "It seems that there was an error in writing the database to file." << std::endl;
        std::cerr << "Please contact your nearest code monkey for assistance." << std::endl;
        std::cerr << "Have a nice day!" << std::endl;
    }
}


/* compareLang()
 * This function is passed into the BST member function calls so that the BST can make the proper
 * comparisons between the secondary key of each respective Country object.
 *
 * Return values at a glance:
 *  lhs == rhs : return 0
 *  lhs >  rhs : return 1
 *  lhs <  rhs : return 2
 */
unsigned compareLang(const Country &lhs, const Country &rhs)
{
    if (lhs.getLanguage() == rhs.getLanguage())
        return 0;
    if (lhs.getLanguage() >  rhs.getLanguage())
        return 1;
    return 2;
}


/* compareLangQuery()
 * This function is passed into the BST member function calls so that the BST can make the proper
 * comparisons between the secondary key of a Country object with that of a query string without
 * having to transfer the string into some sort of dummy Country object.
 *
 * Return values at a glance:
 *  lhs == rhs : return 0
 *  lhs >  rhs : return 1
 *  lhs <  rhs : return 2
 */
unsigned compareLangQuery(const Country &lhs, const std::string &queryStr)
{
    if (lhs.getLanguage() == queryStr)
        return 0;
    if (lhs.getLanguage() >  queryStr)
        return 1;
    return 2;
}

void levelIndent(const unsigned level)
{
    for (auto i = 0u; i < level; i++)
        std::cout << "  ";
}

// This visit function enables the BST to print out with indents based on a node's level.
//
void visitBST_Node_indent (const BinaryNode<Country> *nodePtr, const unsigned level)
{

    levelIndent(level);
    std::cout << "###########################################" << std::endl;
    levelIndent(level);
    std::cout << "            Country: " << nodePtr->getData().getName() << std::endl;
    levelIndent(level);
    std::cout << "       Capital City: " << nodePtr->getData().getCapitalCity() << std::endl;
    levelIndent(level);
    std::cout << "Recognized Language: " << nodePtr->getData().getLanguage() << std::endl;
    levelIndent(level);
    std::cout << "         Population: " << nodePtr->getData().getPopulation() << std::endl;
    levelIndent(level);
    std::cout << "     Major Religion: " << nodePtr->getData().getMajorReligion() << std::endl;
    levelIndent(level);
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "          GDP (USD): $" << nodePtr->getData().getGDP() << std::endl;
    levelIndent(level);
    std::cout << "       Surface Area: " << nodePtr->getData().getSurfaceArea() << " km^2" << std::endl;
    levelIndent(level);
    std::cout << "###########################################" << std::endl;
}
