//           Originally written by: Shun Furuya
// Integration & Style adjustments: Remy Dinh

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include<string>

template<class ItemType>
class LinkedList
{
private:
    struct Node
    {
        ItemType  item;
        Node     *next;
    };

    Node *head;
    int   length;

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
    head       = new Node;   // Sentinel Node
    head->next = head;
    length     = 0;
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

    Node *newNode,
         *pCur,
         *pPre;

    newNode       = new Node;
    newNode->item = dataIn;

    pPre = head;        // Start pre at Sentinel Node
    pCur = head->next;  // Start cur at "1st Node"

    while (pCur != head && newNode->item > pCur->item)
    {
        pPre = pCur;
        pCur = pCur->next;
    }
    
    pPre->next    = newNode;
    newNode->next = pCur;

    
    length++;
    return true;
}

template <class ItemType>
bool LinkedList<ItemType>::deleteNode(const std::string &target)
{
    Node *pCur;
    Node *pPre;
    bool  deleted = false;
    
    pPre = head;
    pCur = head->next;

    while (pCur != NULL && pCur->item < target)
    {
        pPre = pCur;
        pCur = pCur->next;
    }
    
    if (pCur && pCur->item == target)
    {
        pPre->next = pCur->next;
        delete pCur;
        deleted = true;
        length--;
    }
    return deleted;
}

template <class ItemType>
bool LinkedList<ItemType>::searchList(const std::string &target, ItemType &dataOut) const
{
    bool  found = false;
    Node *pCur;
       
    pCur = head->next;
    while (pCur != head && pCur->item < target)
    {
        pCur = pCur->next;
    }
    
    if (pCur->item == target)
    {
        dataOut = pCur->item;
        found = true;
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
    Node     *wanderer = head->next; // Skip past the sentinel node
    unsigned itemCount = 1u;

    while (wanderer != head)
    {
        std::cout << std::endl;
        std::cout << "Item: " << itemCount << std::endl;
        std::cout << wanderer->item;
        wanderer = wanderer->next;
        itemCount++;
    }
}

#endif
