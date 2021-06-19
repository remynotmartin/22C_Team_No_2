/*    Originally written by: Taeyoon Kim
 * Style and integration by: Remy Dinh
 * 
 * This BinaryNode is to be used by the Binary Tree & BST
 * Similarly to the HashNode, the BinaryNode class template as prepared
 * here is a "meta-node", in that it points to another node that contains
 * data, rather than directly containing a copy of the data itself.
 *
 */


#ifndef _BINARY_NODE
#define _BINARY_NODE

#include "ListNode.h"

template<class ItemType>
class BinaryNode
{   
private:
	ListNode<ItemType>   *dataPtr;      // Pointer to Data Node
	BinaryNode<ItemType> *leftPtr;		// Pointer to left child
	BinaryNode<ItemType> *rightPtr;		// Pointer to right child

public:
	// constructors
	BinaryNode(ListNode<ItemType> *anItemPtr) { dataPtr = anItemPtr; leftPtr = nullptr; rightPtr = nullptr; }
	~BinaryNode() {}

	// setters
	void setDataPtr  (ListNode<ItemType>   *aDataPtr) { dataPtr  = aDataPtr; }
	void setLeftPtr  (BinaryNode<ItemType> *left)     { leftPtr  = left; }
    void setRightPtr (BinaryNode<ItemType> *right)    { rightPtr = right; }
    
	// getters
    ItemType              getData()     const { return dataPtr->getItem(); }
	ListNode<ItemType>*   getDataPtr()  const { return dataPtr;  }
	BinaryNode<ItemType>* getLeftPtr()  const { return leftPtr;  }
	BinaryNode<ItemType>* getRightPtr() const { return rightPtr; }

    // other functions
    bool isLeaf() const {return (leftPtr == nullptr && rightPtr == nullptr);}
    
}; 

#endif 
