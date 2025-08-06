#include<iostream>
#include<bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;

class Graph{
protected:
    int n;
public:
    Graph(int nodes){
        n = nodes;
    }
    virtual void bfs(int src){}
    virtual void dfs(int src){}
    virtual void djk(int src){}
    virtual void prm(int src){}
    virtual void wbfs(int src){}

};

int main(int argc, char* argv[]) {
    
    return 0;
}