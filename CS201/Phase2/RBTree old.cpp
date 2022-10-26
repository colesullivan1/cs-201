#include <iostream>
#include <cmath>
using namespace std;

template <typename keytype, typename valuetype>
class RBTree {

    public:
        RBTree() {
            this->head = new Node;
            this->nil = new Node;
            this->count = 0;
            
            this->head = this->nil;
        };

        RBTree(keytype k[], valuetype V[], int s) {
            this->head = new Node;
            this->nil = new Node;
            this->count = 0;
            
            this->head = this->nil;

            for (int i = 0; i < s; i++) {
                insert(k[i], V[i]);
            }
        };

        ~RBTree() {
            free(this->head);
        };

        valuetype* search(keytype k) {
            return this->findVal(this->head, k);
        }

        void insert(keytype k, valuetype v) {
            Node* x = new Node;
            Node* y = new Node;
            Node* z = new Node;

            y = this->nil;
            x = this->head;

            z->key = k;
            z->value = v;

            while (x != this->nil) {
                y = x;
                if (k < x->key)  x = x->left;
                else    x = x->right;
            }

            z->parent = y;

            if (y == this->nil)  this->head = z;
            else if (z->key < y->key)   y->left = z;
            else    y->right = z;

            z->left = this->nil;
            z->right = this->nil;
            z->color = 1;

            this->insertFixup(z);
            this->count++;
        };

        int remove(keytype k) {
            Node* y = new Node;
            y = this->head;
            
            while (y != this->nil) {
                if (y->key == k)    break;
                else if (k < y->key)    y = y->left;
                else    y = y->right;
            }

            this->count--;

            if (y->key == k) {
                this->removeNode(y);
                return 1;
            } else    return 0;
        };

        int rank(keytype k) {
            Node* y= new Node;
            y = this->head;

            while (y != this->nil) {
                if (y->key == k)    break;
                else if (k < y->key)    y = y->left;
                else    y = y->right;
            }

            if (y == this->nil) return 0;

            int r = 1 + this->sizeOf(y->left);

            while (y != this->head) {
                if (y == y->parent->right)  r = r + 1 + this->sizeOf(y->parent->left);
                y = y->parent;
            }

            return r;
        };

        keytype select(int pos) {
            return this->findPos(this->head, pos);
        };

        keytype* successor(keytype k) {
            Node* y = new Node;
            y = this->head;

            while (y != this->nil) {
                if (y->key == k)    break;
                else if (k < y->key)    y = y->left;
                else    y = y->right;
            }

            if (y != this->nil) {
                y = this->findSuccessor(y);
                return &(y->key);
            } else    return NULL;
        }

        keytype* predecessor(keytype k) {
            Node* y = new Node;
            y = this->head;

            while (y != this->nil) {
                if (y->key == k)    break;
                else if (k < y->key)    y = y->left;
                else    y = y->right;
            }

            if (y != this->nil) {
                y = this->findPredecessor(y);
                return &(y->key);
            } else    return NULL;
        }

        int size() {
            return this->count;
        }

        void preorder() {
            printPreOrder(this->head);
            cout << endl;
        };

        void inorder() {
            printInOrder(this->head);
            cout << endl;
        }

        void postorder() {
            printPostOrder(this->head);
            cout << endl;
        }

        void printk(int k) {
            for (int i = 1; i <= k; i++) cout << this->findPos(this->head,i) << " ";
            cout << endl;
        }

    private:
        struct Node {
            keytype key;
            valuetype value;
            Node* left;
            Node* right;
            Node* parent;
            //color = 0 => black, color = 1 => red
            bool color;
        };

        Node* head;
        Node* nil;
        int count;

        void insertFixup(Node* z) {
            Node* y = new Node;
            while(z->parent->color == 1) {
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
                            this->leftRotate(z);
                        }
                        z->parent->color = 0;
                        z->parent->parent->color = 1;
                        this->rightRotate(z->parent->parent);
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
                            this->rightRotate(z);
                        }
                        z->parent->color = 0;
                        z->parent->parent->color = 1;
                        this->leftRotate(z->parent->parent);
                    }
                }
            }
            this->head->color = 0;
        }; 

        void leftRotate(Node* z) {
            Node* y = new Node;
            y = z->right;
            z->right = y->left;

            if (y->left != this->nil)   y->left->parent = z;
            y->parent = z->parent;

            if (z->parent == this->nil) this->head = y;
            else if (z == z->parent->left)  z->parent->left = y;
            else    z->parent->right = y;

            y->left = z;
            z->parent = y;
        };

        void rightRotate(Node* z) {
            Node* y = new Node;
            y = z->left;
            z->left = y->right;

            if (y->right != this->nil)   y->right->parent = z;
            y->parent = z->parent;

            if (z->parent == this->nil) this->head = y;
            else if (z == z->parent->right)  z->parent->right = y;
            else    z->parent->left = y;

            y->right = z;
            z->parent = y;
        };

        valuetype* findVal(Node* z, keytype k) {
            if (z == this->nil)  return NULL;

            if (z->key == k)  return &(z->value);
            else if (z->key < k)    return findVal(z->right, k);
            else    return findVal(z->left, k);
        };

        int sizeOf(Node* z) {
            if (z == this->nil) return 0;
            else    return 1 + this->sizeOf(z->left) + this->sizeOf(z->right);
        }

        keytype findPos(Node* x, int i) {
            int r = this->sizeOf(x->left) + 1;
            if (i == r) return x->key;
            else if (i < r) return this->findPos(x->left, i);
            else    return this->findPos(x->right, i - r);
        }

        Node* getMin(Node* x) {
            Node* y = new Node;
            y = x;

            while (y->left != this->nil)    y = y->left;

            return y;
        }

        Node* getMax(Node* x) {
            Node* y = new Node;
            y = x;

            while (y->right != this->nil)   y = y->right;

            return y;
        }

        void transplant(Node* u, Node* v) {
            if (u->parent == this->nil) this->head = v;
            else if (u == u->parent->left)  u->parent->left = v;
            else    u->parent->right = v;

            v->parent = u->parent;
        }

        void removeNode(Node* z) {
            Node* x = new Node;
            Node* y = new Node;
            y = z;

            bool originalColor = y->color;

            if (z->left == this->nil) {
                x = z->right;
                this->transplant(z, z->right);
            } else if (z->right == this->nil) {
                x = z->left;
                this->transplant(z, z->left);
            } else {
                y = this->getMax(z->left);
                originalColor = y->color;
                x = y->right;

                if (y->parent == z) x->parent = y;
                else {
                    this->transplant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }

                this->transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
            }

            if (originalColor == 0) this->deleteFixup(x);
        };

        void deleteFixup(Node* x) {
            Node* w = new Node;

            while (x != this->head && x->color == 0) {
                if (x == x->parent->left) {
                    w = x->parent->right;

                    if (w->color == 1) {
                        w->color = 0;
                        x->parent->color = 1;
                        this->leftRotate(x->parent);
                        w = x->parent->right;
                    }

                    if (w->left->color == 0 && w->right->color == 0) {
                        w->color = 1;
                        x = x->parent;
                    } else {
                        if (w->right->color == 0) {
                            w->left->color = 0;
                            w->color = 1;
                            this->rightRotate(w);
                            w = x->parent->right;
                        }

                        w->color = x->parent->color;
                        x->parent->color = 0;
                        w->right->color = 0;
                        this->leftRotate(x->parent);
                        x = this->head;
                    }
                } else {
                    w = x->parent->left;

                    if (w->color == 1) {
                        w->color = 0;
                        x->parent->color = 1;
                        this->rightRotate(x->parent);
                        w = x->parent->left;
                    }

                    if (w->right->color == 0 && w->left->color == 0) {
                        w->color = 1;
                        x = x->parent;
                    } else {
                        if (w->left->color == 0) {
                            w->right->color = 0;
                            w->color = 1;
                            this->leftRotate(w);
                            w = x->parent->left;
                        }

                        w->color = x->parent->color;
                        x->parent->color = 0;
                        w->left->color = 0;
                        this->rightRotate(x->parent);
                        x = this->head;
                    }
                }
            }
            x->color = 0;
        }

        Node* findSuccessor(Node* x) {
            if (x->right != this->nil) {
                return this->getMin(x->right);
            }

            Node* y = new Node;
            y = x->parent;

            while (y != this->nil && x == y->right) {
                x = y;
                y = y->parent;
            }

            return y;
        }

        Node* findPredecessor(Node* x) {
            if (x->left != this->nil) {
                return this->getMax(x->left);
            }

            Node* y = new Node;
            y = x->parent;

            while (y != this->nil && x == y->left) {
                x = y;
                y = y->parent;
            }

            return y;
        }

        void printPreOrder(Node* z) {
            if (z == this->nil)  return;

            cout << z->key << " ";
            printPreOrder(z->left);
            printPreOrder(z->right);
        };

        void printInOrder(Node* z) {
            if (z == this->nil)  return;

            printInOrder(z->left);
            cout << z->key << " ";
            printInOrder(z->right);
        };

        void printPostOrder(Node* z) {
            if (z == this->nil) return;

            printPostOrder(z->left);
            printPostOrder(z->right);
            cout << z->key << " ";
        }
};
