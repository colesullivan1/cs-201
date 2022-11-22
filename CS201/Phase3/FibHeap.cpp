/*  Author:     Cole Sullivan
    Due Date:   November 22, 2022
    Class:      CS 201-002
    Assignment: Build a Fibonacci heap based on the methods described in the
                recorded lectures and in the Introduction to Algorithms textbook    */

#include <iostream>
#include <math.h>
using namespace std;

//  Fibonacci Heap Node Struct
template <typename keytype>
struct FibHeapNode {
    keytype key;
    FibHeapNode<keytype>* parent;
    FibHeapNode<keytype>* child;
    FibHeapNode<keytype>* left;
    FibHeapNode<keytype>* right;
    int degree;
    bool mark;
};

//  Fibonacci Heap Class
template <typename keytype>
class FibHeap {
    public:
        FibHeap();
        FibHeap(keytype k[], int s, CircularDynamicArray<FibHeapNode<keytype>*> &handle);
        ~FibHeap();

        keytype peekKey();
        keytype extractMin();

        FibHeapNode<keytype>* insert(keytype k);

        bool decreaseKey(FibHeapNode<keytype>* h, keytype k);

        void merge(FibHeap<keytype> &H2);

        void printKey();

    private:
        FibHeapNode<keytype>* head;
        FibHeapNode<keytype>* tail;
        FibHeapNode<keytype>* min;
        int numRoots;
        int numNodes;

        void initHeap();
        void initNode(FibHeapNode<keytype>* n, keytype k);

        void consolidate();
        void link(FibHeapNode<keytype>* y, FibHeapNode<keytype>* x);

        void insertNode(FibHeapNode<keytype>* n);

        void cut(FibHeapNode<keytype>* x, FibHeapNode<keytype>* y);
        void cascadingCut(FibHeapNode<keytype>* y);

        void print(FibHeapNode<keytype>* n);
};

//  Default Constructor
template <typename keytype>
FibHeap<keytype>::FibHeap() {
    initHeap();
}

//  Constructs new fib heap of size s using keys stored in k[], then stores every key's handle in cda &handle
template <typename keytype>
FibHeap<keytype>::FibHeap(keytype k[], int s, CircularDynamicArray<FibHeapNode<keytype>*> &handle) {
    initHeap();

    //  Add keys in k[] to heap and add their handles to cda
    for (int i = 0; i < s; i++) {
        FibHeapNode<keytype>* node = new FibHeapNode<keytype>;
        initNode(node, k[i]);

        insertNode(node);
        handle.addEnd(node);
        numNodes++;
    }

    consolidate();
}

//  Destructor
template <typename keytype>
FibHeap<keytype>::~FibHeap() {
    head = nullptr;
    tail = nullptr;
    min = nullptr;
    numRoots = 0;
    numNodes = 0;
}

//  Returns smallest key in fib heap
template <typename keytype>
keytype FibHeap<keytype>::peekKey() {
    return min->key;
}

//  Removes smallest key from fib heap and returns it
template <typename keytype>
keytype FibHeap<keytype>::extractMin() {
    FibHeapNode<keytype>* z = min;

    //  If smallest key was only key in fib heap, clear fib heap
    if (numNodes == 1) {
        head = nullptr;
        tail = nullptr;
        min = nullptr;
        numRoots--;
        numNodes--;
        return z->key;
    }

    //  Else if smallest key exists,
    if (z != nullptr) {
        // Add each child of z to root list and set each's parent to nullptr
        if (z->child != nullptr) {
            FibHeapNode<keytype>* curr = z->child;
            while (curr->left != nullptr) curr = curr->left;
            while (curr != nullptr) {
                FibHeapNode<keytype>* next = curr->right;
                insertNode(curr);
                curr = next;
            }
        }

        // Remove z from root list
        if (head == z) {
            head = head->left;
            head->right = nullptr;
        } else if (tail == z) {
            tail = tail->right;
            tail->left = nullptr;
        } else {
            z->left->right = z->right;
            z->right->left = z->left;
        }
        numRoots--;
        numNodes--;

        // If z was only node in root list, set min to nullptr
        if (numRoots == 0)  min = nullptr;
        else {
            // Else, set min to head (for now) and consolidate
            min = head;
            consolidate();
        }
    }

    return z->key;
}

//  Inserts key k into root list and returns key's handle
template <typename keytype>
FibHeapNode<keytype>* FibHeap<keytype>::insert(keytype k) {
    FibHeapNode<keytype>* node = new FibHeapNode<keytype>;
    initNode(node, k);

    insertNode(node);
    numNodes++;

    return node;
}

//  Decreases key stored at handle h to k, returns true if successful and false otherwise
template <typename keytype>
bool FibHeap<keytype>::decreaseKey(FibHeapNode<keytype>* h, keytype k) {
    if (k > h->key)    return false;

    //  If h is now smaller than its parent, calls cut() on h and its parent and cascadingCut() on its parent
    h->key = k;
    FibHeapNode<keytype>* y = h->parent;
    if (y != nullptr && h->key < y->key) {
        cut(h, y);
        cascadingCut(y);
    }

    if (h->key < min->key)  min = h;    //  If h is now the smallest key in the fib heap, h now becomes min

    return true;
}

//  Merges H2 into fib heap and consumes H2
template <typename keytype>
void FibHeap<keytype>::merge(FibHeap<keytype> &H2) {
    //  Appends H2's root list to end of current root list
    tail->left = H2.head;
    H2.head->right = tail;
    tail = H2.tail;

    //  If H2's smallest key is smaller than current smallest key, H2.min becomes min
    if (min == nullptr || (H2.min != nullptr && H2.min->key < min->key))  min = H2.min;

    numRoots += H2.numRoots;
    numNodes += H2.numNodes;

    //  Consumes H2
    H2.head = nullptr;
    H2.tail = nullptr;
    H2.min = nullptr;
    H2.numRoots = 0;
    H2.numNodes = 0;
}

//  Prints contents of fib heap
template <typename keytype>
void FibHeap<keytype>::printKey() {
    //  Calls print on each member of root list
    FibHeapNode<keytype>* root = head;
    while (numNodes != 0 && root != nullptr) {
        cout << "Rank " << root->degree << endl;
        print(root);
        cout << endl;
        if (root->left != nullptr) cout << endl;
        root = root->left;
    }
}

//  Initalizes root list
template <typename keytype>
void FibHeap<keytype>::initHeap() {
    head = new FibHeapNode<keytype>;
    head->parent = nullptr;
    head->child = nullptr;
    head->left = nullptr;
    head->right = nullptr;

    tail = head;
    min = head;

    numRoots = 0;
    numNodes = 0;
}

//  Initalizes new node n with key k
template <typename keytype>
void FibHeap<keytype>::initNode(FibHeapNode<keytype>* n, keytype k) {
    n->parent = nullptr;
    n->child = nullptr;
    n->left = nullptr;
    n->right = nullptr;
    n->key = k;
    n->degree = 0;
    n->mark = false;
}

//  Reduces number of trees in the root list until every root in the root list has a distinct degree
template <typename keytype>
void FibHeap<keytype>::consolidate() {
    //  Creates cda A to keep track of roots according to their degrees
    CircularDynamicArray<FibHeapNode<keytype>*> A;
    int dN = log2(numNodes) + 1;    //  D(n)
    for (int i = 0; i < dN; i++)  A.addEnd(nullptr);

    FibHeapNode<keytype>* next = head;
    for (int i = 0; i < numRoots; i++) {
        //  Sets x to ith root in root list
        FibHeapNode<keytype>* x = next;
        next = next->left;
        int d = x->degree;

        //  If there is already a root with the same degree as x, consolidates trees until there are no conflicting degrees
        while (A[d] != nullptr) {
            FibHeapNode<keytype>* y = A[d];

            if (x->key > y->key) {
                FibHeapNode<keytype>* temp = x;
                x = y;
                y = temp;
            }
            link(y, x);
            A[d] = nullptr;
            d++;
        }

        A[d] = x;   //  Adds x to A
    }

    //  Clears root list
    min = nullptr;
    head = nullptr;
    tail = nullptr;
    numRoots = 0;

    //  Adds roots stored in consolidated cda A to new, cleared root list
    for (int i = 0; i < dN; i++) {
        if (A[i] != nullptr)  insertNode(A[i]);
    }
}

//  Adds root y to root x's child list
template <typename keytype>
void FibHeap<keytype>::link(FibHeapNode<keytype>* y, FibHeapNode<keytype>* x) {
    //  Removes y from root list
    if (head == y) {
        /*  Case 1: y is the head of the root list
            Sets root to y's left to be new head    */
        head = head->left;
        head->right = nullptr;
    } else if (tail == y) {
        /*  Case 2: y is the tail of the root list
            Sets root to y's right to be new tail   */
        tail = tail->right;
        tail->left = nullptr;
    } else {
        /*  Case 3: y is neither the head nor the tail of the root list
            Sets root to y's right to now point to the root to y's left, and vice versa */
        y->left->right = y->right;
        y->right->left = y->left;
    }

    //  Adds y to x's child list
    if (x->child == nullptr) {
        /*  Case 1: x has no children                         x
            Sets x's child to be y and y's parent to be x       |
                                                                y           */
        x->child = y;
        y->parent = x;
        y->left = nullptr;
        y->right = nullptr;
    } else {
        /*  Case 2: x already has at least one child                  x
            Adds y to end of x's child list and updates pointers       /|
                                                                      y-c   */
        FibHeapNode<keytype>* curr = x->child;
        while (curr->left != nullptr)   curr = curr->left;
        curr->left = y;
        y->parent = x;
        y->left = nullptr;
        y->right = curr;
    }
    x->degree++;
    y->mark = false;
}

//  Inserts node n into root list
template <typename keytype>
void FibHeap<keytype>::insertNode(FibHeapNode<keytype>* n) {
    if (numRoots == 0) {
        /*  Case 1: root list is empty
            Set head, tail and min to n */
        n->parent = nullptr;
        n->left = nullptr;
        n->right = nullptr;
        head = n;
        tail = n;
        min = n;
    } else {
        /*  Case 2: root list has at least 1 root
            Set tail to n and update pointers   */
        n->parent = nullptr;
        n->left = nullptr;
        n->right = tail;
        tail->left = n;
        tail = n;
        if (n->key < min->key)  min = n;    //  If n is new smallest node, n becomes min
    }
    numRoots++;
}

//  "Cuts" the link between x and its parent, making x a root
template <typename keytype>
void FibHeap<keytype>::cut(FibHeapNode<keytype>* x, FibHeapNode<keytype>* y) {
    //  Remove x from y's child list
    if (x == y->child) {
        //  Case 1: x is head of y's child list
        if (x->left == nullptr) {
            //  Case 1.1: x is only node in y's child list
            y->child = nullptr;
        } else {
            //  Case 1.2: there is at least one other node in y's child list
            x->left->right = nullptr;
            y->child = x->left;
        }
    } else {
        //  Case 2: x is not the head of y's child list
        if (x->left == nullptr) {
            //  Case 2.1: x is tail of y's child list
            x->right->left = nullptr;
        } else {
            //  Case 2.2: x is somewhere in the middle of y's child list
            x->left->right = x->right;
            x->right->left = x->left;
        }
    }   y->degree--;

    insertNode(x);
    x->mark = false;
}

//  Recursively "cuts" its way up the tree until it finds either the root or an unmarked node
template <typename keytype>
void FibHeap<keytype>::cascadingCut(FibHeapNode<keytype>* y) {
    FibHeapNode<keytype>* z = y->parent;
    if (z != nullptr) { //  Checks if y is the root of the tree
        if (y->mark == false)   y->mark = true;
        else {
            //  If y isn't the root and isn't marked, "cuts" y out of the tree and recurs up the tree
            cut(y, z);
            cascadingCut(z);
        }
    }
}

//  Recursive helper function for print
template <typename keytype>
void FibHeap<keytype>::print(FibHeapNode<keytype>* n) {
    FibHeapNode<keytype>* curr = n;

    cout << curr->key << " ";
    //  Recursively traveres tree, calling print on each node it comes across
    if (curr->child != nullptr) {
        curr = curr->child;
        while (curr->left != nullptr) {
            curr = curr->left;
        }
        while (curr != nullptr) {
            print(curr);
            curr = curr->right;
        }
    }
}