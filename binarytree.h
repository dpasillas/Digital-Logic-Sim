#ifndef BINARYTREE_H
#define BINARYTREE_H

class Node;

/*/ Base class for BinarySearchTree /*/

class BinaryTree
{
public:
    BinaryTree();

    bool isEmpty();

    void print();
    int count();

protected:
    int count(Node* node);
    void swap(Node *n1, Node *n2);
    void print(Node* node);
    virtual
        void print(void* data) = 0;
    bool insert(void* data);
    void* find(void* data);
    void* findFirst(void* data);
    bool remove(void* data);

    virtual
        bool equals(void* data, Node* node) = 0;
    virtual
        bool equivalence(void *data, Node* node) = 0;
    virtual
        bool greaterThan(void* data, Node* node) = 0;
    virtual
        bool lessThan(void* data, Node* node) = 0;
    int depth(Node* node);
    int max(int i, int j);
    bool insert(void* data, Node *&node, Node *parent);
    void* find(void *data, Node *node);
    void* findFirst(void *data, Node *node, Node *parent);
    bool remove(Node *node);
    void rotate(Node *node);
    void balance(Node *node);


    Node *root, *current;
    void *currentData;
};

#endif // BINARYTREE_H
