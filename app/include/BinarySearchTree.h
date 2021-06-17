// Binary Tree Class Template
//  Originally written by: Taeyoon Kim
// Style & Integration by: Remy Dinh
 
#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include "BinaryNode.h" // A "meta-node", so to speak; already includes "ListNode.h"

template<class ItemType>
class BinarySearchTree
{
protected:
    BinaryNode<ItemType> *rootPtr;        // ptr to root node
    unsigned              count;          // number of nodes in tree

public:
    // "admin" functions
    BinarySearchTree()  { rootPtr = nullptr; count = 0u; }
    ~BinarySearchTree() { destroyBST(rootPtr); }
   
    // Common functions for all binary trees
    bool     isEmpty   ()                                             const { return count == 0; }
    unsigned getCount  ()                                             const { return count; }
    void     clear     ()                                                   { destroyTree(rootPtr); rootPtr = nullptr; count = 0; }
    void     inOrder   (void visit(const BinaryNode<ItemType>*))      const { _inorder(visit, rootPtr); }

    // To be used by a secret menu option, prints an indented tree using in-order traversal.
    void printTree (void visit(const BinaryNode<ItemType>*, const unsigned)) const { _printTree(visit, rootPtr, 1); }
    
    // insert a node at the correct location
    bool insertBST    (const ListNode<ItemType> *itemPtr, unsigned compare(const ItemType&, const ItemType&));

    // remove a node if found
    bool removeBST    (const ListNode<ItemType> *itemPtr, unsigned compare(const ItemType&, const ItemType&));

    // find a target node
    BinaryNode<ItemType>* searchBST (const ItemType &target, unsigned compare(const ItemType&, const ItemType&)) const;

    // delete all nodes from the tree
    void destroyBST (BinaryNode<ItemType>* nodePtr);
    
private:

    // internal traverse
    void _inorder   (void visit(const BinaryNode<ItemType>*), BinaryNode<ItemType>* nodePtr) const;
    void _printTree (void visit(const BinaryNode<ItemType>*, const unsigned), BinaryNode<ItemType>* nodePtr, const unsigned level) const;

    // internal insert node: insert newNode in nodePtr subtree
    void _insert (BinaryNode<ItemType> *nodePtr, BinaryNode<ItemType> *newNode, unsigned compare(const ItemType&, const ItemType&);
   
    // search for target node
    BinaryNode<ItemType>* _search (BinaryNode<ItemType> *treePtr, const ItemType &target, unsigned compare()) const;
    
    // internal remove node: locate and delete target node under nodePtr subtree
    void _remove (BinaryNode<ItemType> *nodePtr, const ItemType target, bool &success);
    
     // delete target node from tree, called by internal remove node
    void _removeNode (BinaryNode<ItemType>* targetNodePtr);
    
     // remove the leftmost node in the left subtree of nodePtr
    void _removeLeftmostNode (BinaryNode<ItemType>* nodePtr, ItemType &successor);
};


// Wrapper for _insert - Inserting items within a tree
// Remy integration notes:
//  - Removed redundant this-> pointer invokation before rootPtr,
//    since 'this' is implicit to the class template in this context.
//
template<class ItemType>
void BinarySearchTree<ItemType>::insertBST(const ListNode<ItemType> *newEntry, unsigned compare(const ItemType&, const ItemType&))
{
    // Dynamically allocate a new BinaryNode to be inserted into the tree.
    // The constructor points the new node's dataPtr directly to the data
    // node that was passed in.
    BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);

    _insert(rootPtr, newNodePtr, compare);
    count++;
}


// Wrapper for _remove
// - it calls the private _search function that returns a Node pointer or NULL
// - if found, it copies data from that node and sends it back to the caller
//   via the output parameter, and returns true, otherwise it returns false.
template<class ItemType>
bool BinarySearchTree<ItemType>::removeBST(const ItemType &item)
{
    bool isSuccess = false;
    rootPtr = _remove(rootPtr, item, isSuccess);
    return isSuccess;
}

/* [Remy's Notes from Integration Purgatory]
 *   - Changed return type from bool to BinaryNode<ItemType>* and adjusted logic
 *     accordingly.
 */
// Wrapper for _search
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::searchBST(const ItemType &target
                                                            unsigned        compare (const ItemType&, const ItemType&)) const
{
    return _search(rootPtr, target, compare);
}


// Recursively destroys the entire tree.
template<class ItemType>
void BinarySearchTree<ItemType>::destroyBST(BinaryNode<ItemType>* treeRoot)
{
    if (treeRoot) // != NULL
    {
        destroyBST(treeRoot->getLeftPtr());
        destroyBST(treeRoot->getRightPtr());
        
        delete treeRoot;
    }
}

    
//////////////////////////// private functions ////////////////////////////////////////////

//Implementation of the insert operation
// 
// [Remy, Notes from Integration Purgatory]
//      - I removed a redundant BinaryNode<ItemType>* pointer called parent.
//      - Fixed incorrect type returns
//      - Renamed pWalk to pCur
//
// The initial call of this function is made on the root of the tree, rootPtr
// Subsequent recursive calls are made on roots of subtrees.
//
// A compare function will be passed in from main to make the proper comparisons, since
// for the purposes of this project, the Binary Tree sorts its nodes based on the 
// recognized language (secondary key), rather than the name (primary key).
//
// The compareLang(lhs, rhs) function passed in from main() for this project returns
// these values:
//  lhs == rhs : return 0
//  lhs >  rhs : return 1
//  lhs <  rhs : return 2
//
template<class ItemType>
void BinarySearchTree<ItemType>::_insert(BinaryNode<ItemType> *treeRoot,
                                         BinaryNode<ItemType> *newNodePtr,
                                         unsigned              compare(const ItemType&, const ItemType&))
{
    BinaryNode<ItemType> *pCur = treeRoot;
    
    if (!treeRoot) // == NULL
    {
        treeRoot = newNodePtr;
        return;
    }

    unsigned compResult = compare(pCur->getItem(), newNodePtr->getItem());

    // "You're going to see that trees really lend themselves to recursion." - Delia, Winter Quarter 2017
    //
    // "Thank goodness." - Remy, Spring Quarter 2021

    if (compResult == 1) // newNodePtr's item's secondary key < pCur's item's secondary key, so go left
    {
        if (pCur->getLeftPtr()) // A left node already exists
            _insert(pCur->getLeftPtr(), newNodePtr);
        else
            pCur->setLeftPtr(newNodePtr);
    }
    else // Case where newNodePtr's item's secondary key is either greater or equal to pCur's
    {
        if (pCur->getRightPtr()) // A right node already exists
            _insert(pCur->getRightPtr(), newNodePtr);
        else
            pCur->setRightPtr(newNodePtr);
    }
}


// Implementation for the search operation
//  - return NULL if target not found, otherwise
//  - returns a pointer to the node that matched the target
//
// [Remy's notes from Integration Purgatory]
//  - Added compare function to handle secondary key comparisons
//
// The compareLang(lhs, rhs) function passed in from main() for this project returns
// these values:
//  lhs == rhs : return 0
//  lhs >  rhs : return 1
//  lhs <  rhs : return 2
//
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_search (BinaryNode<ItemType> *subTreeRoot,
                                                           const ItemType       &target,
                                                           unsigned              compare (const ItemType &lhs, const ItemType &rhs)) const
{
    if (!subTreeRoot) // subTreeRoot == nullptr
    {
        return subTreeRoot;
    }
    unsigned compResult = compare(target, subTreeRoot->getItem());

    if      (compResult == 2)
        return _search(subTreeRoot->getLeftPtr(), target);
    else if (compResult == 1)
        return _search(subTreeRoot->getRightPtr(), target);
    else if (compResult == 0)
    {
        // Be careful. Here we have a secondary key match,
        // but now we need to check the primary keys.
        if (subTreeRoot->getItem() == target) // ItemType's overloaded operator compares primary keys
            return subTreeRoot;

        // If previous test fails (Primary keys do not match) continue check for right subtree,
        // because items with duplicate secondary keys get pushed to the right.
        else
            return _search(subTreeRoot->getRightPtr(), target());
    }
        
}


// internal remove: locate and delete target node under nodePtr subtree
template<class ItemType>
void BinarySearchTree<ItemType>::_remove(BinaryNode<ItemType>* nodePtr, const ItemType target, bool &success)
{
    if (!nodePtr) // == NULL
    {
        success = false;
        return nodePtr;
    }
    if (nodePtr->getItem() > target)
        nodePtr->setLeftPtr(_remove(nodePtr->getLeftPtr(), target, success));
    else if (nodePtr->getItem() < target)
        nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), target, success));
    else
    {
        nodePtr =  _removeNode(nodePtr);
        success = true;
    }
    return nodePtr;
}

 // delete target node from tree, called by internal remove node
template<class ItemType>
void BinarySearchTree<ItemType>::_removeNode(BinaryNode<ItemType>* targetNodePtr)
{
    if (targetNodePtr->isLeaf())    // no leaf
    {
        delete targetNodePtr;
        targetNodePtr = nullptr;
        return targetNodePtr;
    }
    else if (!targetNodePtr->getLeftPtr())  //Right
    {
        BinaryNode<ItemType>* rightChild = targetNodePtr->getRightPtr();
        delete targetNodePtr;
        targetNodePtr = nullptr;
        return rightChild;
    }
    else if (!targetNodePtr->getRightPtr()) //Left
    {
        BinaryNode<ItemType>* leftChild = targetNodePtr->getLeftPtr();
        delete targetNodePtr;
        targetNodePtr = nullptr;
        return leftChild;
    }
    else    // have both childrens
    {
        ItemType newNodeVal;
        targetNodePtr->setRightPtr(_removeLeftmostNode(targetNodePtr->getRightPtr(), newNodeVal));
        targetNodePtr->setItem(newNodeVal);
        return targetNodePtr;
    }
}

 // remove the leftmost node in the left subtree of nodePtr
template<class ItemType>
void BinarySearchTree<ItemType>::_removeLeftmostNode(BinaryNode<ItemType>* nodePtr, ItemType &successor)
{
    if (!nodePtr->getLeftPtr())
    {
        successor = nodePtr->getItem();
        return _removeNode(nodePtr);
    }
    else
    {
        nodePtr->setLeftPtr(_removeLeftmostNode(nodePtr->getLeftPtr(), successor));
        return nodePtr;
    }
}


// [ Remy's notes from Integration Purgatory ]
//   - Adjusted parameter type of the visit() function, and adjusted visit's argument
//     within the function body accordingly.
//   - Otherwise, looks solid, pending further testing.
//
//Inorder Traversal
template<class ItemType>
void BinarySearchTree<ItemType>::_inorder(void visit(BinaryNode<ItemType>*), BinaryNode<ItemType>* nodePtr) const
{
    if (nodePtr) // != NULL
    {
        _inorder(visit, nodePtr->getLeftPtr());
        visit(nodePtr);
        _inorder(visit, nodePtr->getRightPtr());
    }
}

    
// [ Remy's notes from Integration Purgatory ]
//  - Adjusted the visit() function parameters
// Prints tree as an indented list.
// This is basically a special case of the in-order traversal, just using a different visit function.
// This might get rather chunky, depending on how deep the tree goes, and how wide one's terminal is.
//
template<class ItemType>
void BinarySearchTree<ItemType>::_printTree (void                  visit (BinaryNode<ItemType>*, const unsigned),
                                             BinaryNode<ItemType> *nodePtr,
                                             const unsigned        level) const
{
    if(isEmpty())
    {
        std::cout << "Tree is empty!" << std::endl;
        return;
    }
        
    _printTree(visit, nodePtr->getRightPtr(), level+1);
    visit(nodePtr, level);
    _printTree(visit, nodePtr->getLeftPtr(), level+1);
}

#endif

