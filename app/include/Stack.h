// Written by: Remy Dinh
//
// Stack class created to implement an undo-delete stack
// for records erased from the country database.
//
// The stack will hold the history of the session's
// deleted records, but will be wiped by using the function
// clear() if the state of the database is written to a file.
//

#ifndef STACK_H
#define STACK_H

#include <new> // for std::bad_alloc in event dynamic memory runs out.

template <typename ItemType>
class Stack
{
private:
    struct StackNode
    {
        ItemType   item;
        StackNode *next;
    }
    StackNode *top;
    unsigned   count;

public:
    Stack();
    ~Stack();

    bool       isEmpty  () const { return count == 0; }
    ItemType   peek     () const { return top->item; }
    unsigned   getCount () const { return count; }
    StackNode *pop      ();
    bool       push     (const ItemType &);
    bool       clear    ();
};

template <typename ItemType>
Stack<ItemType>::Stack()
{
    top   = nullptr;
    count = 0u;
}

// Dynamic memory needs to be cleaned up when all is said and done.
// Continually pops nodes off of the stack and deletes them unti
// it's empty.
template <typename ItemType>
Stack<ItemType>::~Stack()
{
    StackNode *durak;
    while (top != nullptr)
    {
        durak = pop();
        delete durak;
    }

}

template <typename ItemType>
bool Stack<ItemType>::smash()
{
    if (isEmpty())
        return false;

    StackNode *durak;
    while (top != nullptr)
    {
        durak = pop();
        delete durak;
    }
    return true;
}

template <typename ItemType>
StackNode *Stack<ItemType>::pop()
{
    if (isEmpty())
        return nullptr;

    StackNode *holder = top;
    
    top = top->next;
    count--;
    return holder;
}

template <typename ItemType>
bool Stack<ItemType>::push(const ItemType &input)
{
    try
    {
        StackNode* newNode = new StackNode;
    }
    catch(std::bad_alloc &ba)
    {
        std::cerr << "Error while attempting to push item onto delete stack." << std::endl;
        std::cerr << "Exception caught: " << ba.what() << std::endl;
        return false;
    }

    newNode->item = input;
    newNode->next = top;
              top = newNode;
    count++;

    return true;
}

#endif
