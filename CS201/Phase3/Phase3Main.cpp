#include <iostream>
using namespace std;
#include "CircularDynamicArray.cpp"
#include "Heap.cpp"
#include "FibHeap.cpp"

//inside FibHeap.cpp the definitition of FibHeapNode is expected, for example:
//template <typename keytype>
//struct FibHeapNode {
//    keytype x;
//    FibHeapNode<keytype> *ptr;
//    etc.  You may define any elements needed inside FibHeapNode
//};


int main(){
	string A[10] = {"A","B","C","D","E","F","H","I","J","K"};
	int B[10] = {10,9,8,7,6,5,4,3,2,1};

	Heap<int> T1, T2(B,10);

	T2.printKey();
	//Should output  1 2 4 3 6 5 8 10 7 9

	for(int i=0; i<10; i++) T1.insert(B[i]);

	T1.printKey();
	// Should output 1 2 5 4 3 9 6 10 7 8

	cout << T2.peekKey() << endl;
	//Should output 1

	cout << T1.extractMin() << endl;
	//Should output 1

	T1.printKey();
	//Should output 2 3 5 4 8 9 6 10 7

	CircularDynamicArray<FibHeapNode<string> *> H1;
	FibHeap<string> X(A,10,H1), Y;

	X.printKey();
	//Should output
	//Rank 1
	//J K
	//
	//Rank 3
	//A E H I F C D B

	cout << X.extractMin() << endl;
	//Should output A

	X.printKey();
	//Should output
	//Rank 0
	//B
	//
	//Rank 3
	//C E H I F J K D

	cout << X.extractMin() << endl;
	//Should output B

	X.decreaseKey(H1[6],"A");   //This should decrease H to an A
	cout<<X.peekKey()<<endl;    //Should print an A
	X.printKey();
	//Should output
	//Rank 3
	//C E F J K D
	//
	//Rank 1
	//A I

	X.decreaseKey(H1[5],"B");   //This should decrease F to an B
	cout<<X.peekKey()<<endl;    //Should print an A
	X.printKey();
	//Should output
	//Rank 2
	//C J K D
	//
	//Rank 1
	//A I
	//
	//Rank 0
	//B
	//
	//Rank 0
	//E

	H1.addEnd(X.insert("Z"));
	H1.addEnd(X.insert("P"));
	cout << X.extractMin() << endl;
	//Should output A
	X.printKey();
	//Should output
	//Rank 0
	//I
	//
	//Rank 3
	//B C J K D P Z E


	CircularDynamicArray<FibHeapNode<string> *> H2;
	H2.addEnd(Y.insert("M"));
	H2.addEnd(Y.insert("O"));
	H2.addEnd(Y.insert("G"));
	H2.addEnd(Y.insert("N"));
	H2.addEnd(Y.insert("L"));
	H2.addEnd(Y.insert("A"));
	Y.extractMin();

	Y.printKey();
	//Should output
	//Rank 0
	//L
	//
	//Rank 2
	//G M O N

	Y.merge(X);
	cout << Y.peekKey() << endl;
	//Should output B

	Y.printKey();
	//Should output
	//Rank 0
	//L
	//
	//Rank 2
	//G M O N
	//
	//Rank 0
	//I
	//
	//Rank 3
	//B C J K D P Z E

	cout << Y.extractMin() << endl;
	//Should output B

	Y.printKey();
    //Should output
	//Rank 0
	//E
	//
	//Rank 2
	//I P Z L
	//
	//Rank 3
	//C G M O N J K D

	return 0;
}