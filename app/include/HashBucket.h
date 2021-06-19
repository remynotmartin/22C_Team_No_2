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

#include "HashNode.h"

template<class ItemType>
class HashBucket
{
private:
    HashNode<ItemType> *bucketHead;
    unsigned            itemCount;

public:
    HashBucket();
    ~HashBucket();

    // Getters
    unsigned getCollisionCount () const { return (itemCount == 0) ? 0 : (itemCount - 1); } // First item isn't a collision!
    unsigned getItemCount      () const { return itemCount; }


    // Utilities
    bool                isEmpty    ()                               const { return itemCount == 0; }
    bool                insertItem (HashNode<ItemType> *inputNode);
    bool                removeItem (const std::string  &query);
    HashNode<ItemType>* searchItem (const std::string  &query);

    // Added to simplify rehashing operation
    HashNode<ItemType>* popItem    ();
};

template<class ItemType>
HashBucket<ItemType>::HashBucket()
{
    //head = nullptr; // Unused old pointer inherited from LinkedList

    bucketHead = new HashNode<ItemType>; // Allocate sentinel node,
    bucketHead->setNext(bucketHead);     // Point at self, "Snake eats its tail"
    itemCount = 0u;
}

template<class ItemType>
HashBucket<ItemType>::~HashBucket()
{
    HashNode<ItemType> *terminator = bucketHead->getNext(), // Jump past the sentinel node
                       *nextNode   = terminator->getNext();
    
    while (terminator != bucketHead) // Terminator is on a real/non-sentinel node
    {
        
        delete terminator;
        terminator = nextNode;
        nextNode   = terminator->getNext();

    }

    // Now that we're back at the sentinel node, get rid of it.
    delete terminator;
}


template<class ItemType>
bool HashBucket<ItemType>::insertItem(HashNode<ItemType> *inputNode)
{
    HashNode<ItemType> *pCur = bucketHead,            // pre starts at sentinel node
                       *pPre = bucketHead->getNext();

    while (pCur != bucketHead && inputNode->getItem() > pCur->getItem())
    {
        pPre = pCur;
        pCur = pCur->getNext();
    }

    // "Weave" the new node into the bucket
    pPre->setNext(inputNode);
    inputNode->setNext(pCur);
    itemCount++;
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
        itemCount--;
    }

    return true;
}


template<class ItemType>
HashNode<ItemType>* HashBucket<ItemType>::searchItem(const std::string &query)
{
    if (itemCount == 0)
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


// Implemented by Remy to simplify the rehash operation
//
template<class ItemType>
HashNode<ItemType>* HashBucket<ItemType>::popItem()
{
    if (bucketHead == bucketHead->getNext()) // Only sentinel node remains.
        return nullptr;

    HashNode<ItemType> *topItem = bucketHead->getNext();

    // bucketHead points directly to the sentinel node, so point that
    // sentinel node to the next item (if there is any)
    bucketHead->getNext()->setNext(topItem->getNext());

    // Disconnect topItem from the bucket
    topItem->setNext(nullptr);

    itemCount--;
    return topItem;
}

#endif
