#include<bits/stdc++.h>
using namespace std;

int main(){
    srand(time(NULL));
    int numOfNodes = 100+rand()%900;
    int numOfEdges = (numOfNodes*(numOfNodes-1))/2;
    cout<<numOfNodes<<" "<<numOfEdges;
    return 0;
}