//           Originally written by: Shun Furuya
// Integration & Style adjustments: Remy Dinh

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <fstream> // For Mahik's outputItems() function.
#include <string>
#include "ListNode.h" // For the unified ListNode class template

template<class ItemType>
class LinkedList
{
private:
    ListNode<ItemType> *head;
    unsigned            length;

public:
    LinkedList();  // constructor
    ~LinkedList(); // destructor

    // Linked list operations
    unsigned            getLength   ()                                   const {return length;}
    bool                insert      (ListNode<ItemType> *inputDataNode);
    ListNode<ItemType> *searchList  (const std::string&)                 const;
    ListNode<ItemType> *removeItem  (const std::string &target);
    void                displayList ()                                   const;

    // File I/O utility function implemented by Mahik, and integrated by Remy.
    void                outputItems (const std::string &outputFilename)  const;
                                    
};

template <class ItemType>
LinkedList<ItemType>::LinkedList()
{
    head = new ListNode<ItemType>; // Sentinel Node
    head->setNext(head);           // Point at itself, "Snake eats its own tail"
    length = 0;
}

// Implemented by Remy (during integration phase)

template <class ItemType>
LinkedList<ItemType>::~LinkedList()
{
    ListNode<ItemType> *wanderer = head->getNext(),
                       *nextNode = wanderer->getNext();
    while (wanderer != head)
    {
        delete wanderer;
        wanderer = nextNode;
        nextNode = nextNode->getNext();
    }
    // Now that the main nodes are gone, don't forget to delete the sentinel node.
    delete head;
}


// Notes by Remy (during integration phase)
// - return type changed from void to bool
// - I called a search on the list to check if the primary key (country name) already exists
// - Changed argument type from ItemType to ListNode<ItemType>* to make it easier to reinsert
//   nodes from the undoStack.
template <class ItemType>
bool LinkedList<ItemType>::insert(ListNode<ItemType> *inputDataNode)
{
    // Search list first to ensure primary key doesn't already exist in the database
    if (searchList(inputDataNode->getItem().getName()) != nullptr)
        return false;
    
    ListNode<ItemType> *pCur,
                       *pPre;

    pPre = head;             // Start pre at Sentinel Node
    pCur = head->getNext();  // Start cur at "1st Node"

    while (pCur != head && inputDataNode->getItem() > pCur->getItem())
    {
        pPre = pCur;
        pCur = pCur->getNext();
    }
    
    pPre->setNext(inputDataNode);
    inputDataNode->setNext(pCur);
    
    length++;
    return true;
}

// Implemented by Remy during integration phase
//
// removeItem() is meant to work by disconnecting a node from the coreDataList,
// and then passing it back to the caller to manage the node, rather than deleting
// the matching data node outright.
//
template <class ItemType>
ListNode<ItemType> *LinkedList<ItemType>::removeItem(const std::string &target)
{
    ListNode<ItemType> *dataPre = head, 
                       *dataPtr = head->getNext();
    while (dataPtr != head && dataPtr->getItem() < target)
    {
        dataPre = dataPtr;
        dataPtr = dataPtr->getNext();
    }

    if (dataPtr->getItem() == target)
    {
        dataPre->setNext(dataPtr->getNext()); // Patch previous node to next
        dataPtr->setNext(nullptr);            // Disconnect data from list
        length--;
        return dataPtr;
    }
    
    return nullptr; // Item was not found in database.

}

// The SearchList() function queries the list for an item which has an attribute
// (usually the primary key) that matches the queried target.
// The function then returns a pointer to the item, if found.
// Otherwise, it returns nullptr.
//
template <class ItemType>
ListNode<ItemType> *LinkedList<ItemType>::searchList(const std::string &target) const
{
    ListNode<ItemType> *pCur;
       
    pCur = head->getNext();
    while (pCur != head && pCur->getItem() < target)
        pCur = pCur->getNext();
    
    if (pCur->getItem() == target)
        return pCur;

    return nullptr; // Item not found in database.
}


// displayList()
//
// Implemented by Remy during the integration phase
// of this project, primarily for purposes of testing.
template <class ItemType>
void LinkedList<ItemType>::displayList() const
{
    ListNode<ItemType> *wanderer  = head->getNext(); // Skip past the sentinel node
    unsigned            itemCount = 1u;

    while (wanderer != head)
    {
        std::cout << std::endl;
        std::cout << "Item: " << itemCount << std::endl;
        std::cout << wanderer->getItem();
        wanderer = wanderer->getNext();
        itemCount++;
    }
}


//             Implemented by: Mahik
// Integration adjustments by: Remy
//
// This function outputs all records of the coreDataList to an output file. Originally Mahik had the 
// name of the output file included here in the LinkedList header file, but I changed it to be an argument
// from the function caller.
//
// Mahik originally passed in a lot of variables, but I decided to just make them local to the function
// since they're just buffers for item data members.
//
// I also added a file open check for good measure.
//
template <class ItemType>
void LinkedList<ItemType>::outputItems(const std::string &outputFilename) const
{
     ListNode<ItemType> *pCur;  // To move through the list

     // Position pCur: skip the sentinel node of the list.
     pCur = head->getNext();

     std::ofstream outputFile(outputFilename);
     if (!outputFile)
     {
         std::cerr << "Failed to open output file " << outputFilename << " for record write. Aborting process." << std::endl;
         return;
     }

     // Buffer variables
     std::string name       = "",
                 language   = "",
                 religion   = "",
                 capital    = "";
     double      GDP        = 0.0,
                 area       = 0.0;
     unsigned    population = 0u;

     while (pCur != head)
     {
          // Data out
          name       = pCur->getItem().getName();
          language   = pCur->getItem().getLanguage();
          population = pCur->getItem().getPopulation();
          religion   = pCur->getItem().getMajorReligion();
          GDP        = pCur->getItem().getGDP();
          area       = pCur->getItem().getSurfaceArea();
          capital    = pCur->getItem().getCapitalCity();

          outputFile << name << ";" << language << ";" << population << ";" << religion << ";" << GDP << ";" << area << ";" << capital << std::endl;

          // Move to the next node.
         pCur = pCur->getNext();
     }
    outputFile.close();
}

#endif
