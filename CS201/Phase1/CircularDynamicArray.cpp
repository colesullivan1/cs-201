/*  Author:     Cole Sullivan
    Due Date:   September 18, 2022
    Class:      CS 201-002
    Assignment: Build a circular dynamic array based on the methods described
                in lecture and in the Introduction to Algorithms textbook   */

#include <iostream>
#include <cmath>

using namespace std;

//  Circular Dynamic Array Class
/*  NOTE:   When accessing an element of data, the index should be adjusted
            by the formula (front + index) % capacity. This is because the
            array is circular, and the front of the array may not be at index
            0.  */
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

        void swap(int x, int y);

    private:
        int cap;        //  Maximum capacity of the array before resizing
        int size;       //  Current size of the array
        elmtype* data;  //  Pointer to the array
        int front;      //  Index of the front of the array
        elmtype error;  //  Error value

        elmtype QuickSelect(int l, int r, int k);
        int QuickPartition(int l, int r);

        int WCSelect(int l, int r, int k);
        int WCPivot(int l, int r);
        int WCPartition(int l, int r, int pivotIndex, int k);
        int WCPartition5(int l, int r);

        void mergeSort(int l, int r);
        void merge(int l, int m, int r);
};

//  Default constructor
template <typename elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray() {
    cap = 2;
    size = 0;
    data = new elmtype[cap];
    front = 0;
}

//  Overloaded constructor, constructs cda with capacity and size s
template <typename elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray(int s) {
    cap = s;
    size = s;
    data = new elmtype[s];
    front = 0;
}

//  Destructor
template <typename elmtype>
CircularDynamicArray<elmtype>::~CircularDynamicArray() {
    delete[] data;
}

//  Copy constructor
template <typename elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray(const CircularDynamicArray &cda) {
    cap = cda.cap;
    size = cda.size;
    data = new elmtype[cap];
    front = 0;

    for (int i = 0; i < cda.size; i++)  addEnd(cda.data[i]);    //  Deep copies cda.data to data
}

//  Copy assignment operator
template <typename elmtype>
CircularDynamicArray<elmtype>& CircularDynamicArray<elmtype>::operator=(const CircularDynamicArray &cda) {
    cap = cda.cap;
    size = 0;
    data = new elmtype[cap];
    front = 0;

    for (int i = 0; i < cda.size; i++)  addEnd(cda.data[i]);    //  Deep copies cda.data to data

    return *this;
}

//  Returns reference to element stored at position i in the circular dynamic array
template <typename elmtype>
elmtype& CircularDynamicArray<elmtype>::operator[](int i) {
    //  If i exists, return reference to element at position i
    if (i >= 0 && i < size) return data[(front + i) % cap];
    else {
        //  Else, return reference to error
        cout << "Error: index " << i << " is out of bounds" << endl;
        return error;
    }
}

//  Adds element v to end of circular dynamic array
template <typename elmtype>
void CircularDynamicArray<elmtype>::addEnd(elmtype v) {
    if (size >= cap) {
        /*  If size is greater than or equal to capacity,
            increase size by 1 and double capacity      */

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

        //  Set front to 0, double capacity, increase size by 1
        //  and copy temp to data
        front = 0;
        cap *= 2;
        data = new elmtype[cap];
        data = temp;
        size++;
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
    front = (front + 1) % cap;  //  Move front to next element
    size--;

    if (((double)size / (double)cap) <= 0.25) {
        //  If size is less than or equal to 1/4 of capacity, halve capacity

        //  Create temp array and copy old data values to temp
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

//  Returns kth smallest element using the quick select algorithm
template <typename elmtype>
elmtype CircularDynamicArray<elmtype>::QuickSelect(int k) {
    return QuickSelect(0, size - 1, k);
}

//  Returns kth smallest element using the median of medians algorithm
template <typename elmtype>
elmtype CircularDynamicArray<elmtype>::WCSelect(int k) {
    return data[(front + WCSelect(0, size - 1, k - 1)) % cap];
}


//  Sorts circular dynamic array using the merge sort algorithm
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
    int mid = (left + right) / 2;

    while (left <= right) {
        /*  If value at middle is equal to e, return middle
            Else if value at middle is less than e, left gets middle + 1
            Else, right gets middle - 1                                 */
        if (data[(front + mid) % cap] == e)     return mid;
        else if (data[(front + mid) % cap] < e) left = mid + 1;
        else                                    right = mid - 1;

        mid = (left + right) / 2;
    }

    return -1;
}

//  Swaps elements at indices x and y
template <typename elmtype>
void CircularDynamicArray<elmtype>::swap(int x, int y) {
    elmtype temp = data[(front + x) % cap];
    data[(front + x) % cap] = data[(front + y) % cap];
    data[(front + y) % cap] = temp;
}

//  Recursive helper function for QuickSelect()
template <typename elmtype>
elmtype CircularDynamicArray<elmtype>::QuickSelect(int l, int r, int k) {
    if (k > 0 && k <= r - l + 1) {
        int pivotIndex = QuickPartition(l, r);   //  Partition array

        /*  If pivot index is equal to k, return value at index
            Else, if pivot index is greater than k, recursively searches left half of array
            Else, recursively searches right half of array  */
        if (pivotIndex - l == k - 1)     return data[(front + pivotIndex) % cap];
        else if (pivotIndex - l > k - 1) return QuickSelect(l, pivotIndex - 1, k);
        else                        return QuickSelect(pivotIndex + 1, r, k - pivotIndex + l - 1);
                                    //  k must be adjusted to account for the elements that were removed from the array
    }

    return error;   //  If k is not a valid value, return error
}

//  Partitions array into two halves, one with elements less than pivot and one with elements greater than pivot
template <typename elmtype>
int CircularDynamicArray<elmtype>::QuickPartition(int l, int r) {
    //  Sets pivot to random value between l and r, then swaps pivot to end of subarray
    int random = l + rand() % (r - l + 1);
    swap(random, r);

    elmtype pivot = data[(front + r) % cap];    //  Sets pivot to last element in subarray

    //  Iterates through subarray, swapping elements less than or equal to pivot to front of subarray
    int i = l;
    for (int j = l; j <= r - 1; j++) {
        /*  If element at position j is less than or equal to pivot,
            swap elements at i and j and increase i by 1            */
        if (data[(front + j) % cap] <= pivot) {
            swap(i, j);
            i++;
        }
    }

    swap(i, r); //  Swaps pivot to correct position
    return i;   //  Returns index of pivot
}

//  Recursive helper function for WCSelect()
template <typename elmtype>
int CircularDynamicArray<elmtype>::WCSelect(int l, int r, int k) {
    while (1) {
        if (l == r) return l;   //  If subarray is size 1, return index

        int pivotIndex = WCPivot(l, r); //  Sets pivot to median of medians
        pivotIndex = WCPartition(l, r, pivotIndex, k);  //  Partitions array around pivot

        /*  If pivot index is equal to k, return pivot index
            Else, if pivot index is greater than k, partitions and searches left half of array
            Else, partitions and searches right half of array                                      */
        if (k == pivotIndex)        return pivotIndex;
        else if (k < pivotIndex)    r = pivotIndex - 1;
        else                        l = pivotIndex + 1;
    }
}

//  Finds pivot using the median of medians algorithm
template <typename elmtype>
int CircularDynamicArray<elmtype>::WCPivot(int l, int r) {
    //  If subarray is less than 5 elements, returns median of subarray
    if (r - l < 5)  return WCPartition5(l, r);

    //  Otherwise, moves medians of each group of 5 elements to front of subarray
    for (int i = l; i <= r; i += 5) {
        //  Finds right bound of group of 5 elements
        int subR = i + 4;
        if (subR > r)   subR = r;

        int median5 = WCPartition5(i, subR);    //  Finds median of group of 5 elements
        swap(median5, l + floor((i - l) / 5));  //  Moves median to front of subarray
    }

    //  Recursively computes median of medians
    int mid = (r - l) / 10 + l;
    return WCSelect(l, l + floor((r - l) / 5), mid);
}

//  Partitions array into 3 groups: less than pivot, equal to pivot, and greater than pivot
template <typename elmtype>
int CircularDynamicArray<elmtype>::WCPartition(int l, int r, int pivotIndex, int k) {
    //  Swaps pivot to end of subarray
    elmtype pivotValue = data[(front + pivotIndex) % cap];
    swap(pivotIndex, r);
    int storeIndex = l; //  Stores index of first element in less than group

    //  Partitions array into 3 groups: less than pivot, equal to pivot, and greater than pivot
    for (int i = l; i < r; i++) {
        if (data[(front + i) % cap] < pivotValue) {
            swap(storeIndex, i);    //  Swaps element at i to front of less than group
            storeIndex++;           //  By end, store index should be first element in equal to group
        }
    }
    int storeIndexEq = storeIndex;  //  Stores index of first element in equal to group
    for (int i = storeIndex; i < r; i++) {
        if (data[(front + i) % cap] == pivotValue) {
            swap(storeIndexEq, i);  //  Swaps element at i to front of equal to group
            storeIndexEq++;
        }
    }
    swap(r, storeIndexEq);  //  Swaps pivot to front of greater than group

    /*  If k is less than storeIndex, returns storeIndex
        Else, if k is less than or equal to storeIndexEq, returns k
        Else, returns storeIndexEq                                          */
    if (k < storeIndex)         return storeIndex;
    else if (k <= storeIndexEq) return k;
    return storeIndexEq;
}

//  Selects the median of at most 5 elements
template <typename elmtype>
int CircularDynamicArray<elmtype>::WCPartition5(int l, int r) {
    //  Iterates through subarray and sorts elements in ascending order
    int i = l;
    while (i < r) {
        int j = i;
        while (j > l && data[(front + j - 1) % cap] > data[(front + j) % cap]) {
            swap(j - 1, j);
            j--;
        }
        i++;
    }
    return floor((l + r) / 2);  //  Returns index of median
}

//  Sorts cda using the merge sort algorithm
template <typename elmtype>
void CircularDynamicArray<elmtype>::mergeSort(int l, int r) {
    if (l >= r) return; //  If subarray is size 1, returns

    int mid = l + (r - l) / 2;
    mergeSort(l, mid);      //  Recursively sorts left subarray
    mergeSort(mid + 1, r);  //  Recursively sorts right subarray
    merge(l, mid, r);       //  Merges sorted subarrays
}

//  Merges left and right subarrays created in mergeSort()
template <typename elmtype>
void CircularDynamicArray<elmtype>::merge(int l, int m, int r) {
    //  Creates temporary array to store sorted subarrays
    int sizeOne = m - l + 1;
    int sizeTwo = r - m;
    elmtype* leftArray = new elmtype[sizeOne];
    elmtype* rightArray = new elmtype[sizeTwo];

    //  Copies elements from cda to left and right subarrays
    for (int i = 0; i < sizeOne; i++)   leftArray[i] = data[(front + l + i) % cap];
    for (int i = 0; i < sizeTwo; i++)   rightArray[i] = data[(front + m + 1 + i) % cap];

    //  Merges left and right subarrays into cda
    int indexOne = 0;
    int indexTwo = 0;
    int indexMerge = l;
    while (indexOne < sizeOne && indexTwo < sizeTwo) {
        if (leftArray[indexOne] <= rightArray[indexTwo]) {
            //  If element in left subarray is less than or equal to element in right subarray, copies element to cda
            data[(front + indexMerge) % cap] = leftArray[indexOne];
            indexOne++;
        } else {
            //  If element in right subarray is less than element in left subarray, copies element to cda
            data[(front + indexMerge) % cap] = rightArray[indexTwo];
            indexTwo++;
        }

        indexMerge++;
    }

    //  Copies remaining elements from left subarray to cda
    while (indexOne < sizeOne) {
        data[(front + indexMerge) % cap] = leftArray[indexOne];
        indexOne++;
        indexMerge++;
    }

    //  Copies remaining elements from right subarray to cda
    while (indexTwo < sizeTwo) {
        data[(front + indexMerge) % cap] = rightArray[indexTwo];
        indexTwo++;
        indexMerge++;
    }
}