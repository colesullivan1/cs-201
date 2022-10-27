/*
1. Search
2. Remove

*/

#include <iostream>
#include "RBTree.cpp"
using namespace std;

int main(){
    int keys[] = {50,25,75,10,20,90,80,100,1,5,8,32,72,49,4,125,95,126,127,128,129,130,131,132,133,134,136,129,140,150,162};
    int vals[] = {50,25,75,10,20,90,80,100,1,5,8,32,72,49,4,125,95,126,127,128,129,130,131,132,133,134,136,129,140,150,162};

    RBTree<int, int>* myTree = new RBTree<int,int>(keys,vals,31);

    for(int i = 0; i < 31; i++){
        int* answer = myTree->search(keys[i]);
        if(answer == nullptr)
            cout << "Error - Number " << keys[i] << " not found" << endl;
    }

    int* answer = myTree->search(1000);
    if(answer != nullptr)
        cout << "Error - Search returned false positive" << endl;

    if(myTree->remove(10000) != 0){
        cout << "Error - Remove tried to remove data that doesn't exist" << endl;
    }
    
    //----------------------------------------------------------------


    //----------------------------------------------------------------


    //---------------------------------------------------------------- 


    //----------------------------------------------------------------


    //----------------------------------------------------------------

    if(myTree->remove(100) == 0){
        cout << "Error - " << 100 << " failed" <<endl;
    }

    if(myTree->remove(4) == 0){
        cout << "Error - " << 4   << " failed" <<endl;
    }

    if(myTree->remove(130) == 0){
        cout << "Error - " << 130 << " failed" <<endl;
    }

    if(myTree->remove(126) == 0){
        cout << "Error - " << 126 << " failed" <<endl;
    }

    if(myTree->remove(5) == 0){
        cout << "Error - " << 5 << " failed" <<endl;
    }

    if(myTree->remove(132) == 0){
        cout << "Error - " << 132 << " failed" <<endl;
    }
    
    if(myTree->remove(131) == 0){
        cout << "Error - " << 131 << " failed" <<endl;
    }
    cout << 1 << endl;
    if(myTree->remove(134) == 0){
        cout << "Error - " << 134 << " failed" <<endl;
    }
    cout << 2 << endl;

    if(myTree->remove(20) == 0){
        cout << "Error - " << 20 << " failed" <<endl;
    }

    if(myTree->remove(80) == 0){
        cout << "Error - " << 80 << " failed" <<endl;
    }

    if(myTree->remove(90) == 0){
        cout << "Error - " << 90 << " failed" <<endl;
    }

    if(myTree->remove(8) == 0){
        cout << "Error - " << 8 << " failed" <<endl;
    }

    

    myTree->insert(48,48);
    myTree->insert(24,24);
    myTree->insert(23,23);
    myTree->insert(22,22);

    if(myTree->remove(10) == 0){
        cout << "Error - " << 10 << " failed" <<endl;
    }

    if(myTree->remove(25) == 0){
        cout << "Error - " << 25 << " failed" <<endl;
    }

    if(myTree->remove(48) == 0){
        cout << "Error - " << 48 << " failed" <<endl;
    }

    if(myTree->remove(1) == 0){
        cout << "Error - " << 1 << " failed" <<endl;
    }

    if(myTree->remove(129) == 0){
        cout << "Error - " << 129 << " failed" <<endl;
    }

    cout << "Desired Preorder: 95 32 23 22 24 50 49 75 72 129 127 125 128 140 133 136 150 162\nActual Preod    : ";
    myTree->preorder();
    cout << endl;

    cout << "Desired inorder : 22 23 24 32 49 50 72 75 95 125 127 128 129 133 136 140 150 162\nActual  inod    : ";
    myTree->inorder();
    cout << endl;

    cout << "Desired output: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18" << endl;
    cout << "Actual  output: ";
    for(int i = 1; i <= 18; i++){
        cout << myTree->rank(myTree->select(i)) << ' ';
    }
    cout << endl;
    cout << endl;

    myTree->inorder();
    for(int i = 18; i>=1; i--){
        myTree->printk(i);
    }
    
    int inorderArray[] = {22, 23, 24, 32, 49, 50, 72, 75, 95, 125, 127, 128, 129, 133, 136, 140, 150, 162};

    cout << "Pred Test\n";

    myTree->inorder();
    for(int i = 0; i < 18; i++){
        int* pred = myTree->predecessor(inorderArray[i]);
        if(pred == nullptr){
            cout << "NO ";
        }
        else
            cout << *pred << " ";
    }
    cout << endl<< endl;

    cout << "Succ Test\n"; 

    myTree->inorder();
    for(int i = 0; i < 18; i++){
        int* pred = myTree->successor(inorderArray[i]);
        if(pred == nullptr){
            cout << "NO ";
        }
        else
            cout << *pred << " ";
    }
    cout << endl; 
    
    

    
}