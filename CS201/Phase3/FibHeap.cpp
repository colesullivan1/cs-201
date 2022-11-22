/*  Author:     Cole Sullivan
    Due Date:   November 22, 2022
    Class:      CS 201-002
    Assignment: Build a Fibonacci heap based on the methods described in the recorded lectures  */

#ifndef Included_CircularDynamicArray
#define Included_CircularDynamicArray
#include "CircularDynamicArray.cpp"
#endif

#include <iostream>
#include <math.h>
using namespace std;

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

template <typename keytype>
FibHeap<keytype>::FibHeap() {
    initHeap();
}

template <typename keytype>
FibHeap<keytype>::FibHeap(keytype k[], int s, CircularDynamicArray<FibHeapNode<keytype>*> &handle) {
    //  Initialize heap
    initHeap();

    //  Add keys in k[] to heap and add their handles to cda
    for (int i = 0; i < s; i++) {
        FibHeapNode<keytype>* node = new FibHeapNode<keytype>;
        initNode(node, k[i]);

        insertNode(node);
        handle.addEnd(node);
        numNodes++;
    }

    consolidate();  //  Form binomial trees
}

template <typename keytype>
FibHeap<keytype>::~FibHeap() {
    head = nullptr;
    tail = nullptr;
    min = nullptr;
    numRoots = 0;
    numNodes = 0;
}

template <typename keytype>
keytype FibHeap<keytype>::peekKey() {
    return min->key;
}

template <typename keytype>
keytype FibHeap<keytype>::extractMin() {
    FibHeapNode<keytype>* z = min;

    if (numNodes == 1) {
        head = nullptr;
        tail = nullptr;
        min = nullptr;
        numRoots--;
        numNodes--;
        return z->key;
    }

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

template <typename keytype>
FibHeapNode<keytype>* FibHeap<keytype>::insert(keytype k) {
    FibHeapNode<keytype>* node = new FibHeapNode<keytype>;
    initNode(node, k);

    insertNode(node);
    numNodes++;

    return node;
}

template <typename keytype>
bool FibHeap<keytype>::decreaseKey(FibHeapNode<keytype>* h, keytype k) {
    if (k > h->key)    return false;

    h->key = k;
    FibHeapNode<keytype>* y = h->parent;

    if (y != nullptr && h->key < y->key) {
        cut(h, y);
        cascadingCut(y);
    }

    if (h->key < min->key)  min = h;

    return true;
}

template <typename keytype>
void FibHeap<keytype>::merge(FibHeap<keytype> &H2) {
    tail->left = H2.head;
    H2.head->right = tail;
    tail = H2.tail;

    if (min == nullptr || (H2.min != nullptr && H2.min->key < min->key))  min = H2.min;

    numRoots += H2.numRoots;
    numNodes += H2.numNodes;

    H2.head = nullptr;
    H2.tail = nullptr;
    H2.min = nullptr;
    H2.numRoots = 0;
    H2.numNodes = 0;
}

template <typename keytype>
void FibHeap<keytype>::printKey() {
    FibHeapNode<keytype>* curr = head;
    while (numNodes != 0 && curr != NULL) {
        cout << "Rank " << curr->degree << endl;
        print(curr);
        cout << endl;
        if (curr->left != nullptr) cout << endl;
        curr = curr->left;
    }
}

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

template <typename keytype>
void FibHeap<keytype>::consolidate() {
    CircularDynamicArray<FibHeapNode<keytype>*> A;
    int dN = log2(numNodes) + 1;

    for (int i = 0; i < dN; i++)  A.addEnd(nullptr);

    FibHeapNode<keytype>* next = head;
    for (int i = 0; i < numRoots; i++) {
        FibHeapNode<keytype>* x = next;
        next = next->left;
        int d = x->degree;

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

        A[d] = x;
    }

    min = nullptr;
    head = nullptr;
    tail = nullptr;
    numRoots = 0;

    for (int i = 0; i < dN; i++) {
        if (A[i] != nullptr)  insertNode(A[i]);
    }
}

template <typename keytype>
void FibHeap<keytype>::link(FibHeapNode<keytype>* y, FibHeapNode<keytype>* x) {
    if (head == y) {
        head = head->left;
        head->right = nullptr;
    } else if (tail == y) {
        tail = tail->right;
        tail->left = nullptr;
    } else {
        y->left->right = y->right;
        y->right->left = y->left;
    }

    if (x->child == nullptr) {
        x->child = y;
        y->parent = x;
        y->left = nullptr;
        y->right = nullptr;
    } else {
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

template <typename keytype>
void FibHeap<keytype>::insertNode(FibHeapNode<keytype>* n) {
    if (numRoots == 0) {
        n->parent = nullptr;
        n->left = nullptr;
        n->right = nullptr;
        head = n;
        tail = n;
        min = n;
    } else {
        n->parent = nullptr;
        n->left = nullptr;
        n->right = tail;
        tail->left = n;
        tail = n;
        if (n->key < min->key)  min = n;
    }
    numRoots++;
}

template <typename keytype>
void FibHeap<keytype>::cut(FibHeapNode<keytype>* x, FibHeapNode<keytype>* y) {
    //  Remove x from y's child list
    if (x == y->child) {
        if (x->left == nullptr) y->child = nullptr;
        else {
            x->left->right = nullptr;
            y->child = x->left;
        }
    } else {
        if (x->left == nullptr) {
            x->right->left = nullptr;
        } else {
            x->left->right = x->right;
            x->right->left = x->left;
        }
    }   y->degree--;

    insertNode(x);
    x->mark = false;
}

template <typename keytype>
void FibHeap<keytype>::cascadingCut(FibHeapNode<keytype>* y) {
    FibHeapNode<keytype>* z = y->parent;
    if (z != nullptr) {
        if (y->mark == false)   y->mark = true;
        else {
            cut(y, z);
            cascadingCut(z);
        }
    }
}

template <typename keytype>
void FibHeap<keytype>::print(FibHeapNode<keytype>* n) {
    FibHeapNode<keytype>* curr = n;

    cout << curr->key << " ";
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