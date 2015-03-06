#include "node.h"

Node::Node(void* data)
{
    this->data = data;
    left = right = parent = 0;
}

Node::~Node()
{
    if(this->left)
        delete this->left;
    if(this->right)
        delete this->right;

}

Node*& Node::child(Direction d)
{
    if(d)
        return right;
    else
        return left;
}

Direction Node::childhood()
{
    if(parent)
    {
        if(this == parent->left)
            return LEFT;
        else
            return RIGHT;
    }
    else
        return LEFT;
}

Node* Node::extreme(Direction d)
{
    if(child(d))
        return child(d)->extreme(d);
    else
        return this;
}
