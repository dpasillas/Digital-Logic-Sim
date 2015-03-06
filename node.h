#ifndef NODE_H
#define NODE_H
#include "direction.h"

/*/ Binary Tree Nodes /*/

class Node
{
public:
    Node(void* data);
    ~Node();
    Node*& child(Direction d);
    Direction childhood();
    Node* extreme(Direction d);

    Node *left, *right, *parent;
    void *data;
};

#endif // NODE_H
