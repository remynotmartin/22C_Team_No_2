//             Coder: Shun Furuya
// Style Adjustments: Remy Dinh

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "HashNode.h"

template<class ItemType>
class HashTable
{
private:
    HashNode<ItemType> *hashAry;
    int                 hashSize;
    int                 count;
    
public:
    HashTable  ()      { count = 0; hashSize = 47; hashAry = new HashNode<ItemType>[hashSize]; }
    HashTable  (int n) { count = 0; hashSize = n;  hashAry = new HashNode<ItemType>[hashSize]; }
    ~HashTable ()      { delete [] hashAry; }

    int    getCount      () const { return count; }
    int    getSize       () const { return hashSize; }
    double getLoadFactor () const { return 100.0 * count / hashSize; }
    bool   isEmpty       () const { return count == 0; }
    bool   isFull        () const { return count == hashSize; }
    
    
    bool insert ( const ItemType &itemIn);
    bool remove ( ItemType &itemOut, const ItemType &key);
    int  search ( ItemType &itemOut, const string &key);
    
private:
    int _hash(string key) const;
};


template<class ItemType>
int HashTable<ItemType>::_hash(string key) const
{
    int sum = 0;
    for (int i = 0; key[i]; i++)
        sum += key[i];
    return sum % hashSize;
};


template<class ItemType>
bool HashTable<ItemType>::insert( const ItemType &itemIn)
{
    int    bucket = _hash(itemIn.getName());
    string key    = itemIn.getName();
    
    if (search(itemIn, key) == NULL)
    {
        HashNode<ItemType> list = hashAry[bucket];
        list.getList().insertNode(itemIn);
        return true;
    }
}


template<class ItemType>
bool HashTable<ItemType>::remove(ItemType &itemOut, const ItemType &key)
{
    return true;
}


template<class ItemType>
int HashTable<ItemType>::search(ItemType &itemOut, const string &key)
{
    HashNode<ItemType> list = hashAry[_hash(key)];
    if (list.getList().searchList(key, itemOut))
        return 1;
    else
        return -1;
}


#endif // HASHTABLE_H_
