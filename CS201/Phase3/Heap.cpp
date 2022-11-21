/*  Author:     Cole Sullivan
    Due Date:   November 22, 2022
    Class:      CS 201-002
    Assignment: Build a standard binary heap based on the methods described in
                the recorded lectures and in the Introduction to Algorithms textbook    */

#include <iostream>
#include "CircularDynamicArray.cpp"
using namespace std;

template <typename keytype>
class Heap {
    public:
        Heap();
        Heap(keytype k[], int s);
        ~Heap();
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

template <typename keytype>
Heap<keytype>::Heap() {
    data = CircularDynamicArray<keytype>();
    size = 0;
}

template <typename keytype>
Heap<keytype>::Heap(keytype k[], int s) {
    data = CircularDynamicArray<keytype>();
    size = s;

    for (int i = 0; i < size; i++)  data.addEnd(k[i]);
    for (int i = (size / 2) - 1; i >= 0; i--)   heapify(i);
}

template <typename keytype>
Heap<keytype>::~Heap() {
}

template <typename keytype>
keytype Heap<keytype>::peekKey() {
    return data[0];
}

template <typename keytype>
keytype Heap<keytype>::extractMin() {
    keytype min = data[0];
    data[0] = data[size - 1];
    size--;
    heapify(0);
    return min;
}

//  Inserts key k into heap
template <typename keytype>
void Heap<keytype>::insert(keytype k) {
    size++;
    data.addEnd(k);
    
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
    cout << endl;
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

template <typename keytype>
void Heap<keytype>::heapify(int i) {
    int l = left(i);
    int r = right(i);
    int lowest = i;

    if (l < size && data[l] < data[i])      lowest = l;
    if (r < size && data[r] < data[lowest]) lowest = r;

    if (lowest != i) {
        data.swap(i, lowest);
        heapify(lowest);
    }
}