/*  Author:     Cole Sullivan
    Due Date:   September 18, 2022
    Class:      CS 201-002
    Assignment: Build a circular dynamic array based on the methods described
                in lecture and in the Introduction to Algorithms textbook   */

#include <iostream>
using namespace std;

//  Circular Dynamic Array Class
template <typename elmtype>
class CircularDynamicArray {
    public:
        CircularDynamicArray();
        CircularDynamicArray(int s);
        ~CircularDynamicArray();
        CircularDynamicArray(const CircularDynamicArray &cda);
        CircularDynamicArray& operator=(const CircularDynamicArray &cda);

        elmtype& operator[](int i);

        void addEnd(elmtype v);
        void addFront(elmtype v);
        void delEnd();
        void delFront();

        int length();
        int capacity();

        void clear();

        Elmtype QuickSelect(int k);
        Elmtype WCSelect(int k);

        void stableSort();

        int linearSearch(elmtype e);
        int binSearch(elmtype e);
    private:
        int capacity;
        int size;
        elmtype* data;
        int front;
        elmtype error;

        int QuickPartition(int l, int r, int p);

        int WCSelect(int l, int r, int k);
        int WCPivot(int l, int r);
        int WCPartition(int l, int r, int p, int n);
        int WCPartition5(int l, int r);
};

//  Default constructor for circular dynamic array
template <typename elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray() {
    capacity = 2;
    size = 0;
    data = new elmtype[capacity];
    front = 0;
    error = -1;
}

//  Overloaded constructor for circular dynamic array, constructs array with capacity and size s
template <typename elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray(int s) {
    capacity = s;
    size = s;
    data = new elmtype[s];
    front = 0;
    error = -1;
}

//  Destructor for circular dynamic array
template <typename elmtype>
CircularDynamicArray<elmtype>::~CircularDynamicArray() {
    delete[] data;
}

//  Copy constructor for circular dynamic array
template <typename elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray(const CircularDynamicArray &cda) {
    capacity = cda.capacity;
    size = cda.size;
    data = new elmtype[capacity];
    front = 0;
    error = -1;
c
    for (int i = 0; i < cda.size; i++)  addEnd(cda.data[i]);    //  Essentially deep copies cda.data to data
}

//  Copy assignment operator for circular dynamic array
template <typename elmtype>
CircularDynamicArray<elmtype>& CircularDynamicArray<elmtype>::operator=(const CircularDynamicArray &cda) {
    capacity = cda.capacity;
    size = 0;
    data = new elmtype[capacity];
    front = 0;
    error = -1;

    for (int i = 0; i < cda.size; i++)  addEnd(cda.data[i]);    //  Essentially deep copies cda.data to data
}

//  Adds element v to end of circular dynamic array
template <typename elmtype>
void CircularDynamicArray<elmtype>::addEnd(elmtype v) {
    if (size >= capacity) {
        /*  If size is greater than or equal to capacity,
            increase size by 1 and multiply capacity by 2   */

        //  Create temp array holding old data values
        elmtype* temp = new elmtype[capacity * 2];
        for (int i = 0; i < size; i++)  temp[i] = data[(front + i) % capacity];

        //  Increase capacity and copy temp to data
        capacity *= 2;
        data = new elmtype[capacity];
        data = temp;
        front = 0;  // maybe remove?

        //  Add v to array and increase size
        data[(front + size) % capacity] = v;
        size++;
    } else {
        /*  If size is less than capacity,
            just add v to array and increase size   */

        data[(front + size) % capacity] = v;
        size++;
    }
}