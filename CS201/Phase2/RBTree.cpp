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
        Node(const Node &r);
        Node& operator=(const Node &r);

        bool operator==(const Node &r);
        bool operator!=(const Node &r);
};

template <typename keytype, typename valuetype>
Node<keytype, valuetype>::Node() {
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
    left = this;
    right = this;
    parent = this;
    color = 1;
    size = 0;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>::Node(bool isNil) {
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    color = 0;
    size = 0;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>::~Node() { 
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>::Node(const Node &r) {
    if (this != &r) {
        key = r.key;
        value = r.value;
        left = r.left;
        right = r.right;
        parent = r.parent;
        color = r.color;
        size = r.size;
    }
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>& Node<keytype, valuetype>::operator=(const Node &r) {
    if (this != &r) {
        key = r.key;
        value = r.value;
        left = r.left;
        right = r.right;
        parent = r.parent;
        color = r.color;
        size = r.size;
    }
    return *this;
}

template<typename keytype, typename valuetype>
class RBTree {
    public:
        RBTree();
        RBTree(keytype k[], valuetype V[], int s);
        ~RBTree();
        RBTree(const RBTree &r);
        RBTree& operator=(const RBTree &r);

        valuetype* search(keytype k);

        void insert(keytype k, valuetype v);
        int remove(keytype k);

        int rank(keytype k);
        keytype select(int pos);

        keytype* successor(keytype k);
        keytype* predecessor(keytype k);

        int size();

        void preorder();
        void inorder();
        void postorder();

        void printk(int k);
    private:
        Node<keytype, valuetype>* head;
        Node<keytype, valuetype>* nil;
        int numNodes;

        Node<keytype, valuetype>* copy(Node<keytype, valuetype>* x);
        void nuke(Node<keytype, valuetype>* x);

        valuetype* searchRecursive(Node<keytype, valuetype>* x, keytype k);

        void insertFixup(Node<keytype, valuetype>* z);
        void leftRotate(Node<keytype, valuetype>* x);
        void rightRotate(Node<keytype, valuetype>* x);

        void removeNode(Node<keytype, valuetype>* z);
        void transplant(Node<keytype, valuetype>* u, Node<keytype, valuetype>* v);
        void deleteFixup(Node<keytype, valuetype>* x);

        keytype selectRecursive(Node<keytype, valuetype>* x, int i);
        int sizeOf(Node<keytype, valuetype>* x);

        Node<keytype, valuetype>* findSuccessor(Node<keytype, valuetype>* x);
        Node<keytype, valuetype>* findPredecessor(Node<keytype, valuetype>* x);
        Node<keytype, valuetype>* getMin(Node<keytype, valuetype>* x);
        Node<keytype, valuetype>* getMax(Node<keytype, valuetype>* x);

        void printPreorder(Node<keytype, valuetype>* x);
        void printInorder(Node<keytype, valuetype>* x);
        void printPostorder(Node<keytype, valuetype>* x);
};

template<typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree() {
    head = new Node<keytype, valuetype>;
    nil = new Node<keytype, valuetype>(true);

    head = nil;

    numNodes = 0;
}

template<typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree(keytype k[], valuetype V[], int s) {
    head = new Node<keytype, valuetype>;
    nil = new Node<keytype, valuetype>(true);

    head = nil;

    numNodes = 0;

    for (int i = 0; i < s; i++) insert(k[i], V[i]);
}

template<typename keytype, typename valuetype>
RBTree<keytype, valuetype>::~RBTree() {
    nuke(head);
}

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree(const RBTree &r) {
    head = new Node<keytype, valuetype>;
    nil = new Node<keytype, valuetype>;
    
    head = r.nil;
    nil = r.nil;
    
    numNodes = r.numNodes;

    head = copy(r.head);
}

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>& RBTree<keytype, valuetype>::operator=(const RBTree &r) {
    head = new Node<keytype, valuetype>;
    nil = new Node<keytype, valuetype>;

    head = r.nil;
    nil = r.nil;

    numNodes = r.numNodes;

    head = copy(r.head);

    return *this;
}

template <typename keytype, typename valuetype>
valuetype* RBTree<keytype, valuetype>::search(keytype k) {
    return searchRecursive(head, k);
}

template<typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::insert(keytype k, valuetype v) {
    Node<keytype, valuetype>* x = new Node<keytype, valuetype>;
    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;
    Node<keytype, valuetype>* z = new Node<keytype, valuetype>;

    x = head;
    y = nil;

    z->key = k;
    z->value = v;

    while (x != nil) {
        y = x;
        if (k < x->key) {
            x->size++;
            x = x->left;
        } else {
            x->size++;
            x = x->right;
        }
    }

    z->parent = y;

    if (y == nil)   head = z;
    else if (z->key < y->key)   y->left = z;
    else    y->right = z;

    z->left = nil;
    z->right = nil;
    z->size = 1;
    z->color = 1;

    insertFixup(z);
    numNodes++;
}

template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::remove(keytype k) {
    Node<keytype, valuetype>* x = new Node<keytype, valuetype>;
    x = head;

    while (x != nil) {
        if (x->key == k)        break;
        else if (k < x->key)    x = x->left;
        else                    x = x->right;
    }

    if (x->key == k) {
        numNodes--;
        removeNode(x);
        return 1;
    } else  return 0;
}

template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::rank(keytype k) {
    Node<keytype, valuetype>* x = new Node<keytype, valuetype>;
    x = head;

    while (x != nil) {
        if (x->key == k)        break;
        else if (k < x->key)    x = x->left;
        else                    x = x->right;
    }

    if (x == nil)   return 0;

    int r = x->left->size + 1;

    while (x != head) {
        if (x == x->parent->right)  r = r + x->parent->left->size + 1;
        x = x->parent;
    }

    return r;
}

template <typename keytype, typename valuetype>
keytype RBTree<keytype, valuetype>::select(int pos) {
    return selectRecursive(head, pos);
}

template <typename keytype, typename valuetype>
keytype* RBTree<keytype, valuetype>::successor(keytype k) {
    Node<keytype, valuetype>* x = new Node<keytype, valuetype>;
    x = head;

    while (x != nil) {
        if (x->key == k)        break;
        else if (k < x->key)    x = x->left;
        else                    x = x->right;
    }

    if (x == getMax(head))      return NULL;

    if (x != nil) {
        x = findSuccessor(x);
        return &(x->key);
    } else  return NULL;
}

template <typename keytype, typename valuetype>
keytype* RBTree<keytype, valuetype>::predecessor(keytype k) {
    Node<keytype, valuetype>* x = new Node<keytype, valuetype>;
    x = head;

    while (x != nil) {
        if (x->key == k)        break;
        else if (k < x->key)    x = x->left;
        else                    x = x->right;
    }

    if (x == getMin(head))      return NULL;

    if (x != nil) {
        x = findPredecessor(x);
        return &(x->key);
    } else  return NULL;
}

template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::size() {
    return numNodes;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::preorder() {
    printPreorder(head);
    cout << endl;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::inorder() {
    printInorder(head);
    cout << endl;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::postorder() {
    printPostorder(head);
    cout << endl;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::printk(int k) {
    for (int i = 1; i <= k; i++)    cout << select(i) << " ";
    cout << endl;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::copy(Node<keytype, valuetype>* x) {    
    if (x == nil)   return nil;
    
    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;
    y->key = x->key;
    y->value = x->value;
    y->left = x->left;
    y->right = x->right;
    y->parent = x->parent;
    y->color = x->color;
    y->size = x->size;

    if (x->left != nil)     y->left = copy(x->left);
    if (x->right != nil)    y->right = copy(x->right);
    
    return y;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::nuke(Node<keytype, valuetype>* x) {
    if (x == nil)   return;

    nuke(x->left);
    nuke(x->right);
    delete x;
}

template <typename keytype, typename valuetype>
valuetype* RBTree<keytype, valuetype>::searchRecursive(Node<keytype, valuetype>* x, keytype k) {
    if (x == nil)   return NULL;

    if (x->key == k)        return &(x->value);
    else if (x->key < k)    return searchRecursive(x->right, k);
    else                    return searchRecursive(x->left, k);
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::insertFixup(Node<keytype, valuetype>* z) {
    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;

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

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::leftRotate(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;

    y = x->right;
    x->right = y->left;

    if (y->left != nil) y->left->parent = x;
    y->parent = x->parent;

    if (x->parent == nil)           head = y;
    else if (x == x->parent->left)  x->parent->left = y;
    else                            x->parent->right = y;

    y->left = x;
    x->parent = y;

    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::rightRotate(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;

    y = x->left;
    x->left = y->right;

    if (y->right != nil) y->right->parent = x;
    y->parent = x->parent;

    if (x->parent == nil)           head = y;
    else if (x == x->parent->right) x->parent->right = y;
    else                            x->parent->left = y;

    y->right = x;
    x->parent = y;

    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::removeNode(Node<keytype, valuetype>* z) {
    Node<keytype, valuetype>* x = new Node<keytype, valuetype>;
    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;
    Node<keytype, valuetype>* n = new Node<keytype, valuetype>;

    y = z;
    bool originalColor = y->color;

    n = findPredecessor(z);
    while (n != nil) {
        n->size--;
        n = n->parent;
    }
    
    if (z->left == nil) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nil) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = getMax(z->left);
        y->size = z->size - 1;

        originalColor = y->color;
        x = y->right;

        if (y->parent == z) x->parent = y;
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (originalColor == 0) deleteFixup(x);

    y = head;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::transplant(Node<keytype, valuetype>* u, Node<keytype, valuetype>* v) {
    if (u->parent == nil)           head = v;
    else if (u == u->parent->left)  u->parent->left = v;
    else                            u->parent->right = v;

    v->parent = u->parent;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::deleteFixup(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* w = new Node<keytype, valuetype>;

    while (x != head && x->color == 0) {
        if (x == x->parent->left) {
            w = x->parent->right;

            if (w->color == 1) {
                w->color = 0;
                x->parent->color = 1;
                leftRotate(x->parent);
                w = x->parent->right;
            }

            if (w->left->color == 0 && w->right->color == 0) {
                w->color = 1;
                x = x->parent;
            } else {
                if (w->right->color == 0) {
                    w->left->color = 0;
                    w->color = 1;
                    rightRotate(w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = 0;
                w->right->color = 0;
                leftRotate(x->parent);
                x = head;
            }
        } else {
            w = x->parent->left;

            if (w->color == 1) {
                w->color = 0;
                x->parent->color = 1;
                rightRotate(x->parent);
                w = x->parent->left;
            }

            if (w->left->color == 0 && w->right->color == 0) {
                w->color = 1;
                x = x->parent;
            } else {
                if (w->left->color == 0) {
                    w->right->color = 0;
                    w->color = 1;
                    leftRotate(w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = 0;
                w->left->color = 0;
                rightRotate(x->parent);
                x = head;
            }
        }
    }

    x->color = 0;
}

template <typename keytype, typename valuetype>
keytype RBTree<keytype, valuetype>::selectRecursive(Node<keytype, valuetype>* x, int i) {
    int r = x->left->size + 1;

    if (i == r)         return x->key;
    else if (i < r)     return selectRecursive(x->left, i);
    else                return selectRecursive(x->right, i - r);
}

template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::sizeOf(Node<keytype, valuetype>* x) {
    if (x == nil)   return 0;
    else            return x->left->size + x->right->size + 1;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::findSuccessor(Node<keytype, valuetype>* x) {
    if (x->right != nil)    return getMin(x->right);

    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;
    y = x->parent;

    while (y != nil && x == y->right) {
        x = y;
        y = y->parent;
    }

    return y;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::findPredecessor(Node<keytype, valuetype>* x) {
    if (x->left != nil) return getMax(x->left);

    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;
    y = x->parent;

    while (y != nil && x == y->left) {
        x = y;
        y = y->parent;
    }

    return y;
}


template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::getMin(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;
    y = x;

    while (y->left != nil)  y = y->left;
    
    return y;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::getMax(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;
    y = x;

    while (y->right != nil) y = y->right;

    return y;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::printPreorder(Node<keytype, valuetype>* x) {
    if (x == nil)   return;

    cout << x->key << " ";
    printPreorder(x->left);
    printPreorder(x->right);
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::printInorder(Node<keytype, valuetype>* x) {
    if (x == nil)   return;

    printInorder(x->left);
    cout << x->key << " ";
    printInorder(x->right);
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::printPostorder(Node<keytype, valuetype>* x) {
    if (x == nil)   return;

    printPostorder(x->left);
    printPostorder(x->right);
    cout << x->key << " ";
}