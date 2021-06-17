// Binary Tree class
//             Created by: Taeyoon Kim
// Style & Integration by: Remy Dinh
 
#ifndef _BINARY_TREE
#define _BINARY_TREE

#include "BinaryNode.h"

template<class ItemType>
class BinaryTree
{
protected:
    BinaryNode<ItemType> *rootPtr;        // ptr to root node
    unsigned              count;          // number of nodes in tree

public:
    // "admin" functions
    BinaryTree()                                  {rootPtr = 0; count = 0;}
    BinaryTree(const BinaryTree<ItemType> & tree) {}
    virtual ~BinaryTree()                         { destroyBST(rootPtr); }
   
    // common functions for all binary trees
    bool     isEmpty   ()                            const { return count == 0; }
    unsigned getCount  ()                            const { return count; }
    void     clear     ()                                  { destroyTree(rootPtr); rootPtr = 0; count = 0; }
    void     inOrder   (void visit(ItemType &))      const { _inorder(visit, rootPtr); }
    void     printTree (void visit(ItemType &, int)) const { _printTree(visit, rootPtr, 1); }
    
    //
    // insert a node at the correct location
    bool insertBST(const ItemType &item);
    // remove a node if found
    bool removeBST(const ItemType &item);
    // find a target node
    bool searchBST(const ItemType &target, ItemType &returnedItem) const;
    // delete all nodes from the tree
    void destroyBST(BinaryNode<ItemType>* nodePtr);
    // print sameKey
    void printSameKey(const ItemType &successor);
    
private:

    // internal traverse
    void _inorder   (void visit(ItemType &), BinaryNode<ItemType>* nodePtr)                 const;
    void _printTree (void visit(ItemType &, int), BinaryNode<ItemType>* nodePtr, int level) const;

    // internal insert node: insert newNode in nodePtr subtree
    void _insert (BinaryNode<ItemType>* nodePtr, BinaryNode<ItemType>* newNode);
   
    // search for target node
    void _search (BinaryNode<ItemType>* treePtr, const ItemType &target) const;
    
    // internal remove node: locate and delete target node under nodePtr subtree
    void _remove (BinaryNode<ItemType>* nodePtr, const ItemType target, bool &success);
    
     // delete target node from tree, called by internal remove node
    void _removeNode (BinaryNode<ItemType>* targetNodePtr);
    
     // remove the leftmost node in the left subtree of nodePtr
    void _removeLeftmostNode (BinaryNode<ItemType>* nodePtr, ItemType &successor);
};


//Wrapper for _insert - Inserting items within a tree
template<class ItemType>
bool BinaryTree<ItemType>::insertBST(const ItemType & newEntry)
{
    BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);
    this->rootPtr = _insert(this->rootPtr, newNodePtr);

    return true;
}

//Wrapper for _search
// - it calls the private _search function that returns a Node pointer or NULL
// - if found, it copies data from that node and sends it back to the caller
//   via the output parameter, and returns true, otherwise it returns false.
template<class ItemType>
bool BinaryTree<ItemType>::searchBST(const ItemType& anEntry, ItemType & returnedItem) const
{
    BinaryNode<ItemType>* temp = nullptr;
   
    temp = _search(this->rootPtr, anEntry); // get searched and returned node;
    if(temp){
        returnedItem = temp->getItem(); // get the returned valuse into returnedItem; returnedItem is College infos and temp is a node that has infos;
        return true;
    }
    return false;
}

//Wrapper for _remove
// - it calls the private _search function that returns a Node pointer or NULL
// - if found, it copies data from that node and sends it back to the caller
//   via the output parameter, and returns true, otherwise it returns false.
template<class ItemType>
bool BinaryTree<ItemType>::removeBST(const ItemType &item)
{
    bool isSuccess = false;
    this->rootPtr = _remove(this->rootPtr, item, isSuccess);
    return isSuccess;
}


//Destroy the entire tree
template<class ItemType>
void BinaryTree<ItemType>::destroyBST(BinaryNode<ItemType>* nodePtr)
{
    if(nodePtr) // != NULL
    {
        destroyBST(nodePtr->getLeftPtr());
        destroyBST(nodePtr->getRightPtr());
        
        delete nodePtr;
    }
}

template<class ItemType>
void BinaryTree<ItemType>::printSameKey(const ItemType &target)
{
    BinaryNode<ItemType>* found = nullptr;
 
    found = _search(this->rootPtr, target); // get searched and returned node;
    while (found)
    {   write Print!! PLEASE!!
        // ADD Print FUNCTION!!!!@!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        found = _search(found->getRightPtr(), target);
    }
}
    
//////////////////////////// private functions ////////////////////////////////////////////

//Implementation of the insert operation
template<class ItemType>
void BinaryTree<ItemType>::_insert(BinaryNode<ItemType>* nodePtr, BinaryNode<ItemType>* newNodePtr)
{
    BinaryNode<ItemType>* pWalk = nodePtr, *parent = nullptr;
    
    if( !nodePtr) // == NULL
    {
        nodePtr = newNodePtr;
        return nodePtr;
    }
    else
    {
        parent = pWalk;
        if(pWalk->getItem() > newNodePtr->getItem())
        {
            if(pWalk->getLeftPtr())
                _insert(pWalk->getLeftPtr(), newNodePtr);
            else
                parent->setLeftPtr(newNodePtr);
        }
        else
        {
            if(pWalk->getRightPtr())
                _insert(pWalk->getRightPtr(), newNodePtr);
            else
                parent->setRightPtr(newNodePtr);
        }
    }

    return nodePtr;
}

//Implementation for the search operation
// - return NULL if target not found, otherwise
// - returns a pointer to the node that matched the target
template<class ItemType>
void BinaryTree<ItemType>::_search(BinaryNode<ItemType>* nodePtr, const ItemType &target) const
{
    BinaryNode<ItemType>* found = nullptr;
    ItemType tempTarget = target;
    
    if (!nodePtr)
    {
        found = nodePtr;
        return found;
    }
    if (tempTarget < nodePtr->getItem())
        return _search(nodePtr->getLeftPtr(), target);
    else if (tempTarget > nodePtr->getItem())
        return _search(nodePtr->getRightPtr(), target);
    else
    {
        found = nodePtr;
    }
        
    return found;
}


// internal remove node: locate and delete target node under nodePtr subtree
template<class ItemType>
void BinaryTree<ItemType>::_remove(BinaryNode<ItemType>* nodePtr, const ItemType target, bool &success)
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
void BinaryTree<ItemType>::_removeNode(BinaryNode<ItemType>* targetNodePtr)
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
void BinaryTree<ItemType>::_removeLeftmostNode(BinaryNode<ItemType>* nodePtr, ItemType &successor)
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

//Inorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_inorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const
{
    if (nodePtr) // != NULL
    {
        ItemType item = nodePtr->getItem();
        _inorder(visit, nodePtr->getLeftPtr());
        visit(item);
        _inorder(visit, nodePtr->getRightPtr());
    }
}

    
//Prints tree as an indented list
template<class ItemType>
void BinaryTree<ItemType>::_printTree(void visit(ItemType &, int), BinaryNode<ItemType>* nodePtr, int level) const
{
    if(!nodePtr)
    {
        return;
    }
        
    ItemType item = nodePtr->getItem();
    visit(item, level);
    _printTree(visit, nodePtr->getRightPtr(), level+1);
    _printTree(visit, nodePtr->getLeftPtr(), level+1);
}

#endif

