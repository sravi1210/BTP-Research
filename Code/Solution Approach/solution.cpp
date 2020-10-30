#include <bits/stdc++.h>
using namespace std;

#define S second
#define F first
#define ll long long int

// Global variables for Graph, EDDA, CMST, destination edge servers, CMST nodes and edges.
vector<vector<ll>> G;
map<ll, ll> destination_edge;

// Function to initialize global variables.
void initialize(ll V){
	G.clear();
	destination_edge.clear();

	for(ll i=0;i<=V;i++){
		vector<ll> temp;
		G.push_back(temp);
	}
	return;
}

int main(){
	ll V, E, R;          // V - Vertex, E - Edges,  R - Destination Edge Servers.
	cin>>V>>E>>R;

	ll d_limit, gamma;   // EDD time constraint limit - d_limit and ratio of cost of C2E and 1-hop E2E transmission - gamma. 
	cin>>d_limit>>gamma;

	d_limit += 1;

	initialize(V);

	for(ll i=0;i<E;i++){        // Create the graph with given edges.
		ll node1, node2;
		cin>>node1>>node2;
		G[node1].push_back(node2);
		G[node2].push_back(node1);
	}

	for(ll i=1;i<=V;i++){      // Add edges from cloud 'c' to each edge server 'v'.
		G[0].push_back(i);
	}
	
	for(ll i=0;i<R;i++){    // Create a map of destination edge servers.
		ll node;
		cin>>node;
		destination_edge[node] = 1;
	}

	return 0;
}