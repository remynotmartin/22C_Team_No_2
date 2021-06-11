// Binary tree abstract base class
// Created by Taeyoon Kim
 
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
    BinaryTree() {rootPtr = 0; count = 0;}
    BinaryTree(const BinaryTree<ItemType> & tree){ }
    virtual ~BinaryTree() { destroyTree(rootPtr); }
   
    // common functions for all binary trees
    bool     isEmpty   ()                            const { return count == 0; }
    unsigned getCount  ()                            const { return count; }
    void     clear     ()                                  { destroyTree(rootPtr); rootPtr = 0; count = 0; }
    void     inOrder   (void visit(ItemType &))      const { _inorder(visit, rootPtr); }
    void     printTree (void visit(ItemType &, int)) const { _printTree(visit, rootPtr, 1); }

    // abstract functions to be implemented by derived class
    bool insertBST (const ItemType &item);
    bool searchBST (const ItemType &target, ItemType &returnedItem) const;

private:
    // delete all nodes from the tree
    void destroyTree (BinaryNode<ItemType>* nodePtr);

    // internal traverse
    void _inorder   (void visit(ItemType &), BinaryNode<ItemType>* nodePtr)                 const;
    void _printTree (void visit(ItemType &, int), BinaryNode<ItemType>* nodePtr, int level) const;
    void _insert    (BinaryNode<ItemType>* nodePtr, BinaryNode<ItemType>* newNode);

    // search for target node
    void _search (BinaryNode<ItemType>* treePtr, const ItemType &target) const;
   
};

//Wrapper for _insert - Inserting items within a tree
template<class ItemType>
bool BinaryTree<ItemType>::insertBST(const ItemType & newEntry)
{
    BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);
    this->rootPtr = _insert(this->rootPtr, newNodePtr);
    return true;
}

//Wrapper for _searchBST - Searching items within a tree
template<class ItemType>
bool BinaryTree<ItemType>::searchBST(const ItemType& anEntry, ItemType & returnedItem) const
{
    BinaryNode<ItemType>* temp = nullptr;
   
    temp = _search(this->rootPtr, anEntry); // get searched and returned node;
    if(temp)
    {
        returnedItem = temp->getItem(); // get the returned valuse into returnedItem; returnedItem is College infos and temp is a node that has infos;
        return true;
    }
    return false;
}

//Destroy the entire tree
template<class ItemType>
void BinaryTree<ItemType>::destroyBST(BinaryNode<ItemType>* nodePtr)
{
    if(nodePtr) // != NULL
    {
        destroyTree(nodePtr->getLeftPtr());
        destroyTree(nodePtr->getRightPtr());
        //cout << "DEBUG - Destructor: Now deleting " << nodePtr->getItem().getName() << endl;
        delete nodePtr;
    }
}

//Implementation of the insert operation
template<class ItemType>
void BinaryTree<ItemType>::_insert(BinaryNode<ItemType>* nodePtr,
                                                          BinaryNode<ItemType>* newNodePtr)
{
    BinaryNode<ItemType>* pWalk = nodePtr, *parent = nullptr;
    
    if( !nodePtr) // == NULL
    {
        nodePtr = newNodePtr;
        return nodePtr;
    }
    else
    {
        while(pWalk) // != NULL
        {
            parent = pWalk;
            if (pWalk->getItem() > getItem())
                pWalk = pWalk->getLeftPtr();
            else
                pWalk = pWalk->getRightPtr();
        }
        if (parent->getItem() > getItem())
            parent->setLeftPtr(newNodePtr);
        else
            parent->setRightPtr(newNodePtr);
    }

    return nodePtr;
}

// Implementation for the search operation
template<class ItemType>
void BinaryTree<ItemType>::_search(BinaryNode<ItemType>* nodePtr,
                                                           const ItemType &target) const
{
    BinaryNode<ItemType>* found = nullptr;
    ItemType tempTarget = target;
    
    if (!nodePtr)
    {
        found = nodePtr;
        return found;
    }
    
    while (nodePtr)
    {
        if (tempTarget < nodePtr->getItem())
            found = nodePtr->getLeftPtr();
        else if (tempTarget > getItem())
           found = nodePtr->getRightPtr();
        else
            found = nodePtr;
    }
    
    return found;
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

