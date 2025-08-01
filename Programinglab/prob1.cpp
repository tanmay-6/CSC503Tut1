#include<bits/stdc++.h>
#include<iostream>
using namespace std;
typedef pair<int,int> pii;


void djk(int src, vector<vector<pii>> &adj){
    vector<int> dist(adj.size(), INT_MAX);
    vector<int> parent(adj.size(),-1);
    
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    dist[src] = 0;
    pq.push({0,src});
    
    while(!pq.empty()){
        int dis = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        for(auto it : adj[node]){
            int ew = it.second; //ew -> edgeweight
            int adjnode = it.first;
            
            if(dis + ew < dist[adjnode]){
                dist[adjnode] = dis + ew;
                parent[adjnode] = node;
                pq.push({dist[adjnode], adjnode});
            }
        }
    }

    for(int i=1; i<adj.size(); i++){
        cout<<parent[i]<<" "<<i<<endl;
    }
}

//BFS
void bfs(int src, vector<vector<int>> &adj){
    queue<int> q;
    vector<int> visited(adj.size(),0);
    visited[src] = 1;
    int level = 0;
    q.push(src);
    while(!q.empty()){
        int size = q.size();
        for(int i=0; i<size; i++){
            int top = q.front();
            cout<<top<<endl;
            q.pop();
            for( auto neigh : adj[top]){
                if(visited[neigh] == 0){
                    q.push(neigh);
                    visited[neigh] = 1;
                }
            }
        }
        level++;
    }
}

//DFS
void dfs(int src, vector<vector<int>> &adj){
    stack<int> st;
    vector<int> visited(adj.size(), 0);
    st.push(src);
    visited[src] = 1;

    while(!st.empty()){
        int node = st.top();
        st.pop();
        cout<<node<<endl;

        for(auto it : adj[node]){
            if(visited[it] == 0){
                st.push(it);
                visited[it] = 1;
            }
        }
        
    }
    cout<<endl;
}

//0/1BFS
void wbfs(int src, vector<vector<pii>> &adj){
    vector<int> dist(adj.size(), INT_MAX);
    vector<int> parent(adj.size(), -1);
    dist[src] = 0;
    deque<int> dq;
    dq.push_front(src);
    
    while(!dq.empty()){
        int node = dq.front();
        dq.pop_front();
        for( auto it : adj[node]){
            int adjnode = it.first;
            int dis = it.second;
            
            if(dis + dist[node] < dist[adjnode]){
                dist[adjnode] = dis + dist[node];
                parent[adjnode] = node;
                
                if(dis == 0) dq.push_front(adjnode);
                else dq.push_back(adjnode);
            }
        }
    }
    
    for(int i=0; i<adj.size(); i++){
        cout<<i<<" "<<dist[i]<<" "<<parent[i]<<endl;
    }
}

//Prim's 
void prm(int src, vector<vector<pii>> &adj){
    vector<int> wgt(adj.size(), INT_MAX), parent(adj.size(), -1);
    vector<int> visited(adj.size(), 0);
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    wgt[0] = 0;
    pq.push({0,0});
    
    while(!pq.empty()){
        int node = pq.top().second;
        pq.pop();
        visited[node] = true;
        
        for(auto it : adj[node]){
            int ew = it.second;
            int adjnode = it.first;
            
            if(visited[adjnode] == 0 && ew<wgt[adjnode]){
                wgt[adjnode] = ew;
                parent[adjnode] = node;
                pq.push({wgt[adjnode], adjnode});
            }
        }
    }
    cout << "E - W"<<endl;
    int total = 0;
    for (int i = 1; i < adj.size(); ++i) {
        if (parent[i] != -1) {
            cout << parent[i] << " " << i << " " << wgt[i] << "\n";
            total += wgt[i];
        }
    }
    cout<<"Total: "<<total<<endl;
}

int main(){
    int n,m;
    cin>>n>>m;
    
    #if defined(DIR)
        #if defined(BFS) || defined(DFS)
            cout<<"unweighted directed"<<endl;
            vector<vector<int>> adj(n+1);
            for(int i=0; i<m; i++){
                int u,v;
                cin>>u>>v;
                adj[u].push_back(v);
            }
            #ifdef BFS
                bfs(0,adj);
            #elif DFS
                dfs(0,adj);
            #else
                cout<<"Unweighted graph allows BFS and DFS only"<<endl;
            #endif
        #else
            cout<<"weighted directed"<<endl;
            vector<vector<pair<int,int>>> adj(n+1);
            for(int i=0; i<m; i++){
                int u,v,w;
                cin>>u>>v>>w;
                adj[u].push_back({v,w});
            }
            #ifdef DJK
                djk(0,adj);
            #elif WBFS
                wbfs(0,adj);
            #endif
        #endif
    #else
        #if defined(BFS) || defined(DFS)
            cout<<"unweighted undirected"<<endl;
            vector<vector<int>> adj(n+1);
            for(int i=0; i<m; i++){
                int u,v;
                cin>>u>>v;
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
            #ifdef BFS
                bfs(0,adj);
            #elif DFS
                dfs(0,adj);
            #else
                cout<<"Unweighted graph allows BFS and DFS only"<<endl;
            #endif
        #else
            cout<<"weighted undirected"<<endl;
            vector<vector<pair<int,int>>> adj(n+1);
            for(int i=0; i<m; i++){
                int u,v,w;
                cin>>u>>v>>w;
                adj[u].push_back({v,w});
                adj[v].push_back({u,w});
            }
            #ifdef DJK
                djk(0,adj);
            #elif WBFS
                wbfs(0,adj);
            #elif PRM
                prm(adj);
            #endif
        #endif
    #endif

    return 0;
}