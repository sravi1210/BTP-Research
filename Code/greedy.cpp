#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

int main(){
	ll V, E, R;          // V - Vertex, E - Edges,  R - Destination Edge Servers.
	cin>>V>>E>>R;

	ll d_limit, gamma;   // EDD time constraint limit - d_limit and ratio of cost of C2E and 1-hop E2E transmission - gamma. 
	cin>>d_limit>>gamma;

	vector<vector<ll>> G(V+1, vector<ll>());      // Graph G(V, E).
	vector<ll> destination_edge;

	for(ll i=0;i<E;i++){        // Create the graph with given edges.
		ll node1, node2;
		cin>>node1>>node2;
		G[node1].push_back(node2);
		G[node2].push_back(node1);
	}

	for(ll i=1;i<=V;i++){      // Add edges from cloud 'c' to each edge server 'v'.
		G[0].push_back(i);
		G[i].push_back(0);
	}

	for(ll i=0;i<R;i++){    // Create an array of destination edge servers.
		ll node;
		cin>>node;
		destination_edge.push_back(node);
	}





	return 0;
}