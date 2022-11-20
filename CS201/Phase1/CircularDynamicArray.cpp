/*  Author:     Cole Sullivan
    Due Date:   September 18, 2022
    Class:      CS 201-002
    Assignment: Build a circular dynamic array based on the methods described
                in lecture and in the Introduction to Algorithms textbook   */

#include <iostream>
#include <math.h>
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

        elmtype QuickSelect(int k);
        elmtype WCSelect(int k);

        void stableSort();

        int linearSearch(elmtype e);
        int binSearch(elmtype e);

    private:
        int cap;
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
    cap = 2;
    size = 0;
    data = new elmtype[cap];
    front = 0;
    error = -1;
}

//  Overloaded constructor for circular dynamic array, constructs array with capacity and size s
template <typename elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray(int s) {
    cap = s;
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
    cap = cda.cap;
    size = cda.size;
    data = new elmtype[cap];
    front = 0;
    error = -1;

    for (int i = 0; i < cda.size; i++)  addEnd(cda.data[i]);    //  Essentially deep copies cda.data to data
}

//  Copy assignment operator for circular dynamic array
template <typename elmtype>
CircularDynamicArray<elmtype>& CircularDynamicArray<elmtype>::operator=(const CircularDynamicArray &cda) {
    cap = cda.cap;
    size = 0;
    data = new elmtype[cap];
    front = 0;
    error = -1;

    for (int i = 0; i < cda.size; i++)  addEnd(cda.data[i]);    //  Essentially deep copies cda.data to data

    return *this;
}

//  Returns reference to element stored at position i in the circular dynamic array
template <typename elmtype>
elmtype& CircularDynamicArray<elmtype>::operator[](int i) {
    if (i >= 0 && i < size) return data[(front + i) % cap];
    else {
        cout << "Error: index " << i << " is out of bounds" << endl;
        return error;
    }
}

//  Adds element v to end of circular dynamic array
template <typename elmtype>
void CircularDynamicArray<elmtype>::addEnd(elmtype v) {
    if (size >= cap) {
        /*  If size is greater than or equal to capacity,
            increase size by 1 and multiply capacity by 2   */

        //  Create temp array holding old data values
        elmtype* temp = new elmtype[cap * 2];
        for (int i = 0; i < size; i++)  temp[i] = data[(front + i) % cap];

        //  Increase capacity and copy temp to data
        cap *= 2;
        data = new elmtype[cap];
        data = temp;
        front = 0;

        //  Add v to end of array and increase size by 1
        data[(front + size) % cap] = v;
        size++;
    } else {
        /*  If size is less than capacity,
            just add v to array and increase size by 1  */

        data[(front + size) % cap] = v;
        size++;
    }
}

//  Add element v to front of circular dynamic array
template <typename elmtype>
void CircularDynamicArray<elmtype>::addFront(elmtype v) {
    if (size >= cap) {
        /*  If size is greater than or equal to capacity,
            increase size by 1 and multiply capacity by 2   */

        //  Create temp array holding old data values
        elmtype* temp = new elmtype[cap * 2];
        for (int i = 0; i < size; i++)  temp[i] = data[(front + i) % cap];

        //  Increase capacity and copy temp to data
        cap *= 2;
        data = new elmtype[cap];
        data = temp;
        
        //  Move front, add v to front of array and increase size by 1
        front = (front + (cap - 1)) % cap;
        data[front] = v;
        size++;
    } else {
        /*  If size is less than capacity,
            just move front, add v to front of array and increase size by 1  */

        front = (front + (cap - 1)) % cap;
        data[front] = v;
        size++;
    }
}

//  Remove element at end of circular dynamic array
template <typename elmtype>
void CircularDynamicArray<elmtype>::delEnd() {
    size--;

    if (((double)size) / ((double)cap) <= 0.25) {
        //  If size is less than or equal to 1/4 of capacity, halve capacity

        //  Create temp array holding old data values
        elmtype* temp = new elmtype[cap / 2];
        for (int i = 0; i < size; i++)  temp[i] = data[(front + i) % cap];

        //  Halve capacity and copy temp to data
        cap /= 2;
        data = new elmtype[cap];
        data = temp;
        front = 0;
    }
}

//  Remove element at front of circular dynamic array
template <typename elmtype>
void CircularDynamicArray<elmtype>::delFront() {
    front = (front + 1) % cap;
    size--;

    if (((double)size / (double)cap) <= 0.25) {
        //  If size is less than or equal to 1/4 of capacity, halve capacity
        
        //  Create temp array holding old data values
        elmtype* temp = new elmtype[cap / 2];
        for (int i = 0; i < size; i++)  temp[i] = data[(front + i) % cap];

        //  Halve capacity and copy temp to data
        cap /= 2;
        data = new elmtype[cap];
        data = temp;
        front = 0;
    }
}

//  Return length (size) of circular dynamic array
template <typename elmtype>
int CircularDynamicArray<elmtype>::length() {
    return size;
}

//  Return capacity of circular dynamic array
template <typename elmtype>
int CircularDynamicArray<elmtype>::capacity() {
    return cap;
}

//  Clears array and starts over with capacity 2 and size 0
template <typename elmtype>
void CircularDynamicArray<elmtype>::clear() {
    cap = 2;
    size = 0;
    data = new elmtype[cap];
    front = 0;
}

template <typename elmtype>
elmtype CircularDynamicArray<elmtype>::QuickSelect(int k) {
    //  FIXME
}

template <typename elmtype>
elmtype CircularDynamicArray<elmtype>::WCSelect(int k) {
    //  FIXME
}

template <typename elmtype>
void CircularDynamicArray<elmtype>::stableSort() {
    if ((size) > 1) {
        CircularDynamicArray<elmtype> leftHalf;
        CircularDynamicArray<elmtype> rightHalf;

        int i = 0;
        for (; i < size / 2; i++)   leftHalf.addEnd(data[(front + i) % cap]);
        for (; i < size; i++)       rightHalf.addEnd(data[(front + i) % cap]);

        leftHalf.stableSort();
        rightHalf.stableSort();
        
        int mainIndex = 0;
        int leftIndex = 0;
        int rightIndex = 0;

        while (leftIndex < leftHalf.length() && rightIndex < rightHalf.length()) {
            if (leftHalf.data[(leftHalf.front + leftIndex) % leftHalf.cap] < rightHalf.data[(rightHalf.front + rightIndex) % rightHalf.cap]) {
                data[(front + mainIndex) % cap] = leftHalf.data[leftIndex];
                leftIndex++;
            } else {
                data[(front + mainIndex) % cap] = rightHalf.data[rightIndex];
                rightIndex++;
            }
            mainIndex++;
        }

        while (leftIndex < leftHalf.length()) {
            data[(front + mainIndex) % cap] = leftHalf.data[leftIndex];
            leftIndex++;
            mainIndex++;
        }

        while (rightIndex < rightHalf.length()) {
            data[(front + mainIndex) % cap] = rightHalf.data[rightIndex];
            rightIndex++;
            mainIndex++;
        }
    }
}

template <typename elmtype>
int CircularDynamicArray<elmtype>::linearSearch(elmtype e) {
    for (int i = 0; i < size; i++)  if (data[(front + i) % cap] == e)   return i;
    return -1;
}

template <typename elmtype>
int CircularDynamicArray<elmtype>::binSearch(elmtype e) {
    int left = front;
    int right = size - 1;
    int mid = 0;

    while (left <= right) {
        mid = (left + right) / 2;

        if (data[(front + mid) % cap] == e)     return mid;
        else if (data[(front + mid) % cap] < e) left = mid + 1;
        else                                    right = mid - 1;
    }

    return -1;
}

template <typename elmtype>
int CircularDynamicArray<elmtype>::QuickPartition(int l, int r, int p) {
    //  FIXME
}

template <typename elmtype>
int CircularDynamicArray<elmtype>::WCSelect(int l, int r, int k) {
    //  FIXME
}

template <typename elmtype>
int CircularDynamicArray<elmtype>::WCPivot(int l, int r) {
    //  FIXME
}

template <typename elmtype>
int CircularDynamicArray<elmtype>::WCPartition(int l, int r, int p, int k) {
    //  FIXME
}

template <typename elmtype>
int CircularDynamicArray<elmtype>::WCPartition5(int l, int r) {
    //  FIXME
}