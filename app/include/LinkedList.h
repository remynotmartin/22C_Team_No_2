//           Originally written by: Shun Furuya
// Integration & Style adjustments: Remy Dinh

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include<string>
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
    unsigned            getLength   ()                               const {return length;}
    bool                insertNode  (const ItemType&);
    void                displayList ()                               const;
    ListNode<ItemType> *searchList  (const std::string&)             const;
    ListNode<ItemType> *removeItem  (const std::string &target);
    void                reinsert    (ListNode<ItemType> *restoredItem); // Used by the undoStack
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
    // To be done: Free the list such that no memory leak occurs.
    ListNode<ItemType>* wanderer = head->getNext(),
                        nextNode = wanderer->getNext();
    while (wanderer != head)
    {
        delete wanderer;
        wanderer = nextNode;
        nextNode = nextNode->getNext();
    }
    // Now that the main nodes are gone, don't forget to delete the sentinel node.
    delete wanderer;
}


// Notes by Remy (during integration phase)
// - return type changed from void to bool
// - I called a search on the list to check if the primary key (country name) already exists
template <class ItemType>
bool LinkedList<ItemType>::insertNode(const ItemType &dataIn)
{
    // Search list first to ensure primary key doesn't already exist in the database
    if (searchList(dataIn.getName()) != nullptr)
        return false;

    ListNode<ItemType> *newNode,
                       *pCur,
                       *pPre;

    newNode       = new ListNode<ItemType>;
    newNode->setItem(dataIn);

    pPre = head;             // Start pre at Sentinel Node
    pCur = head->getNext();  // Start cur at "1st Node"

    while (pCur != head && newNode->getItem() > pCur->getItem())
    {
        pPre = pCur;
        pCur = pCur->getNext();
    }
    
    pPre->setNext(newNode);
    newNode->setNext(pCur);

    
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

// Implemented by Remy during the integration phase
// of this project. Needed to undo a deletion.
//
// reinsert() takes a ListNode<ItemType> from the undoStack
// and reconnects it into the data list.
//
template <class ItemType>
void LinkedList<ItemType>::reinsert(ListNode<ItemType> *restoredNode)
{
    ListNode<ItemType> *pPre    = head,
                       *pInsert = head->getNext();
    while (pInsert != head && pInsert->getItem() < restoredNode->getItem())
    {
        pPre    = pInsert;
        pInsert = pInsert->getNext();
    }

    // Position found, reconnect the node
    restoredNode->setNext(pInsert);
    pPre->setNext(restoredNode);
    length++;
}

#endif
