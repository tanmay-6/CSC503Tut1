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

vector<int> dijikstra(vector<vector<int>> &g, int src=0){
    int size = g.size();
    vector<int> distance(size, INT_MAX);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    distance[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > distance[u]) continue;
        for (auto &e : g[u]) {
            if (distance[u] + e.w < distance[e.to]) {
                distance[e.to] = distance[u] + e.w;
                pq.push({distance[e.to], e.to});
            }
        }
    }
    return distance;
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

int main(){
    int n=200; int m=983;
    vector<vector<int>> myGraph = generate_graph(n,m);
    vector<int> dij = dijikstra(myGraph);
    for(auto i : dij){
        cout<<i<<" ";
    }
    cout<<endl;
    bfs(myGraph);
    return 0;
}