#include <iostream>
using namespace std;

template <typename keytype, typename valuetype>
class Node {
    public:
        keytype key;
        valuetype value;
        Node* left;
        Node* right;
        Node* parent;
        bool color;
        int size;

        Node();
        Node(keytype key, valuetype value);
        Node(bool isNil);
        ~Node();
        Node(const Node &clone);
        void operator=(const Node &clone);
};

template <typename keytype, typename valuetype>
Node<keytype, valuetype>::Node() {
    key = 0;
    value = 0;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    color = 1;
    size = 0;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>::Node(keytype k, valuetype v) {
    key = k;
    value = v;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    color = 1;
    size = 0;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>::Node(bool isNil) {
    key = 0;
    value = 0;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    color = 0;
    size = 0;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>::~Node() {
    delete left;
    delete right;
    delete parent;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>::Node(const Node &clone) {
    key = clone.key;
    value = clone.value;
    left = clone.left;
    right = clone.right;
    parent = clone.parent;
    color = clone.color;
    size = clone.size;
}

template <typename keytype, typename valuetype>
void Node<keytype, valuetype>::operator=(const Node &clone) {
    key = clone.key;
    value = clone.value;
    left = clone.left;
    right = clone.right;
    parent = clone.parent;
    color = clone.color;
    size = clone.size;
}

template<typename keytype, typename valuetype>
class RBTree {
    public:
        RBTree();
        RBTree(keytype k[], valuetype V[], int s);
        ~RBTree();
        RBTree(&clone);
        RBTree& operator=(RBTree &clone);

        void insert(keytype k, valuetype v);
        void insertFixup(Node* z);
        void leftRotate(Node* z);
        void rightRotate(Node* z);

    private:
        Node* head;
        Node* nil;
        int size;
};

template<typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree() {
    head = new Node;
    nil = new Node(true);

    head = nil;

    size = 0;
}

template<typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree(keytype k[], valuetype V[], int s) {
    head = new Node;
    nil = new Node(true);

    head = nil;

    size = 0;

    for (int i = 0; i < s; i++) insert(k[i], V[i]);
}

template<typename keytype, typename valuetype>
RBTree<keytype, valuetype>::~RBTree() {
    delete head;
    delete nil;
}

template<typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::insert(keytype k, valuetype v) {
    Node* x = new Node;
    Node* y = new Node;
    Node* z = new Node;

    x = head;
    y = nil;

    z->key = k;
    z->value = v;

    while (x != nil) {
        y = x;
        if (k < x->key) x = x->left;
        else    x = x->right;
    }

    z->parent = y;

    if (y == nil)   head = z;
    else if (z->key < y->key)   y->left = z;
    else    y->right = z;

    z->left = nil;
    z->right = nil;
    z->color = 1;

    insertFixup(z);
    size++;
}

template<typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::insertFixup(Node* z) {
    Node* y = new Node;

    while (z->parent->color == 1) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;

            if (y->color == 1) {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }

                z->parent->color = 0;
                z->parent->parent->color = 1;
                rightRotate(z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;

            if (y->color == 1) {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }

                z->parent->color = 0;
                z->parent->parent->color = 1;
                leftRotate(z->parent->parent);
            }
        }
    }

    head->color = 0;
}

template<typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::leftRotate(Node* z) {
    Node* y = new Node;

    y = z->right;
    z->right = y->left;

    if (y->left != nil) y->left->parent = z;
    y->parent = z->parent;

    if (z->parent == nil)   head = y;
    else if (z == z->parent->left)  z->parent->left = y;
    else    z->parent->right = y;

    y->left = z;
    z->parent = y;

    y->size = z->size;
    z->size = z->left->size + z->right->size + 1;
}

template<typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::rightRotate(Node* z) {
    Node* y = new Node;

    y = z->left;
    z->left = y->right;

    if (y->right != nil) y->right->parent = z;
    y->parent = z->parent;

    if (z->parent == nil)   head = y;
    else if (z == z->parent->right)  z->parent->right = y;
    else    z->parent->left = y;

    y->right = z;
    z->parent = y;

    y->size = z->size;
    z->size = z->left->size + z->right->size + 1;
}