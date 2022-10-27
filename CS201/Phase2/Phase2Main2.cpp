#include <iostream>
#include <cstdlib>
using namespace std;
#include "RBTree.cpp"

void printErrors(string errors, int numOfErrors){
	if(numOfErrors < 5){
		cout << errors << " PASSED " << endl;
	}else if(numOfErrors < 100){
		cout << errors << " caused " << numOfErrors << " of errors Add+1" << endl;
		cout << errors << " caused " << numOfErrors << " of errors Add+1" << endl;
		cout << errors << " caused " << numOfErrors << " of errors Add+1" << endl;
	}else if (numOfErrors < 1000){
		cout << errors << " caused " << numOfErrors << " of errors Add+1" << endl;
		cout << errors << " caused " << numOfErrors << " of errors Add+1" << endl;
	}else if (numOfErrors < 10000){
		cout << errors << " caused " << numOfErrors << " of errors Add+1" << endl;
	}else{
		cout << errors << " caused " << numOfErrors << " of errors" << endl;
	}
}

void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void removeIndex(int removeIndex);

int main(int argc, char **argv){
	int testToRun = atoi(argv[1]);
	switch (testToRun){
		case 1:
			test1();
			break;
		case 2:
			test2();
			break;
		case 3:
			test3();
			break;
		case 4:
			test4();
			break;
		case 5:
			test5();
			break;
		case 6:
			test6();
			break;
	}
	return 0;
}


void test1(){
	string K[11] = {"B","C","D","E","F","G","H","I","J","K","L"};
	int V[11] = {10,9,8,7,6,5,4,3,2,1,0};
	
	RBTree<string,int> T1, T2(K,V,11);
	T2.postorder();
	
	//Test edge case
	T2.remove("J");
	T2.remove("K");
	
	for(int i=0; i<11; i++) T1.insert(K[i],V[i]);
	// T1 and T2 should be identical trees
	
	T1.postorder();
	
	T1.insert("N",11);
	T1.insert("O",12);
	T1.insert("P",13);
	cout << *(T1.search("N")) << endl;	
	
	cout << T1.rank("H") << endl;
	
	cout << T1.select(5) << endl;
	
	T1.preorder();
	
	T1.inorder();
	
	cout << T1.remove("MISSING") << endl;
	
	cout << T1.remove("O") << endl;
	
	cout <<T1.rank("N") << endl;
	
	T1.inorder();
    
	T1.preorder();
    
	T1.postorder();
    
    T1.printk(5);

	cout << "Finished without failing" << endl << endl;
}

void test2(){
	
	RBTree<int,int> X;
	for (int i=105;i>=0;i--) X.insert(i,105-i);
	
	X.preorder(); 
    
	X.postorder();
    
	X.inorder();
    
	X.printk(55);

	RBTree<int,int> Y = X;
	for (int i=0; i < 55; i++){
		X.rank(i);
		X.remove(i);
	}

    X.preorder();

    X.postorder();

    X.inorder();
	
	Y.preorder();
	
	Y.postorder();
    
	Y.inorder();
    
    Y.printk(55);

    RBTree<int,int> Z;
    for(int i=0; i < 1000500; i++) Z.insert(i, 1000500-i);
    
    Z.printk(90);

	cout << "Finished without failing" << endl << endl;
}

void test3(){
	int rankError = 0;
	int selectError = 0;
	int searchError = 0;
	int treeSize = 100500;
    RBTree<int,int> X;
	for (int i=treeSize;i>=0;i--) X.insert(i,treeSize-i);
	for (int i=1;i<treeSize;i++) {
		cout << i << endl;
		if(X.rank(i) != i+1) rankError++; 
		if(X.select(i) != i-1) selectError++; 
		int* search = X.search(i); 
		if(search == NULL || *(search) != treeSize-i) searchError++; 
	}
	printErrors("Rank",rankError);
	printErrors("Select",selectError);
	printErrors("Search",searchError);
	cout << "Finished without failing" << endl << endl;
}

void test4(){
	removeIndex(13);
}

void test5(){
	removeIndex(19);
}

void test6(){
	RBTree<int,int> X;
	int predErrors = 0;
	int succErrors = 0;
    int size = 11200;
	for (int i=size;i>=0;i--) X.insert(i,size-i);
	for (int i=0; i < X.size(); i++){
		int* pred = X.predecessor(i);
		if(i == 0){
            if(pred != NULL) cout << "Doesn't return NULL for predecessor" << endl;
		}
        else if(*(pred) != i-1){
            predErrors++;
        }
	}
	printErrors("Pred errors",predErrors);
	
	for (int i=X.size()-1; i >= 0; i--){
		int* succ = X.successor(i);
		if(i == size){
            if(succ != NULL) cout << "Doesn't return NULL for successor" << endl;
		}
        else if(*(succ) != i+1){ 
            succErrors++;
        }
	}	
	printErrors("Succ Errors",succErrors);
	
	cout << "Finished without failing" << endl << endl;
}

void removeIndex(int removeIndex){
	int rankError = 0;
	int selectError = 0;
	int searchError = 0;
    RBTree<int,int> X;
	int size = 100500;
    for (int i=size;i>=0;i--) X.insert(i,size-i);
	for (int i=0;i<size;i+=removeIndex) X.remove(i);
	for (int i=1;i<size-size/removeIndex;i++){
        
        int part1Answer = i / removeIndex;
		// Needed for round off error
		int answer = i+(i+(i+(i+(i+(i + i / removeIndex)/removeIndex)/removeIndex)/removeIndex)/removeIndex)/removeIndex;

		//printf("i is %d, Select %d, Rank %d, Search %d\n",i, answer, i-i/removeIndex, size-i);
        
        if(X.select(i) != answer){				
			selectError++;
			//cout << "i is " << i << " Select error after delete " << X.select(i)  << " should be " <<  answer << " or " << part1Answer << endl;
		}
		if (i%removeIndex) {
			//cout << i << " is i" << endl;
			if(X.rank(i) != i-i/removeIndex){
                rankError++; 
                //cout << "Rank error after delete " << X.rank(i) << " should be " << i << " - " << i-i/removeIndex << endl;
			}
			int* search = X.search(i);
            if(search == NULL || *(search) != size-i) searchError++;// cout << "Search error after delete" << endl;
            X.remove(i);
			X.insert(i,size-i);
		}
	}
	printErrors("Rank after delete",rankError);
	printErrors("Select after delete",selectError);
	printErrors("Search after delete",searchError);
	cout << "Finished without failing" << endl << endl;
}

