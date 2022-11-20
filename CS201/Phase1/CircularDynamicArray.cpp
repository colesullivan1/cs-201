/*  Author:     Cole Sullivan
    Due Date:   September 18, 2022
    Class:      CS 201-002
    Assignment: Build a circular dynamic array based on the methods described
                in lecture and in the Introduction to Algorithms textbook   */

/*  NEED TO FIX:
     - WCSelect()
     - COMMENTS: mergeSort() and merge()
     - MAYBE: addEnd(), addFront(), delEnd(), delFront()    */

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

        elmtype QuickSelect(int l, int r, int k);
        int QuickPartition(int l, int r);

        int WCSelect(int l, int r, int k);
        int WCPivot(int l, int r);
        int WCPartition(int l, int r, int p, int n);
        int WCPartition5(int l, int r);

        void mergeSort(int l, int r);
        void merge(int l, int m, int r);
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

        //  Create temp array holding old values + new value
        elmtype* temp = new elmtype[cap * 2];
        temp[0] = v;
        for (int i = 0; i < size; i++)  temp[i + 1] = data[(front + i) % cap];

        //  Set front to 0, increase capacity by a factor of 2, increase size by 1
        //  and copy temp to data
        front = 0;
        cap *= 2;
        data = new elmtype[cap];
        data = temp;
        size++;

        /*
        //  Create temp array holding old data values
        elmtype* temp = new elmtype[cap * 2];
        for (int i = 0; i < size; i++)  temp[i] = data[(front + i) % cap];

        //  Increase capacity and copy temp to data
        cap *= 2;
        data = new elmtype[cap];
        data = temp;

        //  Move front, add v to front of array and increase size by 1
        front = (front + cap - 1) % cap;
        data[front] = v;
        size++; */
    } else {
        /*  If size is less than capacity,
            just move front, add v to front of array and increase size by 1  */

        front = (front + cap - 1) % cap;
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

//  Returns kth smallest element using quick select
template <typename elmtype>
elmtype CircularDynamicArray<elmtype>::QuickSelect(int k) {
    return QuickSelect(0, size - 1, k); //  Calls recursive helper function
}

//  Returns kth smallest element using _______
template <typename elmtype>
elmtype CircularDynamicArray<elmtype>::WCSelect(int k) {
    return QuickSelect(0, size - 1, k);
}

//  Sorts circular dynamic array using merge sort
template <typename elmtype>
void CircularDynamicArray<elmtype>::stableSort() {
    mergeSort(0, size - 1);
}

//  Linearly searches circular dynamic array
template <typename elmtype>
int CircularDynamicArray<elmtype>::linearSearch(elmtype e) {
    for (int i = 0; i < size; i++)  if (data[(front + i) % cap] == e)   return i;
    return -1;
}

//  Performs binary search on circular dynamic array
template <typename elmtype>
int CircularDynamicArray<elmtype>::binSearch(elmtype e) {
    int left = 0;
    int right = size - 1;
    int mid = 0;

    while (left <= right) {
        mid = (left + right) / 2;

        /*  If value at middle is equal to e, return middle
            Else if value at middle is less than e, left gets middle + 1
            Else, right gets middle - 1                                 */
        if (data[(front + mid) % cap] == e)     return mid;
        else if (data[(front + mid) % cap] < e) left = mid + 1;
        else                                    right = mid - 1;
    }

    return -1;
}

//  Recursive helper function for QuickSelect
template <typename elmtype>
elmtype CircularDynamicArray<elmtype>::QuickSelect(int l, int r, int k) {
    if (k > 0 && k <= r - l + 1) {
        int index = QuickPartition(l, r);

        /*  If partition index is equal to k, return value at index
            Else, if partition index is greater than k, recursively searches left half of array
            Else, recursively searches right half of array                                      */
        if (index - l == k - 1)     return data[(front + index) % cap];
        else if (index - l > k - 1) return QuickSelect(l, index - 1, k);
        else                        return QuickSelect(index + 1, r, k - index + l - 1);
    }

    return error;   //  If k is not a valid value, return error
}

//  Partition helper function for QuickSelect
template <typename elmtype>
int CircularDynamicArray<elmtype>::QuickPartition(int l, int r) {
    //  Sets pivot to random value between l and r, then swaps pivot to end of subarray
    int random = l + rand() % (r - l + 1);
    elmtype temp = data[(front + random) % cap];
    data[(front + random) % cap] = data[(front + r) % cap];
    data[(front + r) % cap] = temp;

    elmtype pivot = data[(front + r) % cap];
    int i = l;

    for (int j = l; j <= r - 1; j++) {
        /*  If element at position j is less than or equal to pivot,
            swap elements at i and j and increase i by 1            */
        if (data[(front + j) % cap] <= pivot) {
            temp = data[(front + i) % cap];
            data[(front + i) % cap] = data[(front + j) % cap];
            data[(front + j) % cap] = temp;
            i++;
        }
    }

    //  Swaps elements at i and end of subarray
    temp = data[(front + i) % cap];
    data[(front + i) % cap] = data[(front + r) % cap];
    data[(front + r) % cap] = temp;
    return i;
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

//  Recursive helper function for stableSort
template <typename elmtype>
void CircularDynamicArray<elmtype>::mergeSort(int l, int r) {
    if (l >= r) return;

    int mid = l + (r - l) / 2;
    mergeSort(l, mid);      //  left subarray
    mergeSort(mid + 1, r);  //  right subarray
    merge(l, mid, r);
}

//  Merges left and right subarrays created in mergeSort
template <typename elmtype>
void CircularDynamicArray<elmtype>::merge(int l, int m, int r) {
    int sizeOne = m - l + 1;
    int sizeTwo = r - m;

    elmtype* leftArray = new elmtype[sizeOne];
    elmtype* rightArray = new elmtype[sizeTwo];

    for (int i = 0; i < sizeOne; i++)   leftArray[i] = data[(front + l + i) % cap];
    for (int i = 0; i < sizeTwo; i++)   rightArray[i] = data[(front + m + 1 + i) % cap];

    int indexOne = 0;
    int indexTwo = 0;
    int indexMerge = l;

    while (indexOne < sizeOne && indexTwo < sizeTwo) {
        if (leftArray[indexOne] <= rightArray[indexTwo]) {
            data[(front + indexMerge) % cap] = leftArray[indexOne];
            indexOne++;
        } else {
            data[(front + indexMerge) % cap] = rightArray[indexTwo];
            indexTwo++;
        }

        indexMerge++;
    }

    while (indexOne < sizeOne) {
        data[(front + indexMerge) % cap] = leftArray[indexOne];
        indexOne++;
        indexMerge++;
    }

    while (indexTwo < sizeTwo) {
        data[(front + indexMerge) % cap] = rightArray[indexTwo];
        indexTwo++;
        indexMerge++;
    }
}