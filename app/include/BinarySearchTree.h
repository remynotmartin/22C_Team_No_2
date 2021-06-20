// Binary Search Tree Class Template
//  Originally written by: Taeyoon Kim
// Style & Integration by: Remy Dinh
 
#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include <string>
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

    void     inOrder       () const { _inorder(rootPtr); }
    void     inOrder_query (unsigned compare(const ItemType&, const std::string&), const std::string query) const { _inorder_query(rootPtr, query, compare); }

    // To be used by a secret menu option, prints an indented tree using in-order traversal.
    void printTree (void visit(const BinaryNode<ItemType>*, const unsigned)) const { _printTree(visit, rootPtr, 0); }
    
    // insert a node at the correct location
    bool insertBST (ListNode<ItemType> *itemPtr, unsigned compare(const ItemType&, const ItemType&));

    // Break links to a node from the coreDataList
    bool removeBST (const ListNode<ItemType> *itemPtr, unsigned compare(const ItemType&, const ItemType&));

    // find a target node
    BinaryNode<ItemType>* searchBST (const ItemType &target, unsigned compare(const ItemType&, const ItemType&)) const;

    // delete all nodes from the tree
    void destroyBST (BinaryNode<ItemType>* nodePtr);
    
private:

    // internal traverse
    void _inorder       (BinaryNode<ItemType> *nodePtr) const;
    void _inorder_query (BinaryNode<ItemType> *nodePtr, const std::string &query, unsigned compare(const ItemType&, const std::string&)) const;
    void _printTree     (void visit(const BinaryNode<ItemType>*, const unsigned), BinaryNode<ItemType> *nodePtr, const unsigned level) const;

    // internal insert node: insert newNode in nodePtr subtree
    void _insert (BinaryNode<ItemType> *nodePtr, BinaryNode<ItemType> *newNode, unsigned compare(const ItemType&, const ItemType&));
   
    // search for target node
    BinaryNode<ItemType>* _search (BinaryNode<ItemType> *treePtr, const ItemType &target, unsigned compare(const ItemType&, const ItemType&)) const;
    
    // internal remove node: locate and delete target node under nodePtr subtree
    bool _remove (BinaryNode<ItemType> *treeRoot, const ItemType &target, unsigned compare(const ItemType&, const ItemType&));
    
};


// Wrapper for _insert - Inserting items within a tree
// Remy integration notes:
//  - Removed redundant this-> pointer invokation before rootPtr,
//    since 'this' is implicit to the class template in this context.
//
template<class ItemType>
bool BinarySearchTree<ItemType>::insertBST(ListNode<ItemType> *newEntry, unsigned compare(const ItemType&, const ItemType&))
{
    // Dynamically allocate a new BinaryNode to be inserted into the tree.
    // The constructor points the new node's dataPtr directly to the data
    // node that was passed in.
    BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);
    _insert(rootPtr, newNodePtr, compare);
    count++;
    return true;
}


// Wrapper for _remove
// - it calls the private _search function that returns a Node pointer or NULL
// - if found, it copies data from that node and sends it back to the caller
//   via the output parameter, and returns true, otherwise it returns false.
template<class ItemType>
bool BinarySearchTree<ItemType>::removeBST(const ListNode<ItemType> *targetNode, unsigned compare(const ItemType&, const ItemType&))
{
    return _remove(rootPtr, targetNode->getItem(), compare);
}


/* [Remy's Notes from Integration Purgatory]
 *   - Changed return type from bool to BinaryNode<ItemType>* and adjusted logic
 *     accordingly.
 */
// Wrapper for _search
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::searchBST(const ItemType &target,
                                                            unsigned        compare (const ItemType&, const ItemType&)) const
{
    return _search(rootPtr, target, compare);
}


// Recursively destroys the entire tree.
//
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
void BinarySearchTree<ItemType>::_insert(BinaryNode<ItemType> *subTreeRoot,
                                         BinaryNode<ItemType> *newNodePtr,
                                         unsigned              compare(const ItemType&, const ItemType&))
{
    if (rootPtr == nullptr) // Handles case of empty tree
        rootPtr = newNodePtr;
    else
    {
        unsigned compResult = compare(subTreeRoot->getData(), newNodePtr->getData());

        // "You're going to see that trees really lend themselves to recursion." - Delia, Winter Quarter 2017
        //
        // "Thank goodness." - Remy, Spring Quarter 2021

        if (compResult == 1) // newNodePtr's item's secondary key < pCur's item's secondary key, so go left
        {
            if (subTreeRoot->getLeftPtr()) // A left node already exists
                _insert(subTreeRoot->getLeftPtr(), newNodePtr, compare);
            else
                subTreeRoot->setLeftPtr(newNodePtr);
        }
        else // Case where newNodePtr's item's secondary key is either greater or equal to pCur's
        {
            if (subTreeRoot->getRightPtr()) // A right node already exists
                _insert(subTreeRoot->getRightPtr(), newNodePtr, compare);
            else
                subTreeRoot->setRightPtr(newNodePtr);
        }
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
        return subTreeRoot;

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

// The compareLang(lhs, rhs) function passed in from main() for this project returns
// these values (it compares the secondary keys):
//  lhs == rhs : return 0
//  lhs >  rhs : return 1
//  lhs <  rhs : return 2
//
// internal remove: locate and delete target node under subtree
//
// Remy: I rewrote this private remove function based on the algorithm provided by zyBooks,
//        since the old solution was resulting in a segmentation fault.
//
template<class ItemType>
bool BinarySearchTree<ItemType>::_remove(BinaryNode<ItemType> *treeRoot, const ItemType &target, unsigned compare(const ItemType&, const ItemType&))
{
    BinaryNode<ItemType> *parent  = nullptr,
                         *current = treeRoot;

    while (current)
    {
        unsigned compResult = compare(current->getData(), target);

        if (compResult == 0) // Match secondary key
        {
            if (current->getData() == target) // Match Primary Key
            {
                if (current->isLeaf()) // Remove Canadian/Leaf
                {
                    if (!parent)
                        rootPtr = nullptr;
                    else if (parent->getLeftPtr() == current)
                        parent->setLeftPtr(nullptr);
                    else
                        parent->setRightPtr(nullptr);
                    delete current;
                }
                else if (current->getRightPtr() == nullptr) // Remove node with only left child
                {
                    if (!parent) // Node is the tree root
                        rootPtr = current->getLeftPtr();
                    else if (parent->getLeftPtr() == current)
                        parent->setLeftPtr(current->getLeftPtr());
                    else
                        parent->setRightPtr(current->getLeftPtr());
                    delete current;
                }
                else if (current->getLeftPtr() == nullptr) // Remove node with only right child
                {
                    if (!parent) // Node is the tree root
                        rootPtr = current->getRightPtr();
                    else if (parent->getLeftPtr() == current)
                        parent->setLeftPtr(current->getRightPtr());
                    else
                        parent->setRightPtr(current->getRightPtr());
                    delete current;
                }
                else // Remove a node with two children
                {
                    // Find successor (leftmost child of the right subtree)
                    BinaryNode<ItemType> *successor = current->getRightPtr();
                    while (successor->getLeftPtr())
                        successor = successor->getLeftPtr();

                    ListNode<ItemType> *successorDataPtr= successor->getDataPtr(); // Copy the successor's data location

                    // Remove the successor from tree. This will also delete the node,
                    // so we shouldn't try deleting it again.
                    _remove(rootPtr, successor->getData(), compare); 

                    successor = nullptr; // Don't double delete!

                    current->setDataPtr(successorDataPtr);
                }
                count--;
                return true;
            }
            else
            {
                parent  = current;
                current = current->getRightPtr();
            }
        }
        else if (compResult == 2) // Search Right
        {
            parent  = current;
            current = current->getRightPtr();
        }
        else                      // Search Left
        {
            parent  = current;
            current = current->getLeftPtr();
        }
    }
    return false; // Target not found
}

// [ Remy's notes from Integration Purgatory ]
//   - Adjusted parameter type of the visit() function, and adjusted visit's argument
//     within the function body accordingly.
//   - Otherwise, looks solid, pending further testing.
//
//Inorder Traversal
template<class ItemType>
void BinarySearchTree<ItemType>::_inorder(BinaryNode<ItemType>* nodePtr) const
{
    if (nodePtr) // != NULL
    {
        _inorder(nodePtr->getLeftPtr());

        // Simple visit, though if one wants to be more intricate in the future,
        // admittedly, using a visit() function pointer would be more versatile.
        std::cout << nodePtr->getData();

        _inorder(nodePtr->getRightPtr());
    }
}


// [ Remy's notes from Final Integration Phase ]
// Function created to fulfill language query function, where the database will display all
// items that match the secondary key query in the BST.
//
// Difference from standard in-order traversal is that there is a conditional check before visit.
//
// The compare(lhs, rhs) function passed in from main() for this project returns these values:
//  lhs == rhs : return 0
//  lhs >  rhs : return 1
//  lhs <  rhs : return 2
//

//
template<class ItemType>
void BinarySearchTree<ItemType>::_inorder_query (BinaryNode<ItemType> *nodePtr,
                                                 const std::string    &query,
                                                 unsigned              compare(const ItemType&, const std::string&)) const
{
    if (nodePtr) // != NULL
    {
        _inorder_query(nodePtr->getLeftPtr(), query, compare);

        if (compare(nodePtr->getData(), query) == 0) // Case of a match, so we visit the node
            std::cout << nodePtr->getData();

        _inorder_query(nodePtr->getRightPtr(), query, compare);
    }

}
    
// [ Remy's notes from Integration Purgatory ]
//  - Adjusted the visit() function parameters
// Prints tree as an indented list.
// This is basically a special case of the in-order traversal, just using a different visit function.
// This might get rather chunky, depending on how deep the tree goes, and how wide one's terminal is.
//
template<class ItemType>
void BinarySearchTree<ItemType>::_printTree (void visit (const BinaryNode<ItemType>*, const unsigned), BinaryNode<ItemType> *nodePtr, const unsigned level) const
{
    if (isEmpty())
    {
        std::cout << "Tree is empty!" << std::endl;
        return;
    }

    if (!nodePtr) // Base case. I forgot this earlier which kept causing a segmentation fault TT. - Remy
        return;
        
    _printTree(visit, nodePtr->getRightPtr(), level+1);
    visit(nodePtr, level);
    _printTree(visit, nodePtr->getLeftPtr(), level+1);
}

#endif

