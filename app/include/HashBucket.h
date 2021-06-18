// Written by: Remy Dinh
//
// Class template written to fulfill bucket functionality.
//
// I wanted to use a hashAry of LinkedList objects, but
// doing LinkedList<HashNode<ItemType> would result in the HashNodes
// being encapsulated by ListNodes, which is a bit of a headache.
//

#ifndef HASH_BUCKET
#define HASH_BUCKET

#include "LinkedList.h"
#include "HashNode.h"

template<class ItemType>
class HashBucket : public LinkedList<ItemType>
{
private:
    HashNode<ItemType> *bucketHead;
    unsigned length;

public:
    HashBucket();
    ~HashBucket();

    // Getters
    unsigned getCollisionCount() const { return (length == 0) ? 0 : length - 1; } // First item isn't a collision!
    // getLength() inherited from LinkedList

    // Utilities
    bool                insertItem (HashNode<ItemType> *inputNode);
    bool                removeItem (const std::string  &query);
    HashNode<ItemType>* searchItem (const std::string  &query);

};

template<class ItemType>
HashBucket<ItemType>::HashBucket()
{
    //head = nullptr; // Unused old pointer inherited from LinkedList

    bucketHead = new HashNode<ItemType>; // Allocate sentinel node,
    bucketHead->setNext(bucketHead);     // Point at self, "Snake eats its tail"
    length = 0u;
}

template<class ItemType>
HashBucket<ItemType>::~HashBucket()
{
    HashNode<ItemType> *terminator = bucketHead->getNext(), // Jump past the sentinel node
                       *nextNode   = terminator->getNext();
    while (terminator != bucketHead)
    {
        delete terminator;
        terminator = nextNode;
        nextNode   = nextNode->getNext();
    }
    // Now that we're back at the sentinel node, get rid of it.
    delete terminator;
}


template<class ItemType>
bool HashBucket<ItemType>::insertItem(HashNode<ItemType> *inputNode)
{
    if (searchItem(inputNode->getItem().getName()) != nullptr)
    {
        std::cout << "This item already exists in the table!" << std::endl;
        return false;
    }

    HashNode<ItemType> *pCur,
                       *pPre;
    pPre = bucketHead;         // pre starts at sentinel node
    pCur = bucketHead->getNext();

    while (pCur != bucketHead && inputNode->getItem() > pCur->getItem())
    {
        pPre = pCur;
        pCur = pCur->getNext();
    }

    // "Weave" the new node into the bucket
    pPre->setNext(inputNode);
    inputNode->setNext(pCur);
    length++;                 // Increment bucket's length (not the table's)
    return true;
}

// Unlike with the coreDataList, there's no undo stack for HashNodes in this program,
// so this function erases nodes, rather than simply disconnecting and returning them.
//
template<class ItemType>
bool HashBucket<ItemType>::removeItem(const std::string &query)
{
    if (searchItem(query) == nullptr)
    {
        std::cout << "Item is not in the hash table!" << std::endl;
        return false;
    }

    HashNode<ItemType> *dataPre = bucketHead,
                       *dataPtr = bucketHead->getNext();
    while (dataPtr != bucketHead && dataPtr->getItem() < query)
    {
        dataPre = dataPtr;
        dataPtr = dataPtr->getNext();
    }

    if (dataPtr->getItem() == query)
    {
        dataPre->setNext(dataPtr->getNext()); // Patch previous node to successor
        dataPtr->setNext(nullptr);            // Disconnect queried node
        delete dataPtr;                       // Wipe the queried node
        length--;                             // Decrement bucket's length (not the table's)
    }

    return true;
}


template<class ItemType>
HashNode<ItemType>* HashBucket<ItemType>::searchItem(const std::string &query)
{
    if (length == 0)
        return nullptr;
    HashNode<ItemType> *pCur = bucketHead->getNext(); // Skip sentinel node

    while (pCur != bucketHead && pCur->getItem() < query)
        pCur = pCur->getNext();

    if (pCur == bucketHead)
        return nullptr;

    if (pCur->getItem() == query)
        return pCur;

    return nullptr; // Item's not in the bucket!
}

#endif
