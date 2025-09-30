#include<iostream>
#include<bits/stdc++.h>
using namespace std;

void heapify(vector<int> &arr, int n, int i){
    int largest = i;
    int l = 2*i+1;
    int r = l+1;
    if(l<n && arr[l]>arr[largest]){
        largest = l;
    }
    if(r<n && arr[r]>arr[largest]){
        largest = r;
    }

    if(largest != i){
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapsort(vector<int> &arr, int n){
    for(int i = n/2 -1; i>=0; i--){
        heapify(arr, n, i);
    }

    for(int i=n-1; i>0; i--){
        swap(arr[i], arr[0]);
        heapify(arr, i, 0);
    }
}

int main(){
    vector<int> arr = {5,3,4,6,9,10,8};
    for(int i=0; i<arr.size(); i++) cout<<arr[i]<<" ";
    cout<<endl;
    heapsort(arr, arr.size());
    for(int i=0; i<arr.size(); i++) cout<<arr[i]<<" ";
    cout<<endl;
    return 0;
}   