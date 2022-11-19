/*  Author:     Cole Sullivan
    Due Date:   October 20, 2022
    Class:      CS 201-002
    Assignment: Build a red-black tree based on the methods described in the
                Introduction to Algorithms textbook                       */

#include <iostream>
using namespace std;

//  Node Class
template <typename keytype, typename valuetype>
class Node {
    public:
        keytype key;
        valuetype value;
        Node* left;
        Node* right;
        Node* parent;
        bool color; // 0 => black, 1 => red
        int size;

        Node();
        Node(keytype key, valuetype value);
        Node(bool isNil);
        ~Node();
        Node(const Node &r);
        Node& operator=(const Node &r);
};

//  Default constructor for Node
template <typename keytype, typename valuetype>
Node<keytype, valuetype>::Node() {
    left = NULL;
    right = NULL;
    parent = NULL;
    color = 1;
    size = 0;
}

//  Overloaded constructor for Node, constructs Node with key k and value v
template <typename keytype, typename valuetype>
Node<keytype, valuetype>::Node(keytype k, valuetype v) {
    key = k;
    value = v;
    left = NULL;
    right = NULL;
    parent = NULL;
    color = 1;
    size = 0;
}

//  Constructor for Nil Node
template <typename keytype, typename valuetype>
Node<keytype, valuetype>::Node(bool isNil) {
    left = this;
    right = this; 
    parent = this; 
    color = 0;  //  Nil node should always be black
    size = 0;  
}

//  Destructor for Node
template <typename keytype, typename valuetype>
Node<keytype, valuetype>::~Node() {
}

/*  Copy constructor for Node
    Deep copies r to this   */
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

/*  Copy assignment operator for Node
    If this != r, deep copies r to this */
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


//  Red-Black Tree Class
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
        int numNodes;   //  Size of red-black tree

        Node<keytype, valuetype>* copy(Node<keytype, valuetype>* x);
        void nuke(Node<keytype, valuetype>* x);

        valuetype* searchRecursive(Node<keytype, valuetype>* x, keytype k);

        void insertFixup(Node<keytype, valuetype>* z);
        void leftRotate(Node<keytype, valuetype>* x);
        void rightRotate(Node<keytype, valuetype>* y);

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

//  Default constructor for red-black tree
template<typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree() {
    head = new Node<keytype, valuetype>;
    nil = new Node<keytype, valuetype>(true);

    head = nil;
    numNodes = 0;
}

//  Overloaded constructor for red-black tree, constructs tree with s nodes using k[] and V[] as keys and values
template<typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree(keytype k[], valuetype V[], int s) {
    head = new Node<keytype, valuetype>;
    nil = new Node<keytype, valuetype>(true);

    head = nil;
    numNodes = 0;

    for (int i = 0; i < s; i++) insert(k[i], V[i]);
}

//  Destructor for red-black tree
template<typename keytype, typename valuetype>
RBTree<keytype, valuetype>::~RBTree() {
    nuke(head); //  Nukes tree
}

//  Copy constructor for red-black tree
template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree(const RBTree &r) {
    head = new Node<keytype, valuetype>;
    nil = new Node<keytype, valuetype>;

    head = r.nil;
    nil = r.nil;

    numNodes = r.numNodes;

    head = copy(r.head);    //  Deep copy r to this
}

//  Copy assignment operator for red-black tree
template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>& RBTree<keytype, valuetype>::operator=(const RBTree &r) {
    head = new Node<keytype, valuetype>;
    nil = new Node<keytype, valuetype>;

    head = r.nil;
    nil = r.nil;

    numNodes = r.numNodes;

    head = copy(r.head);    //  Deep copy r to this

    return *this;
}

//  Searches red-black tree for key k
template <typename keytype, typename valuetype>
valuetype* RBTree<keytype, valuetype>::search(keytype k) {
    return searchRecursive(head, k);
}

//  Inserts new leaf with key k and value v into red-black tree
template<typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::insert(keytype k, valuetype v) {
    Node<keytype, valuetype>* z = new Node<keytype, valuetype>(k, v);

    z->size = 1;
    z->left = nil;
    z->right = nil;
    z->parent = nil;

    //  If tree is empty, new node becomes head of tree
    if (head == nil) {
        z->color = 0;
        head = z;
        head->size = 1;
        return;
    }

    Node<keytype, valuetype>* x = head;
    Node<keytype, valuetype> *y = nil;

    //  Iterates down through tree, starting with head
    while (x != nil) {
        y = x;      //  Every iteration, copies x to y
        x->size++;  //  Every iteration, increments size of nodes on new node's path

        if (z->key < x->key)    x = x->left;
        else                    x = x->right;
    }

    //  Places new node z in tree
    z->parent = y;
    if (z->key < y->key)    y->left = z;
    else                    y->right = z;

    numNodes++;
    insertFixup(z);
}

//  Removes leaf with key k from red-black tree
template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::remove(keytype k) {
    if (search(k) == NULL)  return 0;   //  If there is no node with key k, return 0

    Node<keytype, valuetype>* x = head;

    //  Iterates down through tree, starting with head, until node with key k is found
    while (x != nil) {
        if (x->key == k)        break;
        else if (k < x->key)    x = x->left;
        else                    x = x->right;
    }

    //  Decrements node sizes on path to removed node
    Node<keytype, valuetype>* s = x->parent;
    while (s != nil) {
        s->size--;
        s = s->parent;
    }

    numNodes--;
    removeNode(x);  //  Actually removes node
    return 1;
}

//  Returns rank of node in red-black tree with key k
template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::rank(keytype k) {
    Node<keytype, valuetype>* x = head;

    /*  Iterates down through tree, starting with head, until either
            1: end of tree is reached, then return 0
            2: node with key k is found, then break */
    while (1) {
        if (x == nil)   return 0;

        if (x->key == k)        break;
        else if (k < x->key)    x = x->left;
        else                    x = x->right;
    }

    int r = x->left->size + 1;

    /*  Iterates up through tree, starting with x, until head is reached
        If x is a right child, add x's subtree to r */
    while (x != head) {
        if (x == x->parent->right)  r = r + x->parent->left->size + 1;
        x = x->parent;
    }

    return r;
}

//  Returns key of node in red-black tree with position pos
template <typename keytype, typename valuetype>
keytype RBTree<keytype, valuetype>::select(int pos) {
    return selectRecursive(head, pos);
}

//  Returns key of node in red-black tree that comes after key k
template <typename keytype, typename valuetype>
keytype* RBTree<keytype, valuetype>::successor(keytype k) {
    Node<keytype, valuetype>* x = new Node<keytype, valuetype>;
    x = head;

    /*  Iterates down through tree, starting with head, until either:
        1: end of tree is reached
        2: node with key k is found */ 
    while (x != nil) {
        if (x->key == k)        break;
        else if (k < x->key)    x = x->left;
        else                    x = x->right;
    }

    if (x == getMax(head))      return NULL;    //  If x is largest node in tree, return NULL

    //  If x is nil, return NULL. Else, find successor of x and return its key
    if (x != nil) {
        x = findSuccessor(x);
        return &(x->key);
    } else  return NULL;
}

//  Returns key of node in red-black tree that comes before key k
template <typename keytype, typename valuetype>
keytype* RBTree<keytype, valuetype>::predecessor(keytype k) {
    Node<keytype, valuetype>* x = new Node<keytype, valuetype>;
    x = head;

    /*  Iterates down through tree, starting with head, until either:
        1: end of tree is reached
        2: node with key k is found */ 
    while (x != nil) {
        if (x->key == k)        break;
        else if (k < x->key)    x = x->left;
        else                    x = x->right;
    }

    if (x == getMin(head))      return NULL;    //  If x is smallest node in tree, return NULL

    //  If x is nil, return NULL. Else, find predecessor of x and return its key
    if (x != nil) {
        x = findPredecessor(x);
        return &(x->key);
    } else  return NULL;
}

//  Returns number of nodes in red-black tree
template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::size() {
    return numNodes;
}

//  Prints the keys of red-black tree in a preorder traversal
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::preorder() {
    printPreorder(head);
    cout << endl;
}

//  Prints the keys of red-black tree in an inorder traversal
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::inorder() {
    printInorder(head);
    cout << endl;
}

//  Prints the keys of red-black tree in a postorder traversal
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::postorder() {
    printPostorder(head);
    cout << endl;
}

//  Prints the smallest k keys in a red-black tree
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::printk(int k) {
    for (int i = 1; i <= k; i++)    cout << select(i) << " ";
    cout << endl;
}

//  Deep copies red-black tree
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

    if (x->left != nil)     y->left = copy(x->left);    //  If x has a left child, recursively copy it to y's left child
    if (x->right != nil)    y->right = copy(x->right);  //  If x has a right child, recursively copy it to y's right child

    return y;
}

//  Destructor helper function
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::nuke(Node<keytype, valuetype>* x) {
    if (x == nil)   return;

    nuke(x->left);  //  Recursively nuke x's left child
    nuke(x->right); //  Recursively nuke x's right child
    delete x;       //  Delete x
}

//  Helper function for search()
template <typename keytype, typename valuetype>
valuetype* RBTree<keytype, valuetype>::searchRecursive(Node<keytype, valuetype>* x, keytype k) {
    if (x == nil)   return NULL;

    /*  If node with key k is found, return node's value. Else:
        1: If current key is less than k, recursively check x's right child
        2: If current key is greater than k, recursively check x's left child   */
    if (x->key == k)        return &(x->value);
    else if (x->key < k)    return searchRecursive(x->right, k);
    else                    return searchRecursive(x->left, k);
}

//  Fixup helper function for insert()
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::insertFixup(Node<keytype, valuetype>* z) {
    while(z->parent->color == 1) {
        if(z->parent == z->parent->parent->left) {
            Node<keytype, valuetype>* y = z->parent->parent->right;

            if(y->color == 1) {
                /*  Case 1.1: z's uncle y is red
                    Sets z's parent and y to black, sets z's parent's parent to red */
                z->parent->color = 0;
                y->color=0;

                if(z->parent->parent != head) z->parent->parent->color = 1;
                z = z->parent->parent;
            } else {
                if(z == z->parent->right) {
                    /*  Case 1.2: z's uncle y is black and z is a right child
                        Sets z to its parent and calls a left rotation on z to transform to case 1.3    */
                    z = z->parent;
                    leftRotate(z);
                }

                /*  Case 1.3: z's uncle y is black and z is a left child
                    Sets z's parent to black and z's parent's parent to red, then calls a right rotation on z's parent's parent */
                z->parent->color = 0;
                z->parent->parent->color = 1;
                rightRotate(z->parent->parent);
            }
        } else {
            Node<keytype, valuetype>* y = z->parent->parent->left;

            if(y->color == 1) {
                /*  Case 2.1: z's uncle y is red
                    Sets z's parent and y to black, sets z's parent's parent to red */
                z->parent->color = 0;
                y->color = 0;

                if(z->parent->parent != head) z->parent->parent->color = 1;
                z=z->parent->parent;
            } else {
                if(z == z->parent->left) {
                    /*  Case 2.2: z's uncle y is black and z is a left child
                        Sets z to its parent and uses a right rotation to transform to case 2.3   */
                    z = z->parent;
                    rightRotate(z);
                }

                /*  Case 2.3: z's uncle y is black and z is a right child
                    Sets z's parent to black and z's parent's parent to red, then calls a left rotation on z's parent's parent  */
                z->parent->color = 0;
                z->parent->parent->color = 1;
                leftRotate(z->parent->parent);
            }
        }
    }
}

//  Left rotation for insert() and remove()
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::leftRotate(Node<keytype, valuetype>* x) {
    if(x->right==nil)   return;

    Node<keytype, valuetype>* y = x->right;

    //  Turn's y's left subtree into x's right subtree
    x->right = y->left;
    if(y->left != nil)  y->left->parent = x;

    y->parent = x->parent;

    if(x->parent == nil)            head = y;
    else if(x == x->parent->left)   x->parent->left = y;
    else                            x->parent->right = y;

    y->left = x;
    x->parent = y;

    //  Updates node sizes
    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
}

//  Right rotation for insert() and remove()
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::rightRotate(Node<keytype, valuetype>* y) {
    if(y->left == nil)  return;

    Node<keytype, valuetype>* x = y->left;

    //  Turn y's right subtree into x's left subtree
    y->left = x->right;
    if(x->right != nil) x->right->parent = y;

    x->parent = y->parent;

    if(y->parent == nil)            head = x;
    else if(y == y->parent->right)  y->parent->right = x;
    else                            y->parent->left = x;

    x->right = y;
    y->parent = x;

    //  Updates node sizes
    x->size = y->size;
    y->size = y->left->size + y->right->size + 1;
}

//  Removes node z from red-black tree, helper function for remove()
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::removeNode(Node<keytype, valuetype>* z) {
    /*  x => node moving into y's original position
        y => either the node being removed (cases 1 and 2) or the node moved within the tree (case 3)
        z => node being removed */
    Node<keytype, valuetype>* x;
    Node<keytype, valuetype>* y = z;
    bool originalColor=y->color;
    
    if(z->left == nil) {
        /*  Case 1: removed node has no left child
            Sets x to z's right child and transplants z's right child to z  */
        x = z->right;
        transplant(z, z->right);
    } else if(z->right == nil) {
        /*  Case 2: removed node has no right child
            Sets x to z's left child and transplants z's left child to z    */
        x = z->left;
        transplant(z, z->left);
    } else {
        /*  Case 3: removed node has both a left and a right child
            Sets y to z's predecessor, sets x to y's right child    */
        y = getMin(z->right);
        originalColor = y->color;

        x = y->right;

        /*  If z's predecessor is one of its children, set x's parent to y
            Else, transplant y's right child to y, then set y's right child to z's right child
            and y's right child's parent to y   */
        if(y->parent == z)  x->parent = y;
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;

            //  Decrement size of all nodes between x's parent and y
            Node<keytype, valuetype> *s = x->parent;
            while(s != nil && s != y) {
                s->size--;
                s = s->parent;
            }
        }

        transplant(z, y);

        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
        y->size = y->left->size + y->right->size + 1;
    }

    if(originalColor==0)    deleteFixup(x); //  If node y was black, it's possible red-black properties were violated
                                            //  So, call deleteFixup()
}

//  Transplant helper function for remove 
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::transplant(Node<keytype, valuetype>* u, Node<keytype, valuetype>* v) {
    /*  Replaces one of u's subtrees with the subtree rooted at v
        The subtree replaced depends on the following factors:
            1: If u is the head of the entire tree, v becomes the head of the tree
            2: If u is a left child of a parent node, v becomes the left child
            3: If u is a right child of a parent node, v becomes the right child    */
    if (u->parent == nil)           head = v;
    else if (u == u->parent->left)  u->parent->left = v;
    else                            u->parent->right = v;

    v->parent = u->parent;
}

//  Fixup helper function for remove()
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::deleteFixup(Node<keytype, valuetype>* x) {
    while(x != head && x->color == 0) {
            if(x==x->parent->left) {
                Node<keytype, valuetype> *w = x->parent->right;

                if(w->color==1) {
                    /*  Case 1.1: x's sibling w is red
                        Switch colors of w and x's parent and calls a left rotation on x's parent
                        Then, sets w to x's new sibling */
                    w->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }

                if(w->left->color == 0 && w->right->color == 0) {
                    /*  Case 1.2: x's sibling w is black and both of w's children are black
                        Sets w to black and sets x to its parent
                        If entered through case 1.1, terminates loop  */
                    w->color = 1;
                    x = x->parent;
                } else {
                    if(w->right->color == 0) {
                        /*  Case 1.3: x's sibling w is black, w's left child is red and its right child is black
                            Switch colors of w and its left child and calls a right rotation on w
                            Then, sets w to x's new sibling */
                        w->left->color = 0;
                        w->color = 1;
                        rightRotate(w);
                        w = x->parent->right;
                    }

                    /*  Case 1.4: x's sibling w is black and w's right child is red
                        Sets w to x's parent's color, x's parent's color to red and w's right child's color to black
                        Then, calls a left rotation on x's parent and afterwards sets the head of the tree to x, which
                        causes the loop to terminate    */
                    w->color = x->parent->color;
                    x->parent->color = 1;
                    w->right->color = 0;
                    leftRotate(x->parent);
                    x = head;
                }
            } else {
                Node<keytype, valuetype> *w = x->parent->left;

                if(w->color == 1) {
                    /*  Case 2.1: x's sibling w is red
                        Switch colors of w and x's parent and calls a right rotation on x's parent
                        Then, sets w to x's new sibling */
                    w->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }

                if(w->right->color == 0 && w->left->color == 0) {
                    /*  Case 2.2: x's sibling w is black and both of w's children are black
                        Sets w to black and sets x to its parent
                        If entered through case 2.1, terminates loop  */
                    w->color = 1;
                    x = x->parent;
                } else {
                    if(w->left->color == 0) {
                        /*  Case 2.3: x's sibling w is black, w's left child is black and its right child is red
                            Switch colors of w and its right child and calls a left rotation on w
                            Then, sets w to x's new sibling */
                        w->right->color = 0;
                        w->color = 1;
                        leftRotate(w);
                        w = x->parent->left;
                    }

                    /*  Case 2.4: x's sibling w is black and w's left child is red
                        Sets w to x's parent's color, x's parent's color to red and w's left child's color to black
                        Then, calls a right rotation on x's parent and afterwards sets the head of the tree to x, which
                        causes the loop to terminate    */
                    w->color = x->parent->color;
                    x->parent->color = 1;
                    w->left->color = 0;
                    rightRotate(x->parent);
                    x = head;
                }
            }
        }

        x->color = 0;
}

//  Helper function for select()
template <typename keytype, typename valuetype>
keytype RBTree<keytype, valuetype>::selectRecursive(Node<keytype, valuetype>* x, int i) {
    int r = x->left->size + 1;  //  r gets size of x's left subtree

    /*  If x's left subtree size is equal to i, return x's key. Else:
        1: If current node's left subtree size is greater than i, recursively check x's left child
        2: If current node's left subtree size is less than i, recursively check x's right child   */
    if (i == r)         return x->key;
    else if (i < r)     return selectRecursive(x->left, i);
    else                return selectRecursive(x->right, i - r);
}

//  Helper function for successor()
template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::findSuccessor(Node<keytype, valuetype>* x) {
    if (x->right != nil)    return getMin(x->right);    //  If x has a right child, return smallest node in x's right subtree

    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;
    y = x->parent;

    /*  Iterate up through tree, starting with x's parent, until either:
        1:  End of tree is reached
        2:  We encounter a node that is a left child    */
    while (y != nil && x == y->right) {
        x = y;
        y = y->parent;
    }

    return y;
}

//  Helper function for predecessor()
template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::findPredecessor(Node<keytype, valuetype>* x) {
    if (x->left != nil) return getMax(x->left); //  If x has a left child, return largest node in x's left subtree

    Node<keytype, valuetype>* y = new Node<keytype, valuetype>;
    y = x->parent;

    /*  Iterate up through tree, starting with x's parent, until either:
        1:  End of tree is reached
        2:  We encounter a node that is a right child    */
    while (y != nil && x == y->left) {
        x = y;
        y = y->parent;
    }

    return y;
}

//  Finds smallest node in a subtree
template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::getMin(Node<keytype, valuetype>* x) {
    while (x->left != nil)  x = x->left;    //  Until x doesn't have a left child, set x to its left child
    return x;
}

//  Finds largest node in a subtree
template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::getMax(Node<keytype, valuetype>* x) {
    while (x->right != nil)  x = x->right;  //  Until x doesn't have a right child, set x to its right child
    return x;
}

//  Helper function for preorder()
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::printPreorder(Node<keytype, valuetype>* x) {
    if (x == nil)   return;

    cout << x->key << " ";
    printPreorder(x->left);
    printPreorder(x->right);
}

//  Helper function for inorder()
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::printInorder(Node<keytype, valuetype>* x) {
    if (x == nil)   return;

    printInorder(x->left);
    cout << x->key << " ";
    printInorder(x->right);
}

//  Helper function for postorder()
template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::printPostorder(Node<keytype, valuetype>* x) {
    if (x == nil)   return;

    printPostorder(x->left);
    printPostorder(x->right);
    cout << x->key << " ";
}
