/*  Author:     Cole Sullivan
    Due Date:   November 22, 2022
    Class:      CS 201-002
    Assignment: Build a standard binary heap based on the methods described in
                the recorded lectures and in the Introduction to Algorithms textbook    */

#include <iostream>
using namespace std;

//  Heap Class
template <typename keytype>
class Heap {
    public:
        Heap();
        Heap(keytype k[], int s);
        ~Heap();
        Heap(const Heap &H);
        Heap& operator=(const Heap &H);
        keytype peekKey();
        keytype extractMin();
        void insert(keytype k);
        void printKey();

    private:
        CircularDynamicArray<keytype> data;
        int size;

        int parent(int i);
        int left(int i);
        int right(int i);

        void heapify(int i);
};

//  Default constructor
template <typename keytype>
Heap<keytype>::Heap() {
    data = CircularDynamicArray<keytype>();
    size = 0;
}

//  Constructs new heap of size s using keys stored in k[]
template <typename keytype>
Heap<keytype>::Heap(keytype k[], int s) {
    data = CircularDynamicArray<keytype>();
    size = s;

    for (int i = 0; i < size; i++)  data.addEnd(k[i]);  //  Adds k[i] to bottom of heap
    for (int i = (size / 2) - 1; i >= 0; i--)   heapify(i); //  Calls heapify s/2 - 1 times
}

//  Destructor
template <typename keytype>
Heap<keytype>::~Heap() {
    size = 0;
}

//  Copy constructor, copies heap H to this
template <typename keytype>
Heap<keytype>::Heap(const Heap &H) {
    data = H.data;
    size = H.size;
}

//  Copy assignment operator, copies heap H to this
template <typename keytype>
Heap<keytype>& Heap<keytype>::operator=(const Heap &H) {
    this->data = H.data;
    this->size = H.size;

    return *this;
}

//  Returns smallest key in heap
template <typename keytype>
keytype Heap<keytype>::peekKey() {
    return data[0];
}

//  Removes smallest key from heap and returns it
template <typename keytype>
keytype Heap<keytype>::extractMin() {
    /*  Sets min to key at top of heap, then removes min from heap and
        moves key from the bottom of the heap to the top            */
    keytype min = data[0];
    data[0] = data[size - 1];
    size--;

    heapify(0); //  Calls heapify on the key at the top of the heap
    return min;
}

//  Inserts key k into heap
template <typename keytype>
void Heap<keytype>::insert(keytype k) {
    size++;
    data.addEnd(k); //  Add k to bottom of heap

    /*  If new key is smaller than its parent, swap new key
    and its parent and set i to new key's new location  */
    int i = size - 1;
    while (i > 0 && data[i] < data[parent(i)]) {
        data.swap(i, parent(i));
        i = parent(i);
    }
}

//  Prints contents of cda storing heap
template <typename keytype>
void Heap<keytype>::printKey() {
    for (int i = 0; i < size; i++)  cout << data[i] << " ";
    if (size != 0)  cout << endl;
}

//  Returns index of parent of i
template <typename keytype>
int Heap<keytype>::parent(int i) {
    return ((i + 1) / 2) - 1;
}

//  Returns index of left child of i
template <typename keytype>
int Heap<keytype>::left(int i) {
    return ((i + 1) * 2) - 1;
}

//  Returns index of right child of i
template <typename keytype>
int Heap<keytype>::right(int i) {
    return ((i + 1) * 2 + 1) - 1;
}

// "Floats" key at i down the heap so that the sub-tree rooted at i obeys the min-heap properties
template <typename keytype>
void Heap<keytype>::heapify(int i) {
    int l = left(i);
    int r = right(i);
    int min = i;

    //  Finds smallest key in i's subtree
    if (l < size && data[l] < data[i])      min = l;
    if (r < size && data[r] < data[min])    min = r;

    //  If i was not the smallest key, swap i and min and call heapify on min
    if (min != i) {
        data.swap(i, min);
        heapify(min);
    }
}