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
    int                 length;

public:
    LinkedList();  // constructor
    ~LinkedList(); // destructor

    // Linked list operations
    int  getLength   ()                               const {return length;}
    bool insertNode  (const ItemType&);
    bool deleteNode  (const std::string&);
    void displayList ()                               const;
    bool searchList  (const std::string&, ItemType &) const;
    bool prependList (const ItemType&);
};

template <class ItemType>
LinkedList<ItemType>::LinkedList()
{
    head = new ListNode<ItemType>; // Sentinel Node
    head->setNext(head);           // Point at itself
    length = 0;
}

// Implemented by Remy (during integration phase)

template <class ItemType>
LinkedList<ItemType>::~LinkedList()
{
    // To be done: Free the list such that no memory leak occurs.
}


// Notes by Remy (during integration phase)
// - return type changed from void to bool
// - I called a search on the list to check if the primary key (country name) already exists
template <class ItemType>
bool LinkedList<ItemType>::insertNode(const ItemType &dataIn)
{
    ItemType durak; 
    // Search list first to ensure primary key doesn't already exist
    if (searchList(dataIn.getName(), durak))
        return false;

    ListNode<ItemType> *newNode,
                       *pCur,
                       *pPre;

    newNode       = new ListNode<ItemType>;
    newNode->setItem(dataIn);

    pPre = head;        // Start pre at Sentinel Node
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

template <class ItemType>
bool LinkedList<ItemType>::deleteNode(const std::string &target)
{
    ListNode<ItemType> *pCur,
                       *pPre;

    bool deleted = false;
    
    pPre = head;
    pCur = head->getNext();

    while (pCur != NULL && pCur->getItem() < target)
    {
        pPre = pCur;
        pCur = pCur->getNext();
    }
    
    if (pCur && pCur->getItem() == target)
    {
        pPre->setNext(pCur->getNext());
        delete pCur;
        deleted = true;
        length--;
    }
    return deleted;
}

template <class ItemType>
bool LinkedList<ItemType>::searchList(const std::string &target, ItemType &dataOut) const
{
    bool found = false;
    ListNode<ItemType> *pCur;
       
    pCur = head->getNext();
    while (pCur != head && pCur->getItem() < target)
    {
        pCur = pCur->getNext();
    }
    
    if (pCur->getItem() == target)
    {
        dataOut = pCur->getItem();
        found   = true;
    }

    return found;
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

#endif
