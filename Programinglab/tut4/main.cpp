#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

#define N 5

void* printMessage(void* arg){
    const char* msg = static_cast<const char*>(arg);  // use const char*
    cout << msg << endl;
    return NULL;
}

int main(){
    // ifstream readfile("random.txt");
    // int num;
    // vector<int> v(N);
    // for(int i=0; i<N; i++){
    //     if(readfile>>num){
    //         v[i]=num;
    //     }
    // }

    pthread_t t1,t2;

    const char* msg1 = "Hello from Thread 1";
    const char* msg2 = "Hello from Thread 2";

    pthread_create(&t1, NULL, printMessage, (void*)msg1);
    pthread_create(&t2, NULL, printMessage, (void*)msg2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    cout<<"Both threads are finished"<<endl;

    return 0;
}