//             Coder: Shun Furuya
// Style Adjustments: Remy Dinh

#ifndef _HASH_NODE
#define _HASH_NODE

#include "LinkedList.h"

template<class ItemType>
class HashNode
{
private:
    LinkedList<ItemType> list;
    int                  occupied;
    int                  noCollisions;

public:
    HashNode()                                   {occupied = 0; noCollisions = 0;}
    HashNode(ItemType anItem)                    {list.insertNode(anItem); occupied = 1; noCollisions = 0;}
    HashNode(ItemType anItem, int ocp, int nCol) {list.insertNode(anItem); occupied = ocp; noCollisions = nCol;}
    
    // setters
    void setItem         (const ItemType &anItem) {list.insertNode(anItem);}
    void setOccupied     (int ocp)                {occupied = ocp;}
    void setNoCollisions (int nCol)               {noCollisions = nCol;}
    
    // getters
    LinkedList<ItemType> getList         () const {return list;}
    int                  getOccupied     () const {return occupied;}
    int                  getNoCollisions () const {return noCollisions;}
};

#endif
