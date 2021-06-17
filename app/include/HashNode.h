// Originally written by: Shun Furuya
//   Style & integration: Remy Dinh

#ifndef _HASH_NODE
#define _HASH_NODE

#include "ListNode.h"

/* Note from Remy (Integration Phase)
 * The HashNode class template is a "meta-node" so to speak, in the sense
 * that it has a dataPtr that points to a data node elsewhere in the program,
 * rather than directly storing a copy of the data itself.
 */

template<class ItemType>
class HashNode
{
private:
    ListNode<ItemType> *dataPtr;
    HashNode<ItemType> *next;

public:
    HashNode  ()                                                              { dataPtr = nullptr; next = nullptr; }
    HashNode  (ListNode<ItemType> *itemPtr)                                   { dataPtr = itemPtr; next = nullptr; }
    HashNode  (ListNode<ItemType> *itemPtr, HashNode<ItemType> *nextHashNode) { dataPtr = itemPtr; next = nextHashNode; }
    ~HashNode () {}
    
    // setters
    void setDataPtr (ListNode<ItemType> *dataNodePtr)  { dataPtr = dataNodePtr; }
    void setNext    (HashNode<ItemType> *nextHashNode) { next = nextHashNode; }
    
    // getters
    ItemType            getItem    () const { return dataPtr->getItem(); } // Wrapper for ListNode<ItemType>'s getItem()
    ListNode<ItemType>* getDataPtr () const { return dataPtr; }
    HashNode<ItemType>* getNext    () const { return next; }
};

#endif
