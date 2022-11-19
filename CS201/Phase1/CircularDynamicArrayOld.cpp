using namespace std;
#include <iostream>
#include <math.h>

template <typename generic>
class CircularDynamicArray {

    public:
        //Main functions
        CircularDynamicArray();
        CircularDynamicArray(int s);
        ~CircularDynamicArray();

        generic& operator[](int i);

        void addEnd(generic v);
        void addFront(generic v);

        void delEnd();
        void delFront();

        int length();
        int capacity();

        void clear();

        generic QuickSelect(int k);
        generic WCSelect(int k);

        void stableSort();

        int linearSearch(generic e);
        int binSearch(generic e);

        //Helper functions
        CircularDynamicArray(CircularDynamicArray &cda);
        void operator=(CircularDynamicArray &right);

        int QSPartition(int l, int r, int p);
        
        int MoMSelect(int l, int r, int k);
        int MoMPivot(int l, int r);
        int MoMPartition(int l, int r, int p, int n);
        int MoMPartition5(int l, int r);

    private: 
        int cap;
        int size;
        generic *data;
        int front;
        generic error;
};

template <typename generic>
CircularDynamicArray<generic>::CircularDynamicArray() {
    this->cap = 2;
    this->size = 0;
    this->data = new generic[2];
    this->front = 0;
    this->error = -1;
}

template <typename generic>
CircularDynamicArray<generic>::CircularDynamicArray(int s) {
    this->cap = s;
    this->size = s;
    this->data = new generic[s];
    this->front = 0;
    this->error = -1;
}

template <typename generic>
CircularDynamicArray<generic>::~CircularDynamicArray() {
    delete[] this->data;
}

template <typename generic>
generic& CircularDynamicArray<generic>::operator[](int i) {
    if (i >= 0 && i < this->size) {
        return this->data[(this->front + i) % this->cap];
    } else {
        cout << "Error: index " << i << " is out of bounds" << endl;
        return this->error;
    }
}

template <typename generic>
void CircularDynamicArray<generic>::addEnd(generic v) {
    if (this->size >= this->cap) {
        generic* temp = new generic[this->cap * 2];
        for (int i = 0; i < this->size; i++)    temp[i] = this->data[(this->front + i) % this->cap];
        delete[] this->data;

        this->data = temp;
        this->cap *= 2;
        this->front = 0;

        this->data[(this->front + this->size) % this->cap] = v;
        this->size++;
    } else {
        this->data[(this->front + this->size) % this->cap] = v;
        this->size++;
    }
}

template <typename generic>
void CircularDynamicArray<generic>::addFront(generic v) {
    if (this->size >= this->cap) {
        generic* temp = new generic[this->cap * 2];
        for (int i = 0; i < this->size; i++)    temp[i] = this->data[(this->front + i) % this->cap];
        delete[] this->data;

        this->data = temp;
        this->cap *= 2;
        this->front = 0;

        this->front = (this->front + (this->cap - 1)) % this->cap;
        this->data[this->front] = v;
        this->size++;
    } else {
        this->front = (this->front + (this->cap - 1)) % this->cap;
        this->data[this->front] = v;
        this->size++;
    }
}

template <typename generic>
void CircularDynamicArray<generic>::delEnd() {
    this->size--;

    if (((double)this->size) / ((double)this->cap) <= .25) {
        generic* temp = new generic[this->cap / 2];
        for (int i = 0; i < this->size; i++)    temp[i] = this->data[(this->front + i) % this->cap];
        delete[] this->data;

        this->data = temp;
        delete[] temp;

        this->cap /= 2;
        this->front = 0;
    }
}

template <typename generic>
void CircularDynamicArray<generic>::delFront() {
    this->front = (this->front + 1) % this->cap;
    this->size--;

    if (((float)this->size) / ((float)this->cap) <= .25) {
        generic* temp = new generic[this->cap / 2];
        for (int i = 0; i < this->size; i++)    temp[i] = this->data[(this->front + i) % this->cap];
        delete[] this->data;

        this->data = temp;
        delete[] temp;

        this->cap /= 2;
        this->front = 0;
    }
}

template <typename generic>
int CircularDynamicArray<generic>::length() {
    return this->size;
}

template <typename generic>
int CircularDynamicArray<generic>::capacity() {
    return this->cap;
}

template <typename generic>
void CircularDynamicArray<generic>::clear() {
    delete[] this->data;
    this->data = new generic[2];
    this->cap = 2;
    this->size = 0;
    this->front = 0;
}

template <typename generic>
generic CircularDynamicArray<generic>::QuickSelect(int k) {
    if (k - 1 < 0 || k - 1 > this->size - 1)    return this->error;

    int pivot = 0;
    int l = 0;
    int r = this->size - 1;

    pivot = this->QSPartition(l, r, 0);

    while (1) {
        if (pivot < k - 1) l = pivot + 1;
        else    r = pivot - 1;

        pivot = QSPartition(l, r, l);
        if (pivot == k - 1) break;
    }

    return this->data[(this->front + (k - 1)) % this->cap];
}

template <typename generic>
generic CircularDynamicArray<generic>::WCSelect(int k) {
    CircularDynamicArray<generic> temp = *this;
    
    return temp.data[temp.MoMSelect(0, this->size, k) + 1];
}

template <typename generic>
void CircularDynamicArray<generic>::stableSort() {
    if ((this->size) > 1) {
        CircularDynamicArray<generic> leftHalf;
        CircularDynamicArray<generic> rightHalf;

        for (int i = 0; i < this->size; i++) {
            if (i < this->size / 2) leftHalf.addEnd(this->data[(this->front + i) % this->cap]);
            else    rightHalf.addEnd(this->data[(this->front + i) % this->cap]);
        }

        leftHalf.stableSort();
        rightHalf.stableSort();
        
        int mainIndex = 0;
        int leftIndex = 0;
        int rightIndex = 0;

        while (leftIndex < leftHalf.length() && rightIndex < rightHalf.length()) {
            if (leftHalf.data[(leftHalf.front + leftIndex) % leftHalf.cap] < rightHalf.data[(rightHalf.front + rightIndex) % rightHalf.cap]) {
                this->data[(this->front + mainIndex) % this->cap] = leftHalf.data[leftIndex];
                leftIndex++;
            } else {
                this->data[(this->front + mainIndex) % this->cap] = rightHalf.data[rightIndex];
                rightIndex++;
            }
            mainIndex++;
        }

        while (leftIndex < leftHalf.length()) {
            this->data[(this->front + mainIndex) % this->cap] = leftHalf.data[leftIndex];
            leftIndex++;
            mainIndex++;
        }

        while (rightIndex < rightHalf.length()) {
            this->data[(this->front + mainIndex) % this->cap] = rightHalf.data[rightIndex];
            rightIndex++;
            mainIndex++;
        }
    }
}

template <typename generic>
int CircularDynamicArray<generic>::linearSearch(generic e) {
    for (int i = 0; i < this->size; i++)    if (this->data[(this->front + i) % this->cap] == e) return i;
    return -1;
}

template <typename generic>
int CircularDynamicArray<generic>::binSearch(generic e) {
    int l = this->front;
    int r = this->size - 1;
    int mid = 0;

    while (l <= r) {
        mid = (l + r) / 2;
        if (this->data[(this->front + mid) % this->cap] == e) return mid;
        else if (this->data[(this->front + mid) % this->cap] < e)   l = mid + 1;
        else    r = mid - 1;
    }
    return -1;
}

template <typename generic>
CircularDynamicArray<generic>::CircularDynamicArray(CircularDynamicArray &cda) {
    this->cap = cda.cap;
    this->size = cda.size;
    this->front = 0;
    this->error = -1;

    this->data = new generic[cda.cap];
    for (int i = 0; i < cda.size; i++)  this->data[i] = cda[i];
}

template <typename generic>
void CircularDynamicArray<generic>::operator=(CircularDynamicArray &right) {
    this->size = right.size;
    this->cap = right.cap;
    this->front = 0;
    this->error = -1;

    this->data = new generic[right.cap];
    for (int i = 0; i < right.size; i++)    this->data[i] = right.data[i];
}

template <typename generic>
int CircularDynamicArray<generic>::QSPartition(int l, int r, int p) {
    if (l >= r) return l;

    generic temp = this->data[(this->front + l) % this->cap];
    this->data[(this->front + l) % this->cap] = this->data[(this->front + p) % this->cap];
    this->data[(this->front + p) % this->cap] = temp;

    generic store = this->data[(this->front + l) % this->cap];
    int index = l + 1;

    while (1) {
        if (this->data[(this->front + index) % this->cap] <= store) {
            this->data[(this->front + (index - 1)) % this->cap] = this->data[(this->front + index) % this->cap];
            index++;

            if (index > r) {
                this->data[(this->front + (index - 1)) % this->cap] = store;
                return index - 1;
            }
        } else {
            temp = this->data[(this->front + r) % this->cap];
            this->data[(this->front + r) % this->cap] = this->data[(this->front + index) % this->cap];
            this->data[(this->front + index) % this->cap] = temp;
            r--;

            if (index > r) {
                this->data[(this->front + (index - 1)) % this->cap] = store;
                return index - 1;
            }
        }
    }
}

template <typename generic>
int CircularDynamicArray<generic>::MoMSelect(int l, int r, int k) {
    
    int pivotIndex;
    int left = l;
    int right = r;

    while (1) {
        if (left == right) return left;

        pivotIndex = this->MoMPivot(left, right);
        pivotIndex = this->MoMPartition(left, right, pivotIndex, k);

        if (pivotIndex == k)    return k;
        else if (pivotIndex >= k)   right = pivotIndex - 1;
        else    left = pivotIndex + 1;
    }
}

template <typename generic>
int CircularDynamicArray<generic>::MoMPivot(int l, int r) {
    if (r - l <= 5)  return this->MoMPartition5(l, r);

    for (int i = l; i <= r; i++) {
        int subRight = i + 4;
        if (subRight > r)   subRight = r;
        int median5 = this->MoMPartition5(i, subRight);

        generic temp = this->data[(this->front + median5) % this->cap];
        this->data[(this->front + median5) % this->cap] = this->data[(this->front + (l + (int)floor((i - l) / 5))) % this->cap];
        this->data[(this->front + (l + (int)floor((i - l) / 5))) % this->cap] = temp;
    }

    int m = (r - l) / 10 + l + 1;
    return this->MoMSelect(l, l + floor((r - l) / 5), m);
}

template <typename generic>
int CircularDynamicArray<generic>::MoMPartition(int l, int r, int p, int k) {
    generic pivotVal = this->data[(this->front + p) % this->cap];

    generic temp = this->data[(this->front + p) % this->cap];
    this->data[(this->front + p) % this->cap] = this->data[(this->front + r) % this->cap];
    this->data[(this->front + r) % this->cap] = temp;

    int store = l;
    
    for (int i = l; i <= r; i++) {
        if (this->data[(this->front + i) % this->cap] < pivotVal) {
            temp = this->data[(this->front + i) % this->cap];
            this->data[(this->front + i) % this->cap] = this->data[(this->front + store) % this->cap];
            this->data[(this->front + store) % this->cap] = temp;
            store++;
        }
    }

    int storeEQ = store;

    for (int i = store; i <= r; i++) {
        if (this->data[(this->front + i) % this->cap] == pivotVal) {
            temp = this->data[(this->front + i) % this->cap];
            this->data[(this->front + i) % this->cap] = this->data[(this->front + storeEQ) % this->cap];
            this->data[(this->front + storeEQ) % this->cap] = temp;
            storeEQ++;
        }
    }

    temp = this->data[(this->front + r) % this->cap];
    this->data[(this->front + r) % this->cap] = this->data[(this->front + storeEQ) % this->cap];
    this->data[(this->front + storeEQ) % this->cap] = temp;

    if (k < store)  return store;
    else if (k <= storeEQ)  return k;
    else    return storeEQ;
}

template <typename generic>
int CircularDynamicArray<generic>::MoMPartition5(int l, int r) {
    int i = l + 1;

    while (i <= r) {
        int j = i;
        
        while (j > l && this->data[(this->front + (j - 1)) % this->cap] > this->data[(this->front + j) % this->cap]) {
            generic temp = this->data[(this->front + (j - 1)) % this->cap];
            this->data[(this->front + (j - 1)) % this->cap] = this->data[(this->front + j) % this->cap];
            this->data[(this->front + j) % this->cap] = temp;
            j--;
        }

        i++;
    }

    return floor((l + r) / 2);
}