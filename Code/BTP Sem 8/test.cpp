/**
 *    author:  sravi1210
 *    created: 07.02.2021 01:33:23       
**/

#include <bits/stdc++.h>
using namespace std;

#define F first
#define S second
#define ll long long int
#define CETC 100 // Cloud to Edge Transmission Cost.

// Global variables for Graph, EDDA, CMST, destination edge servers, CMST nodes and edges.
vector<vector<ll>> G;
map<ll, ll> destination_edge;
map<pair<ll, ll>, ll> weights;


// Function to initialize global variables.
void initialize(ll V){
	G.clear();
	weights.clear();
	destination_edge.clear();

	vector<ll> xtemp;
	for(ll i=0;i<=V;i++){
		vector<ll> temp(V+1, -1);
		G.push_back(temp);
	}
	return;
}

void readInputGraph(ll V, ll E, ll R, ll gamma){
	for(ll i=0;i<E;i++){        // Create the graph with given edges.
		ll node1, node2, w;
		cin>>node1>>node2>>w;
		G[node1][node2] = 1;
		G[node2][node1] = 1;
		weights[{node1, node2}] = w;
		weights[{node2, node1}] = w;
	}

	for(ll i=0;i<=V;i++){    // Set the weights of edges between cloud and edge servers to some comstant default value.
		weights[{0, i}] = CETC;
		weights[{i, 0}] = CETC;
		weights[{0, 0}] = 0;
	}

	for(ll i=0;i<R;i++){    // Create a map of destination edge servers.
		ll node;
		cin>>node;
		destination_edge[node] = 1;
	}

	return;
}

void dfs(vector<bool> &visited, ll index, ll V){
	if(visited[index]){
		return;
	}
	visited[index] = true;
	for(int i=0;i<=V;i++){
		if(G[index][i]==1 && !visited[i]){
			dfs(visited, i, V);
		}
	}
	return;
}

int main(){
	ll V, E, R;          // V - Vertex, E - Edges,  R - Destination Edge Servers.
	cin>>V>>E>>R;

	ll d_limit, gamma;   // EDD time constraint limit - d_limit and ratio of cost of C2E and 1-hop E2E transmission - gamma. 
	cin>>d_limit>>gamma;

	initialize(V);
	readInputGraph(V, E, R, gamma);
	
	vector<bool> visited(V+1, false);
	dfs(visited, 1,  V);

	for(int i=1;i<=V;i++){
		if(!visited[i]){
			cout<<i<<": Not Connected"<<endl;
		}
	}
	return 0;
}