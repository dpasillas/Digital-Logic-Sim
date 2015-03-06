#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "binarytree.h"
#include "node.h"
#include <QQueue>

/*/ This is a binary search tree with added functionality as a priority queue
    Only pointers of any T object are stored, so any stored object can be modified from this tree
    Stored objects ARE NOT COPIED

    any stored object must support the following operators: <, >, ==, T() (the copy constructor),
    and QDebug << (for now) /*/
template <typename T>
class BinarySearchTree : public BinaryTree
{
public:
    typedef void (T::*VFUNC)();

    BinarySearchTree() : BinaryTree()
    {
        func = 0;
        nullT = new T();
        deleteOnRemove = false;
    }

    /*/ if an object is removed, this flag determines whether or not it is deleted /*/
    void setDeleteOnRemove(bool b)
    {
        deleteOnRemove = b;
    }


    BinarySearchTree(BinarySearchTree<T> &bst)
    {
        copyNode(root, bst.root);
        func = bst.func;
        nullT = new T();
        deleteOnRemove = bst.deleteOnRemove;
    }

    /*/ Stores a pointer to a function /*/
    void setExec(VFUNC fptr)
    {
        func = fptr;
    }

    /*/ Calls stored function on every stored object /*/
    void execute()
    {
        if(root)
            execute(func, root);
    }

    /*/ Calls fptr on every stored object /*/
    void execute(VFUNC fptr)
    {
        if(root)
            execute(fptr, root);
    }

    /*/ Calls the selected function only on the "first" stored object /*/
    void executeFirst(VFUNC fptr)
    {
        Node* n = 0;
        void* d = 0;
        if(root)
        {
            n = root->extreme(LEFT);
            d = n->data;
            BinaryTree::remove(n);

            ((T*)(d)->*fptr)();
        }
    }

    /*/ Returns a pointer to the "first" stored object /*/
    T* peek()
    {
        if(root)
            return (T*)(root->extreme(LEFT)->data);
        else
            return 0;
    }

    /*/ Inserts a T object by reference /*/
    void insert(T& t)
    {
        BinaryTree::insert((void*)&t);
    }

    void insert(T *t)
    {
        insert(*t);
    }

    T& find(T& t)
    {
        if(BinaryTree::find((void*)&t))
            return *(T*)(current->data);
        else return *nullT;
    }

    T& find(T *t)
    {
        return find(*t);
    }

    /*/ Finds an object,  /*/
    T& findFirst(T& t)
    {
        if(BinaryTree::findFirst((void*)&t))
            return *(T*)(current->data);
        else return *nullT;
    }

    T& findFirst(T *t)
    {
        return findFirst(*t);
    }

    T& first()
    {
        if(root)
            return *(T*)(root->extreme(LEFT)->data);
        else
            return *nullT;
    }

    bool remove(T& t)
    {
        find(t);
        if(!current)
            return false;

        BinaryTree::remove(current);
        if(deleteOnRemove)
            delete (T*)currentData;
        return true;
    }

    bool remove(T *t)
    {
        return (remove(*t))?true:false;
    }

    void removeAll()
    {
        while(root)
            BinaryTree::remove(root);
    }
    
    void print()
    {
       //qDebug() << "IN_ORDER";
        BinaryTree::print();
       //qDebug() << "END_IN_ORDER";
    }

    void levelOrderPrint()
    {
        if(!root)
            return;
       //qDebug() << "LEVEL_ORDER";
        QQueue<Node*> q;
        q.enqueue(root);
        while(!q.isEmpty())
        {
            Node* node = q.dequeue();
            print(node->data);
            if(node->left)
                q.enqueue(node->left);
            if(node->right)
                q.enqueue(node->right);
        }
       //qDebug() << "END_LEVEL_ORDER";
    }

protected:

    void print(void* data)
    {
       qDebug() << *(T*)data;
    }

    void execute(VFUNC fptr, Node *node)
    {
        if(node->left)
            execute(fptr, node->left);
        ((T*)(node->data)->*fptr)();
        if(node->right)
            execute(fptr, node->right);
    }

    void copyNode(Node *&destination, Node *&source)
    {

        destination = new Node(new T(*(T*)source->data));
        if(source->left)
        {
            copyNode(destination->left, source->left);
            destination->left->parent = destination;
        }
        if(source->right)
        {
            copyNode(destination->right, source->right);
            destination->right->parent = destination;
        }
    }

    //A secondary, looser equivalence function
    bool equivalence(void *data, Node *node)
    {
        return *(T*)data % *(T*)node->data;
    }

    bool equals(void *data, Node *node)
    {
        return *(T*)data == *(T*)node->data;
    }

    bool lessThan(void *data, Node *node)
    {
         return *(T*)data < *(T*)(node->data);
    }

    bool greaterThan(void *data, Node *node)
    {
         return *(T*)data > *(T*)node->data;
    }

private:
    VFUNC func;
    T *nullT;
    bool deleteOnRemove;
};

#endif // BINARYSEARCHTREE_H
