/*
Part1:
    Regular Constructor
    Array Constructor
    Delete 

    insert

    size
    preorder
    inorder
    postorder

    Copy constructor
    Copy assignment


*/

#include <iostream>
#include "RBTree.cpp"
using namespace std;
int main(){
    int keys[] = {50,25,75,10,20,90,80,100,1,5,8,32,72,49,4,125,95,126,127,128,129,130,131,132,133,134};
    int vals[] = {50,25,75,10,20,90,80,100,1,5,8,32,72,49,4,125,95,126,127,128,129,130,131,132,133,134};

    RBTree<int, int>* myTree = new RBTree<int,int>();

    RBTree<int, int>* arrayTree = new RBTree<int,int>(keys,vals,26);
    RBTree<int, int>* arrayTree2 = new RBTree<int,int>(keys,vals,26);

    for(int i = 0; i < 26; i++){
        cout << myTree->size() << " ";
        myTree->insert(keys[i],vals[i]);
        
    }
    cout << myTree->size() << endl << endl;

    cout << "Desired Preorder: 100 50 20 5 1 4 10 8 32 25 49 80 75 72 90 95 128 126 125 127 130 129 132 131 133 134" << endl;
    cout << "Regular Tree Pre: ";
    myTree->preorder();
    cout << "Array Tree Pre  : ";
    arrayTree->preorder();
    cout << endl;

    cout << "Desired Inorder : 1 4 5 8 10 20 25 32 49 50 72 75 80 90 95 100 125 126 127 128 129 130 131 132 133 134" << endl;
    cout << "Regular Tree in : ";
    myTree->inorder();
    cout << "Array Tree in   : ";
    arrayTree->inorder();
    cout << endl;

    cout << "Desired Postord : 4 1 8 10 5 25 49 32 20 72 75 95 90 80 50 125 127 126 129 131 134 133 132 130 128 100" << endl;
    cout << "Regular Tree Pos: ";
    myTree->postorder();
    cout << "Array Tree posto: ";
    arrayTree->postorder();
    cout << endl;

    RBTree<int, int>* copyTree = new RBTree<int,int>(*myTree);
    RBTree<int, int>* copyAssTree = new RBTree<int,int>();
    
    *copyAssTree = *copyTree;

    delete myTree;
    cout << "TEST@" << endl;
    delete copyTree;
    cout << 2 << endl;

    cout << "Desired Preorder: 100 50 20 5 1 4 10 8 32 25 49 80 75 72 90 95 128 126 125 127 130 129 132 131 133 134" << endl;
    cout << "Actual preorder : ";
    copyAssTree->preorder();
    cout << endl;
    cout << "Desired Inorder : 1 4 5 8 10 20 25 32 49 50 72 75 80 90 95 100 125 126 127 128 129 130 131 132 133 134" << endl;
    cout << "Actual Inorder  : ";
    copyAssTree->inorder();
    cout << endl;
    cout << "Desired Postord : 4 1 8 10 5 25 49 32 20 72 75 95 90 80 50 125 127 126 129 131 134 133 132 130 128 100" << endl;
    cout << "Actual Postorde : ";
    copyAssTree->postorder();
    cout << endl;

    

    return 1;
}