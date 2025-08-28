#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

#define N 100000
#define M 10 //2.M denoting number of threads
int currIndex =0; //3.current index in vector
int llIndex =0;
vector<int> v(N);
pthread_mutex_t lockVar; 
pthread_cond_t condVar;

class Node{
public:
    int val;
    Node* next;

    Node(int value){
        val = value;
        next = nullptr;
    }
};

class LinkedList{
public:
    Node* head;
    Node* tail;
    
    LinkedList(){
        head = nullptr;
        tail = nullptr;
    }

    void insert_tail(int val){ // 4.insertion in linkedlist
        Node* temp = new Node(val);
        if(!head){
            head = temp;
            tail = temp;
            return;
        }
        tail->next = temp;
        tail = temp;
    }

    Node* getHead(){
        return head;
    }

    void display(){
        Node* temp = head;
        while(temp){
            cout<<temp->val<<" ";
            temp = temp->next;
        }
    }
};

LinkedList ll;

void* helper(void* arg){
    while(true){
        pthread_mutex_lock(&lockVar);
        if(currIndex >= v.size()){
            pthread_cond_broadcast(&condVar); // waiting threads would wake up
            pthread_mutex_unlock(&lockVar);
            break;
        }

        int value = v[currIndex];
        int x = currIndex;
        currIndex++;
        pthread_mutex_unlock(&lockVar);
        pthread_mutex_lock(&lockVar);
        while(llIndex != x){
            pthread_cond_wait(&condVar, &lockVar);
        }
        ll.insert_tail(value);
        llIndex++;
        pthread_cond_broadcast(&condVar);
        pthread_mutex_unlock(&lockVar);
    }
    return nullptr;
}

bool verifier(LinkedList* ll, vector<int> v){
    Node* curr = ll->getHead();
    for (int i=0; i<N; i++){
        //cout<<curr->val<<" "<<v[i]<<endl;
        if(!curr || curr->val != v[i]){
            return false;
        }
        else{
            curr = curr->next;
        }
    }
    return true;
}

int main(){
    ifstream readfile("random.txt");
    int num;
    for(int i=0; i<N; i++){
        if(readfile>>num){
            v[i]=num;
        }
    } // 1.reads input from file

    pthread_mutex_init(&lockVar, NULL);
    pthread_cond_init(&condVar, NULL);
    pthread_t threads[M];
    int ids[M];

    for(int i=0; i<M; i++){
        ids[i] = i+1;
        pthread_create(&threads[i], NULL, helper ,&ids[i]);
    }

    for(int i=0; i<M; i++){
        pthread_join(threads[i], NULL);
    }

    cout<<verifier(&ll, v)<<endl;
    pthread_mutex_destroy(&lockVar);
    pthread_cond_destroy(&condVar);

    return 0;
}