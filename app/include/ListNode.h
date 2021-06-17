#ifndef LISTNODE_H
#define LISTNODE_H

/*
 * Written by: Remy Dinh
 * Replaces old Node struct in LinkedList and Stack class templates
 * so that both the undoStack and coreDataList in main() can use
 * a unified ListNode class template.
 */

template <typename ItemType>
class ListNode
{
private:
    ItemType            item;
    ListNode<ItemType> *next;
public:
    ListNode() {}
    ListNode(const ItemType &itemIn, ListNode<ItemType> *nextItemPtr): item{itemIn}, next{nextItemPtr} {}
    ~ListNode() {}

    // Accessors
    void      setItem (ItemType input)  { item = input; }
    void      setNext (ListNode *input) { next = input; }
        
    ItemType  getItem () const { return item; }
    ListNode* getNext () const { return next; }
};

#endif // ListNode.h
