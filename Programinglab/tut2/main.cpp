#include<bits/stdc++.h>
using namespace std;

void merge(vector<int>& arr, int l, int m, int r){
    vector<int> left(arr.begin()+l, arr.begin()+m+1);
    vector<int> right(arr.begin()+m+1, arr.begin()+r+1);
    int i=0, j=0, k=l;
    while(i < left.size() && j < right.size())
        arr[k++] = (left[i] < right[j]) ? left[i++] : right[j++];
    while(i < left.size()) arr[k++] = left[i++];
    while(j < right.size()) arr[k++] = right[j++];
}
void mergeSortHelper(vector<int>& arr, int l, int r){
    if(l<r){
        int m=l+(r-l)/2;
        mergeSortHelper(arr, l,m);
        mergeSortHelper(arr, m+1, r);
        merge(arr,l,m,r);
    }
}
void mergeSort(vector<int>& arr){
    mergeSortHelper(arr, 0, arr.size()-1);
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high], i = low - 1;
    for(int j = low; j < high; j++)
        if(arr[j] < pivot) swap(arr[++i], arr[j]);
    swap(arr[i+1], arr[high]);
    return i + 1;
}

void quickSortHelper(vector<int>& arr, int low, int high) {
    if(low < high) {
        int pi = partition(arr, low, high);
        quickSortHelper(arr, low, pi - 1);
        quickSortHelper(arr, pi + 1, high);
    }
}

void quickSort(vector<int>& arr) {
    quickSortHelper(arr, 0, arr.size() - 1);
}

int randomizedPartition(vector<int>& arr, int low, int high) {
    int random = low + rand() % (high - low + 1);
    swap(arr[random], arr[high]);
    return partition(arr, low, high);
}

void randomizedQuickSortHelper(vector<int>& arr, int low, int high) {
    if(low < high) {
        int pi = randomizedPartition(arr, low, high);
        randomizedQuickSortHelper(arr, low, pi - 1);
        randomizedQuickSortHelper(arr, pi + 1, high);
    }
}

void randomizedQuickSort(vector<int>& arr) {
    randomizedQuickSortHelper(arr, 0, arr.size() - 1);
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if(l < n && arr[l] > arr[largest]) largest = l;
    if(r < n && arr[r] > arr[largest]) largest = r;
    if(largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();
    for(int i = n/2 - 1; i >= 0; i--) heapify(arr, n, i);
    for(int i = n-1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for(int i = 0; i < n-1; i++)
        for(int j = 0; j < n-i-1; j++)
            if(arr[j] > arr[j+1])
                swap(arr[j], arr[j+1]);
}

void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for(int i = 0; i < n-1; i++) {
        int min_idx = i;
        for(int j = i+1; j < n; j++)
            if(arr[j] < arr[min_idx])
                min_idx = j;
        if(min_idx != i)
            swap(arr[i], arr[min_idx]);
    }
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <number_of_elements>" << endl;
        return 1;
    }
    int n = atoi(argv[1]);
    cout<<n<<endl;

    srand(time(NULL));
    vector<int> arr(n);
    // for (int i = 0; i < n; ++i) {
    //     arr[i] = rand();
    //     cout<<arr[i]<<" ";
    // }
    // cout<<endl;

    #ifdef MERGE_SORT
        mergeSort(arr);
    #elif defined(QUICK_SORT)
        quickSort(arr);
    #elif defined(RANDOMIZED_QUICK_SORT)
        randomizedQuickSort(arr);
    #elif defined(HEAP_SORT)
        heapSort(arr);
    #elif defined(BUBBLE_SORT)
        bubbleSort(arr);
    #elif defined(SELECTION_SORT)
        selectionSort(arr);
    #else
        cerr << "No sorting algorithm selected!" << endl;
        return 2;
    #endif

    // for(int i=0; i<arr.size(); i++){
    //     cout<<arr[i]<<" ";
    // }
    // cout<<endl;
    cout<<"sorting done"<<endl;
    return 0;
}