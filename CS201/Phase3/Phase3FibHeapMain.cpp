#include <iostream>
using namespace std;
#include "CircularDynamicArray.cpp"
#include "FibHeap.cpp"

void printErrors(string errors, int numOfErrors){
	if(numOfErrors < 5){
		cout << errors << " caused less than 5 errors so +1 line" << endl;
	}
	if(numOfErrors < 100){
		cout << errors << " caused less than 100 errors so +1 line" << endl;
	}
	if (numOfErrors < 500){
		cout << errors << " caused less than 500 errors so +1 line" << endl;
	}
	if (numOfErrors < 5000){
		cout << errors << " caused less than 5000 errors so +1 line" << endl;
	}
	else{
		cout << errors << " caused more than 5000 errors resulting in 0 points" << endl;
	}
}

int main(){

	string B[25] = {"ZY","AS","AG","AD","A","Y","B","C","D","DE","X","F","G","BH","I","W","J","JK","M","O","IU","Q","R","S","T"};

	CircularDynamicArray<FibHeapNode<string> *> Z1;
	FibHeap<string> Z(B,25, Z1), Y;

	Z.printKey();
	
	cout << endl << Z.extractMin() << endl;

	Z.printKey();
	
	CircularDynamicArray<FibHeapNode<string> *> Y1;
	Y1.addEnd(Y.insert("P")); 
	Y1.addEnd(Y.insert("h"));
	Y1.addEnd(Y.insert("a"));
	Y1.addEnd(Y.insert("s"));
	Y1.addEnd(Y.insert("e"));
	Y1.addEnd(Y.insert("3+3-3"));
	
	Y.printKey();
	
	Y.merge(Z);
	cout << endl << Y.peekKey() << endl;
	
	Y.printKey();
	
    string A[10] = {"A","B","C","D","E","F","H","I","J","K"};
    CircularDynamicArray<FibHeapNode<string> *> H1;
	FibHeap<string> H(A,10,H1);

    H.printKey();

	cout << H.extractMin() << endl;
	
	H.printKey();
	
	cout << H.extractMin() << endl;
	
	H.decreaseKey(H1[6],"A");   //This should decrease H to an A
	cout<<H.peekKey()<<endl;    //Should print an A
	H.printKey();

	H.decreaseKey(H1[5],"B");   //This should decrease F to an B
	cout<<H.peekKey()<<endl;    //Should print an A
	H.printKey();

	CircularDynamicArray<FibHeapNode<int> *> X1;
	FibHeap<int> X;
	for(int i = 1; i < 100001; i++){
		X1.addEnd(X.insert(-i));
	}
	int peekKey=0;
	int extractMin=0;
	for(int i = 100000; i > 0; i--){
		if(X.peekKey() != -i) peekKey++;
		if(X.extractMin() != -i) extractMin++;
	}
	printErrors("peekKey",peekKey);
	printErrors("extractMin",extractMin);		
	
	CircularDynamicArray<FibHeapNode<int> *> C1, D1;
	FibHeap<int> C,D;
	for(int i = 1; i < 101001; i++){
		C1.addEnd(C.insert(-i));
		D1.addEnd(D.insert(i));
	}
	C.merge(D);
	peekKey=0;
	extractMin=0;
	for(int i = 101000; i > 0; i--){
		if(C.peekKey() != -i) peekKey++;
		if(C.extractMin() != -i) extractMin++;
	}
	for(int i = 1; i < 101000; i++){
		if(C.peekKey() != i) peekKey++;
		if(C.extractMin() != i) extractMin++;
	}
	printErrors("peekKey",peekKey);
	printErrors("extractMin",extractMin);

    CircularDynamicArray<FibHeapNode<int> *> E1;
	FibHeap<int> E;
	int decreaseKey=0;
	extractMin=0;
	peekKey=0;
	for(int i = 1; i < 101001; i++){
		E1.addEnd(E.insert(i));
	}
    for(int i = 0; i < 101000; i++){
        if(!E.decreaseKey(E1[i], i)) decreaseKey++;
        if(E.peekKey() != i) peekKey++;
        if(E.extractMin() != i) extractMin++;
	}
	printErrors("decreaseKey",decreaseKey);
	printErrors("peekKey",peekKey);
	printErrors("extractMin",extractMin);

	//Test generics
	FibHeap<long> G;
	FibHeap<double> I;

	cout << "Finished without failing" << endl;
	return 0;
}
