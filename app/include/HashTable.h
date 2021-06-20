// Originally written by: Shun Furuya
//   Style & Integration: Remy Dinh

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <string> // Needed by the _hash() function
#include <cmath>  // Needed for the round() function when checking load factor in insert()
#include "ListNode.h"

#include "HashBucket.h"

using std::string;

template<class ItemType>
class HashTable
{
private:
    HashBucket<ItemType> *hashAry;
    unsigned              hashSize;
    unsigned              count;    // Total # of items in the hash table.
    
public:
    HashTable  ()           { count = 0; hashSize = 47; hashAry = new HashBucket<ItemType>[hashSize]; }
    HashTable  (unsigned n) { count = 0; hashSize = n;  hashAry = new HashBucket<ItemType>[hashSize]; }
    ~HashTable ()           { delete [] hashAry; }

    // Accessors
    unsigned getCount        () const { return count; }
    unsigned getSize         () const { return hashSize; }
    bool     isEmpty         () const { return count == 0; }

    // Used for Table Statistics
    double   getLoadFactor   () const { return (static_cast<double>(count) / hashSize); }
    double   getSpaceUtil    () const;
    unsigned getLongestChain () const;

    // Basic operations
    bool                insert ( ListNode<ItemType> *itemPtr);
    bool                remove ( const string &key );
    HashNode<ItemType>* search ( const string &key );

    // Used to output to file
    bool outputItems(const string &outputFilename) const;

    // Bonus function
    // Since we're a team of four, we need to implement this rehash function when our load factor reaches 0.75
    // This will be used by insertion()
    void rehash();
    
private:
    unsigned _hash(const string &key) const;
};

// One of the pertinent hash table statistics, space utilization is the ratio
// between used buckets and unused buckets.
//
// 100.00 * [non-empty buckets] / [total available buckets]
//
// Added by Remy during integration phase.
//
template<class ItemType>
double HashTable<ItemType>::getSpaceUtil() const
{
    unsigned usedBuckets = 0u;
    for (unsigned i = 0u; i < hashSize; i++)
    {
        //std::cout << "Bucket " << i << " has " << hashAry[i].getItemCount() << " items." << std::endl;
        if (hashAry[i].getItemCount() > 0)
            usedBuckets++;
    }
    return 100.00 * usedBuckets / hashSize;
}

// This function goes through the HashTable's buckets and reads the length of each bucket,
// returning the length of the longest chain.
// Added by Remy during integration phase.
//
template<class ItemType>
unsigned HashTable<ItemType>::getLongestChain() const
{
    unsigned longest = 0u;
    for (unsigned i = 0u; i < hashSize; i++)
    {
        if (hashAry[i].getItemCount() > longest)
            longest = hashAry[i].getItemCount();
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


    // Allocate new HashNode<ItemType>
    HashNode<ItemType> *newItemNode = new HashNode<ItemType>(itemPtr);
    
    if (search(key) == nullptr)
    {
        hashAry[bucket].insertItem(newItemNode);
        count++; // Increment table's overall count, not the bucket's

        // Check if load factor has reached or surpassed 0.75
        // If it has, rehash the table.
        double   currentLoadFactor = getLoadFactor();
        unsigned convertedFactor   = round(currentLoadFactor * 100.00);
        if (convertedFactor >= 75)
            rehash();
        
        return true;
    }
    
    // Case: Record already exists in the hash table.
    delete newItemNode; // No longer needed, so get rid of it!
    return false;
}


// This function will remove a queried item's hashNode from its respective bucket, if it exists.
// The LinkedList::removeItem() function returns a pointer to the removed node
// Returns true upon successful removal of meta-node HashNode, and false otherwise.
//
template<class ItemType>
bool HashTable<ItemType>::remove(const string &key)
{
    if (hashAry[_hash(key)].removeItem(key))
    {
        count--; // Decrement table's overall count, not the bucket's
        return true;
    }
    return false;
}

// Added by Remy in an emergency patch after I realized I forgot to output items in hash table order,
// rather than the order of the hash table.
//
template<class ItemType>
bool HashTable<ItemType>::outputItems(const string &outputFilename) const
{
    std::ofstream outputFile(outputFilename);
    if (!outputFile)
    {
        std::cerr << "Could not open output file " << outputFilename << " for writing!" << std::endl;
        return false;
    }
    for (auto i = 0u; i < hashSize; i++)
        hashAry[i].writeToFile(outputFile);
    return true;
}


// Searches through a particular bucket (which itself is LinkedList of HashNodes
//
/* Remy's integration notes:
 *  - Changed return type from int to HashNode<ItemType>*
 */
template<class ItemType>
HashNode<ItemType>* HashTable<ItemType>::search(const string &key)
{
    return hashAry[_hash(key)].searchItem(key);
}


// This function will resize the hash table to the next prime number after resizing it,
// then rehashing all of the current elements to the new table.
//
// Once this is done, it deallocates the old table, then points the hashAry pointer to
// the new table.
//
// Implemented by Remy in the polish stage, now that everything else is successfully integrated.
//
template<class ItemType>
void HashTable<ItemType>::rehash()
{
    unsigned oldHashSize = hashSize;

    // New Hash Table Size Algorithm provided by Shun-san
    // Integrated by Remy
    //
    unsigned newTableSize = oldHashSize * 2;
    bool     isPrime      = false;
    while (!isPrime)
    {
        isPrime = true;
        newTableSize++;
        for (auto i = 2u; i < newTableSize / 2; i++)
        {
            if (newTableSize % i == 0) // "If newTableSize can be divided by i with no remainder, it isn't prime!"
            {
                isPrime = false;
                break;
            }
        }
    }

    // newTableSize is now the next largest prime thanks to Shun-san's algorithm.
    // Allocate a new array of HashBuckets 
    HashBucket<ItemType> *newTablePtr = new HashBucket<ItemType>[newTableSize];

    // Update hashSize, since we have the original hash size saved in oldHashSize
    hashSize = newTableSize;


    // Iterate through the old table, bucket by bucket.
    // For each bucket, transfer hash nodes to new table until the bucket is empty.
    // Proceed to next bucket until end of old table is reached (save the old hashSize temporarily)
    for (auto i = 0u; i < oldHashSize; i++)
    {
        HashNode<ItemType> *holder = hashAry[i].popItem();
        while (holder != nullptr)
        {
            // Call hash; it should return the proper bucket in the new hash table because
            // hashSize has been updated.
            std::string key       = holder->getItem().getName();
            unsigned    newBucket = _hash(key);

            // Transfer popped HashNode to the new table in its correct new bucket.
            newTablePtr[newBucket].insertItem(holder);
            holder = hashAry[i].popItem();
        }
        // Bucket has been depleted after exiting while loop, so now we move on to the next bucket.
    }


    // Deallocate the old hash array.
    // I think the HashBucket destructor will take care of the remaining sentinel nodes for me.
    delete [] hashAry;
    

    // Point hashAry to the new Hash Table
    hashAry = newTablePtr;
}

#endif // HASHTABLE_H_
