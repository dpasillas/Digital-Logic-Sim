#include "binarytree.h"
#include "node.h"
//#include <QDebug>

BinaryTree::BinaryTree()
{
    root = current = 0;
    currentData = 0;
}

void BinaryTree::print()
{
    print(root);
}

void BinaryTree::print(Node *node)
{
    if(!node)
        return;
    print(node->left);
    print(node->data);
    print(node->right);
}

bool BinaryTree::insert(void* data)
{
    return insert(data, root, 0);
}

bool BinaryTree::insert(void* data, Node *&node, Node *parent)
{
    if(!node)
    {
        node = new Node(data);
        node->parent = parent;
        if(!parent)
            root = node;
        balance(node);
        return true;
    }
    else
    {
        if(equals(data, node))
            return false;
        if(lessThan(data, node))
            return insert(data, node->left, node);
        else
            return insert(data, node->right, node);
    }
}
void* BinaryTree::find(void* data)
{
    current = 0;
    return find(data, root);
}

void* BinaryTree::find(void* data, Node *node)
{
    if(!node)
        return 0;
    if(equals(data, node))
    {
        current = node;
        return node->data;
    }
    if(lessThan(data, node))
        return find(data, node->left);
    else
        return find(data, node->right);
}

void* BinaryTree::findFirst(void* data)
{
    current = 0;
    return findFirst(data, root, 0);
}

void* BinaryTree::findFirst(void *data, Node *node, Node *parent)
{
    if(!node)
    {
        current = parent;
        return parent->data;
    }
    if(equivalence(data, node))
    {
        current = node;
        return node->data;
    }
    if(lessThan(data, node))
        return findFirst(data, node->left, node);
    else
        return findFirst(data, node->right, node);
}

bool BinaryTree::remove(void *data)
{
    if(find(data))
        return remove(current);
    else
        return false;
}

bool BinaryTree::remove(Node *node)
{
    Node *leftRightmost = (node->left)?node->left->extreme(RIGHT):0,
         *rightLeftmost = (node->right)?node->right->extreme(LEFT):0,
         *target = (leftRightmost)?leftRightmost:rightLeftmost,
         *targetChild = (target)?(target->left)?target->left:target->right:0;
    //since target is an extreme, it has at most one child

    if(target)
    {
        swap(node, target); //data is swapped
        target->parent->child(target->childhood()) = targetChild;
        if(targetChild)
        {
            targetChild->parent = target->parent;
        }
    }
    else
    {
        target = node;
        if(node == root)
            root = 0;
        else
            node->parent->child(node->childhood()) = 0;
    }
    target->parent = target->left = target->right = 0;
    delete target;
    return true;
}

int BinaryTree::depth(Node* node)
{
    if(node)
        return 1 + max(depth(node->left), depth(node->right));
    else
        return 0;
}

int BinaryTree::max(int i, int j)
{
    return(i > j)?i:j;
}

void BinaryTree::rotate(Node* node)
{
    if(node == root)
        return;
    Direction o = node->childhood(), d = (o)?LEFT:RIGHT;
    Node *parent = node->parent, *pParent = parent->parent, *dChild = node->child(d);

    node->parent = pParent;
    if(pParent)
        pParent->child(parent->childhood()) = node;

    parent->parent = node;
    node->child(d) = parent;

    parent->child(o) = dChild;
    if(dChild)
        dChild->parent = parent;

    if(parent == root)
    {
        root = node;
    }
}
void BinaryTree::balance(Node* node)
{
    if(!node)
        return;
    int left = depth(node->left),
        right = depth(node->right),
        leftLeft = (left)?depth(node->left->left):0,
        leftRight = (left)?depth(node->left->right):0,
        rightLeft = (right)?depth(node->right->left):0,
        rightRight = (right)?depth(node->right->right):0,
        balanceFactor = right - left;

    if(balanceFactor < -1) // left-heavy
    {
        if(leftRight - leftLeft > 0) // left-right-heavy
            rotate(node->left->right);
        rotate(node->left);
        node = node->parent; //after the rotation, node's parent holds node's original position
                             //therefore, node's parent doesn't need to be balanced
    }
    else if(balanceFactor > 1) // right-heavy
    {
        if(rightRight - rightLeft < 0) // right-left-heavy
            rotate(node->right->left);
        rotate(node->right);
        node = node->parent;
    }

    balance(node->parent); //any node that needs to be balanced due to an insertion/removal will
                           //only reside on the path back to the root, thus only node's parents
                           //need to be checked for balancing issues.
}

bool BinaryTree::isEmpty()
{
    return !root;
}

void BinaryTree::swap(Node *n1, Node *n2)
{
    void *temp = n1->data;
    n1->data = n2->data;
    n2->data = temp;
}

int BinaryTree::count()
{
    return count(root);
}

int BinaryTree::count(Node *node)
{
    return (node)?count(node->left) + count(node->right) + 1:0;
}
