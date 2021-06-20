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

#include <iostream>
#include <iomanip>
#include <fstream>
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

    // Added to simplify file write
    void writeToFile(std::ofstream &outputFile);
};

template<class ItemType>
HashBucket<ItemType>::HashBucket()
{
    //head = nullptr; // Unused old pointer inherited from LinkedList

    bucketHead = new HashNode<ItemType>; // Allocate sentinel node, bucketHead points directly to it.
    bucketHead->setNext(bucketHead);     // Point at self, "Snake eats its tail"
    itemCount = 0u;
}

// This destructor assumes that the hashBucket has at least the sentinel node
// remaining, otherwise the second line will result in an access violation.
//
//  pop() from the bucket until only sentinel node remains, then delete it? lol
//
template<class ItemType>
HashBucket<ItemType>::~HashBucket()
{
     // Pop an item if possible, should return nullptr if only the sentinel node remains.
    HashNode<ItemType> *terminator = bucketHead->getNext(),
                       *nextNode   = terminator->getNext();
    while (terminator != bucketHead)
    {
        delete terminator;
        terminator = nextNode;
        nextNode = nextNode->getNext();
    }

    // Now that the list is clear, only the sentinel node remains, so delete the sentinel node.
    delete bucketHead;
}


template<class ItemType>
bool HashBucket<ItemType>::insertItem(HashNode<ItemType> *inputNode)
{
    HashNode<ItemType> *pCur = bucketHead->getNext(),
                       *pPre = bucketHead;

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
    if (isEmpty())
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
    if (isEmpty()) // Only sentinel node remains.
        return nullptr;

    // topItem points to the item at the front of the list
    HashNode<ItemType> *frontItem = bucketHead->getNext();

    // bucketHead points directly to the sentinel node, so point that
    // sentinel node to the next item (if there is any)
    // bucketHead->getNext()->setNext(frontItem->getNext()); // I'm not sure what I was smoking...
    bucketHead->setNext(frontItem->getNext());

    // Disconnect frontItem from the current bucket
    frontItem->setNext(nullptr);

    itemCount--;
    return frontItem;
}

// Implemented by Remy in polish phase to simplify file write function
//
// Writes each record as new line.
//
template<class ItemType>
void HashBucket<ItemType>::writeToFile(std::ofstream &outputFile)
{
    HashNode<ItemType> *wanderer = bucketHead->getNext(); // Skip sentinel node

    // Iterate through the whole bucket until we reach sentinel node
    while (wanderer != bucketHead)
    {
        outputFile << wanderer->getItem().getName() << ",";
        outputFile << wanderer->getItem().getLanguage() << ",";
        outputFile << wanderer->getItem().getPopulation() << ",";
        outputFile << wanderer->getItem().getMajorReligion() << ",";

        outputFile << std::fixed << std::setprecision(2);
        outputFile << wanderer->getItem().getGDP() << ",";
        outputFile << wanderer->getItem().getSurfaceArea() << ",";
        outputFile << wanderer->getItem().getCapitalCity(); // No trailing comma after the last item
        outputFile << std::endl;

        wanderer = wanderer->getNext();
    }

}

#endif
