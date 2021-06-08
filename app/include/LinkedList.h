//        Written by: Shun Furuya
// Style adjustments: Remy Dinh

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "Country.h"

template<class ItemType>
class LinkedList
{
private:
    struct Node
    {
        ItemType item;
        Node    *next;
    };

    Node *head;
    int   length;

public:
    LinkedList();   // constructor
    ~LinkedList();  // destructor

    // Linked list operations
    int  getLength   ()                   const {return length;}
    void insertNode  (ItemType);
    bool deleteNode  (const string);
    void displayList ()                   const;
    bool searchList  (string, ItemType &) const;
};

template <class ItemType>
LinkedList<ItemType>::LinkedList()
{
    head = new Node;
    head->setNext(head);
    length = 0;
}

template <class ItemType>
void LinkedList<ItemType>::insertNode(ItemType dataIn)
{
    Node *newNode;
    Node *pCur;
    Node *pPre;

    newNode       = new Node;
    newNode->item = dataIn;

    pPre = head;
    pCur = head->next;

    while (pCur && newNode->item > pCur->item)
    {
        pPre = pCur;
        pCur = pCur->next;
    }
    
    pPre->next    = newNode;
    newNode->next = pCur;
    
    length++;
}

template <class ItemType>
bool LinkedList<ItemType>::deleteNode(const string target)
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
bool LinkedList<ItemType>::searchList(const string target, ItemType &dataOut) const
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

#endif
