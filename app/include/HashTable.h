// Originally written by: Shun Furuya
//   Style & Integration: Remy Dinh

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <string> // Needed by the _hash() function
#include "LinkedList.h"
#include "ListNode.h"

using std::string;

template<class ItemType>
class HashTable
{
private:
    LinkedList<HashNode<ItemType>> *hashAry;
    unsigned                        hashSize;
    unsigned                        count;
    
public:
    HashTable  ()      { count = 0; hashSize = 47; hashAry = new LinkedList<HashNode<ItemType>>[hashSize]; }
    HashTable  (int n) { count = 0; hashSize = n;  hashAry = new LinkedList<HashNode<ItemType>>[hashSize]; }
    ~HashTable ()      { delete [] hashAry; }

    // Accessors
    unsigned getCount        () const { return count; }
    unsigned getSize         () const { return hashSize; }
    bool     isEmpty         () const { return count == 0; }

    // Used for Table Statistics
    double   getLoadFactor   () const { return 100.0 * count / hashSize; }
    double   getSpaceUtil    () const;
    unsigned getLongestChain () const;

    // Basic operations
    bool                         insert ( ListNode<ItemType> *itemPtr);
    bool                         remove ( ListNode<ItemType> *itemOut, const string &key);
    HashNode<ListNode<ItemType>* search (const string &key);

    // Bonus function
    // Since we're a team of four, we need to implement this rehash function when our load factor reaches 0.75
    //void rehash();
    
private:
    unsigned _hash(const string &key) const;
};

// One of the pertinent hash table statistics, space utilization is the ratio
// between used buckets unused buckets.
//
// 100.00 * [non-empty buckets] / [total available buckets]
//
// Added by Remy during integration phase.
//
template<class ItemType>
double   HashTable<ItemType>::getSpaceUtil() const
{
    unsigned usedBuckets = 0u;
    for (unsigned i = 0u; i< hashSize; i++)
    {
        if (hashAry[i].getCount > 0)
            usedBuckets++;
    }
    return 100.00 * usedBuckets / hashSize;
}

// This function goes through the HashTable's buckets and reads the count of each list,
// returning the count of the longest chain.
// Added by Remy during integration phase.
//
template<class ItemType>
unsigned HashTable<ItemType>::getLongestChain() const
{
    unsigned longest = 0u;
    for (unsigned i = 0u; i < hashSize; i++)
    {
        if (hashAry[i].getCount() > longest)
            longest = hashAry[i].getCount();
    }
    return longest;
}

/* Basic Hash Algorithm to get an indexing value from a
 * C++ std::string key.
 * - I changed the return type from `int` to `unsigned` (Remy)
 * - Changed pass by value to pass-by-reference to save a
 *   bit of memory. (Remy)
 * - Changed from string to const string to prevent data corruption (Remy)
 *
 */
template<class ItemType>
unsigned HashTable<ItemType>::_hash(const string &key) const
{
    int sum = 0;
    for (int i = 0; key[i]; i++)
        sum += key[i];
    return sum % hashSize;
}


// Inserts new hashNode into its proper bucket, if the item doesn't already exist
// in the hash table.
template<class ItemType>
bool HashTable<ItemType>::insert(ListNode<ItemType> *itemPtr)
{
    // Grab item from the item pointer, then extract its name
    string   key    = itemPtr->getItem().getName();
    unsigned bucket = _hash(key);
    
    if (!search(key))
    {
        HashNode<ItemType> *newHashNode = new HashNode<ItemType>(itemPtr);
        hashAry[bucket].insertNode(newHashNode);
        count++;
        return true;
    }
    
    // Case: Record already exists in the hash table.
    return false;
}


// This function will remove a queried item's hashNode from its respective bucket, if it exists.
// The LinkedList::removeItem() function returns a pointer to the removed node
// Returns true upon successful removal of meta-node HashNode, and false otherwise.
//
template<class ItemType>
bool HashTable<ItemType>::remove(ListNode<ItemType> *itemOut, const string &key)
{
    ListNode<ItemType> *holder = hashAry[_hash(key)].removeItem(key);
    if(holder)
    {
        delete holder;
        count--;
        return true;
    }
    return false;
}

// Searches through a particular bucket (which itself is LinkedList of HashNodes
//
/* Remy's integration notes:
 *  - Changed return type from int to HashNode<ItemType>*
 */
template<class ItemType>
HashNode<ItemType>* HashTable<ItemType>::search(const string &key)
{
    /* The HashNode meta-node class has a wrapper for ListNode<ItemType>'s getItem() function,
     * so we should be able to use the LinkedList class template's searchList() function.
     * It returns a pointer to the HashNode meta-node containing the queried data, if it exists
     * in the coreDataList, and by extension, in the queried bucket (linked list) in the hashAry.
     */
    return hashAry[_hash(key)].searchList(key);
}


#endif // HASHTABLE_H_
