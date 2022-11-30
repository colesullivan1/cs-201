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
    keytype key;                    //  Key of the node
    FibHeapNode<keytype>* parent;   //  Parent of the node
    FibHeapNode<keytype>* child;    //  Child of the node
    FibHeapNode<keytype>* left;     //  Left sibling of the node
    FibHeapNode<keytype>* right;    //  Right sibling of the node
    int degree;                     //  Degree (number of children) of the node
    bool mark;                      //  Marked if the node has lost a child since the last time it was made a child of another node
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
        FibHeapNode<keytype>* head; //  Pointer to the head of the root list
        FibHeapNode<keytype>* tail; //  Pointer to the tail of the root list
        FibHeapNode<keytype>* min;  //  Pointer to the minimum node in the fib heap
        int numRoots;               //  Number of roots in the root list
        int numNodes;               //  Total number of nodes in the fib heap

        void initHeap();
        void initNode(FibHeapNode<keytype>* n, keytype k);

        void consolidate();
        void link(FibHeapNode<keytype>* y, FibHeapNode<keytype>* x);

        void insertNode(FibHeapNode<keytype>* n);

        void cut(FibHeapNode<keytype>* x, FibHeapNode<keytype>* y);
        void cascadingCut(FibHeapNode<keytype>* y);

        void print(FibHeapNode<keytype>* n);
};

//  Default constructor
template <typename keytype>
FibHeap<keytype>::FibHeap() {
    initHeap();
}

//  Overloaded constructor, constructs a fib heap from an array of keys and fills the handle array with pointers to the nodes
template <typename keytype>
FibHeap<keytype>::FibHeap(keytype k[], int s, CircularDynamicArray<FibHeapNode<keytype>*> &handle) {
    initHeap(); //  Initializes the heap

    //  Add keys in k[] to heap and add their handles to cda
    for (int i = 0; i < s; i++) {
        FibHeapNode<keytype>* node = new FibHeapNode<keytype>;
        initNode(node, k[i]);   //  Initializes the new node

        insertNode(node);   //  Inserts the new node into the heap
        handle.addEnd(node);    //  Adds the new node to the handle array
        numNodes++; //  Increments the number of nodes in the heap
    }

    consolidate();  //  Consolidates the heap to form binomial trees
}

//  Destructor
/*  Must be created despite being empty to avoid compiler errors
    Think this is because of C++'s default destructor */
template <typename keytype>
FibHeap<keytype>::~FibHeap() {
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
        numRoots = 0;
        numNodes = 0;
        return z->key;
    }

    //  Else if smallest key exists,
    if (z != nullptr) {
        // Add each child of z to root list and set each's parent to nullptr
        if (z->child != nullptr) {
            FibHeapNode<keytype>* curr = z->child;
            //  Iterate to the leftmost child of z
            while (curr->left != nullptr) curr = curr->left;
            //  Then, iterate back to the "head" of z's child list and add each child to root list
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
        //  Decrement number of roots in root list and number of nodes in fib heap
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
    initNode(node, k);  //  Initializes the new node

    insertNode(node);   //  Inserts the new node into the root list
    numNodes++; //  Increments the number of nodes in the heap

    return node;
}

//  Decreases key stored at handle h to k, returns true if successful and false otherwise
template <typename keytype>
bool FibHeap<keytype>::decreaseKey(FibHeapNode<keytype>* h, keytype k) {
    if (k > h->key)    return false;

    h->key = k; //  Decreases key stored at handle h to k

    //  If h is now smaller than its parent, calls cut() on h and its parent and cascadingCut() on its parent
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

    /*  Adds number of nodes in H2 to number of nodes in current fib heap
        and number of roots in H2 to number of roots in current fib heap  */
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
        //  Prints heap rooted at current root
        cout << "Rank " << root->degree << endl;
        print(root);
        cout << endl;

        //  Iterates to next root in root list
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
    int dN = log2(numNodes) + 1;    //  Maximum degree of any node in the heap
    for (int i = 0; i < dN; i++)  A.addEnd(nullptr);    //  Initializes A to have dN elements

    FibHeapNode<keytype>* next = head;
    for (int i = 0; i < numRoots; i++) {
        FibHeapNode<keytype>* x = next; //  x is the ith root in the root list
        next = next->left;              //  next is the (i+1)th root in the root list
        int d = x->degree;              //  d is the degree of x

        //  If there is already a root with the same degree as x, consolidates trees until there are no conflicting degrees
        while (A[d] != nullptr) {
            FibHeapNode<keytype>* y = A[d]; //  y is the root with the same degree as x

            //  If x's key is greater than y's key, swaps x and y
            if (x->key > y->key) {
                FibHeapNode<keytype>* temp = x;
                x = y;
                y = temp;
            }
            link(y, x); //  Makes y a child of x
            A[d] = nullptr; //  Removes y from A, as it is no longer a root
            d++;    //  Increments d, as x now has one more child
        }

        A[d] = x;   //  Adds x to A
    }

    //  Clears root list
    min = nullptr;
    head = nullptr;
    tail = nullptr;
    numRoots = 0;

    //  Adds roots stored in cda A to new, cleared root list
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
        /*  Case 1: x has no other children                     x
            Sets x's child to be y and y's parent to be x       |
                                                                y       */
        x->child = y;
        y->parent = x;
        y->left = nullptr;
        y->right = nullptr;
    } else {
        /*  Case 2: x already has at least one child                    x
            Adds y to end of x's child list and updates pointers       /|
                                                                      y-c       */
        FibHeapNode<keytype>* curr = x->child;
        while (curr->left != nullptr)   curr = curr->left;
        curr->left = y;
        y->parent = x;
        y->left = nullptr;
        y->right = curr;
    }
    x->degree++;    //  Increments x's degree, as it now has one more child
    y->mark = false;    //  Resets y's mark, as it is now a child of x
}

//  Inserts node n at end of root list
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
        if (y->mark == false)   y->mark = true; //  If y is unmarked, just mark it
        else {
            //  If y isn't the root and is marked, "cuts" y out of the tree and recurs up the tree
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
    //  If n has a child, recursively prints n's subtree
    if (curr->child != nullptr) {
        curr = curr->child;
        //  Iterate to the leftmost child in the child list
        while (curr->left != nullptr) {
            curr = curr->left;
        }
        //  Then, iterate back to the "head" of the child list and recursively call print on each node
        while (curr != nullptr) {
            print(curr);
            curr = curr->right;
        }
    }
}