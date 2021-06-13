/*                     Originally written by: Taeyoon Kim
 * Style and integration adjustments made by: Remy Dinh
 * 
 * This BinaryNode is to be used by the Binary Search Tree
 */


#ifndef _BINARY_NODE
#define _BINARY_NODE

#include "ListNode.h"

template<class ItemType>
class BinaryNode
{   
private:
	ListNode<ItemType>   *dataPtr;         // Data portion
	BinaryNode<ItemType> *leftPtr;		// Pointer to left child
	BinaryNode<ItemType> *rightPtr;		// Pointer to right child

public:
	// constructors
	BinaryNode(ListNode<ItemType> *anItemPtr) { dataPtr = anItemPtr; leftPtr = nullptr; rightPtr = nullptr; }
	BinaryNode(ListNode<ItemType> *anItemPtr, 
		       BinaryNode<ItemType> *left, 
			   BinaryNode<ItemType> *right) { dataPtr = anItemPtr; leftPtr = left; rightPtr = right;}
    ~BinaryNode() {}

	// setters
	void setDataPtr  (ListNode<ItemType> *aDataPtr)  { dataPtr  = aDataPtr; }
	void setLeftPtr  (BinaryNode<ItemType> *left)    { leftPtr  = left; }
    void setRightPtr (BinaryNode<ItemType> *right)   { rightPtr = right; }
    
	// getters
	ListNode<ItemType>*   getDataPtr()  const { return dataPtr;  }
	BinaryNode<ItemType>* getLeftPtr()  const { return leftPtr;  }
	BinaryNode<ItemType>* getRightPtr() const { return rightPtr; }

    // other functions
    bool isLeaf() const {return (leftPtr == 0 && rightPtr == 0);}
    
}; 

#endif 
