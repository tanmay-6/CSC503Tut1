#include<bits/stdc++.h>
using namespace std;

vector<vector<int>> generate_graph(int n, int m){
    vector<vector<int>> adj(n);
    set<pair<int,int>> uniqedg;
    srand(time(0));
    while(uniqedg.size() < m){
        int u= rand()%n;
        int v = rand()%n;
        if(u>v) swap(u,v);
        if(u==v) continue;
        if(uniqedg.count({u,v}) || uniqedg.count({v,u})) continue;
        uniqedg.insert({u,v});
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    return adj;
}

void printGraph(vector<vector<int>> &adj){
    int n = adj.size();
    for(int i=0; i<n; i++){
        cout<<i<<" ";
        for(auto j : adj[i]){
            cout<<j<<" ";
        }
        cout<<endl;
    }
}

void dfsrHelper(vector<vector<int>> &adj, vector<bool>& visited, int node = 0){
    visited[node] = true;
    cout<<node<<" ";
    for(int neigh : adj[node]){
        if(!visited[neigh]){
            dfsrHelper(adj, visited, neigh);
        }
    }

    return;
}

void dfsr(vector<vector<int>> &adj){
    int n = adj.size();
    vector<bool> visited(n, false);
    dfsrHelper(adj, visited);
    cout<<endl;
}

void dfsi(vector<vector<int>> &adj, int node = 0){
    int n = adj.size();
    vector<bool> visited(n, false);
    stack<int> s;
    s.push(node);
    while(!s.empty()){
        int curr = s.top();
        s.pop();
        if(!visited[curr]){
            visited[curr] = true;
            cout<<curr<<" ";
        }

        for (auto it = adj[curr].rbegin(); it != adj[curr].rend(); it++) {
            if (!visited[*it]) {
                s.push(*it);
            }
        }
    }
    cout<<endl;
}

void bfs(vector<vector<int>> &adj, int node = 0){
    int n = adj.size();
    vector<bool> visited(n,false);
    queue<int> q;

    visited[node] = true;
    q.push(node);
    while(!q.empty()){
        int curr = q.front();
        q.pop();
        cout<<curr<<" ";
        for(int neigh : adj[curr]){
            if(!visited[neigh]){
                visited[neigh] = true;
                q.push(neigh);
            }
        }
    }
    cout<<endl;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        cout<<"wrong command line input"<<endl;
        return 1;
    }
    srand(time(NULL));
    int numOfNodes;
    int numOfEdges;
    vector<vector<int>> myGraph;
    #ifdef VARN
        char sparsity = argv[1][0];
        numOfNodes = 100+rand()%900;
        if(sparsity == 'a'){
            numOfEdges = 2*numOfNodes;
        }
        else if(sparsity == 'b'){
            numOfEdges = numOfNodes*log(numOfNodes);
        }
        else if(sparsity == 'c'){
            numOfEdges = numOfNodes*sqrt(numOfNodes);
        }
        else if(sparsity == 'd'){
            numOfEdges = (numOfNodes*(numOfNodes-1))/2;
        }
        myGraph = generate_graph(numOfNodes,numOfEdges);
    #endif

    #ifdef VARM
        numOfNodes = stoi(argv[1]);
        int high = (numOfNodes*(numOfNodes-1))/2;
        int low = 2*numOfNodes;
        numOfEdges = low+rand()%(high-low+1);
        myGraph = generate_graph(numOfNodes, numOfEdges);
    #endif

    #ifdef DFSI
    cout<<"DfSI : ";
    dfsi(myGraph);
    #endif
    #ifdef DFSR
    cout<<"DFSR : ";
    dfsr(myGraph);
    #endif
    #ifdef BFS
    cout<<"BFS : ";
    bfs(myGraph);
    #endif
    
    return 0;
}