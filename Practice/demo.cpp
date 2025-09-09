#include<bits/stdc++.h>
using namespace std;

void merge(vector<int> &arr, int low, int mid, int high){
    vector<int> brr;
    int i=low;
    int j=mid+1;
    while(i<=mid && j<=high){
        if(arr[i]<=arr[j]){
            brr.push_back(arr[i++]);
        }
        else{
            brr.push_back(arr[j++]);
        }
    }
    while(i<=mid){
        brr.push_back(arr[i++]);
    }
    while(j<=high){
        brr.push_back(arr[j++]);
    }
    for(int a=0; a<brr.size(); a++){
        arr[low+a] = brr[a];
    }
}

void mergesort(vector<int> &arr, int low, int high){
    if(low >= high) return;
    int mid = low + (high-low)/2;
    mergesort(arr, low, mid);
    mergesort(arr, mid+1, high);
    merge(arr, low, mid, high);
}

int partition(vector<int> &arr, int low, int high){
    int pivot = arr[low];
    int i = low;
    int j = high;

    while(i<j){
        while(arr[i]<= pivot && i<high){
            i++;
        }
        while(arr[j]> pivot && j>low){
            j--;
        }
        if(i<j) swap(arr[i], arr[j]);
    }
    swap(arr[low], arr[j]);
    return j;
}

void quicksort(vector<int> &arr, int low, int high){
    if(low < high){
        int partitionIndex = partition(arr, low, high);
        quicksort(arr, low, partitionIndex-1);
        quicksort(arr, partitionIndex+1, high);
    }
}

void heapify(vector<int> &arr, int n, int i){
    int largest = i;
    int left = 2*i+1;
    int right = 2*i+2;
    if(left <n && arr[left] > arr[largest]){
        largest = left;
    }
    if(right<n && arr[right] > arr[largest]){
        largest = right;
    }
    if(largest != i){
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapsort(vector<int> &arr, int n){
    for(int i=n/2; i>=0; i--){
        heapify(arr, n, i);
    }
    for(int i=arr.size()-1; i>0; i--){
        swap(arr[i], arr[0]);
        heapify(arr, i, 0);
    }
}

int main(){
    vector<int> arr = {6,5,21,16,32,33,12,9,48,65,76,28};
    //mergesort(arr, 0, arr.size()-1);
    //quicksort(arr, 0, arr.size()-1);
    heapsort(arr, arr.size());
    for(int i=0; i<arr.size(); i++) cout<<arr[i]<<" ";
    return 0;
}