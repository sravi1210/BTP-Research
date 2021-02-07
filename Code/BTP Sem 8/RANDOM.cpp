/**
 *    author:  sravi1210
 *    created: 07.02.2021 15:30:09       
**/

#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

#define CETC 100 // Cloud to Edge Transmission Cost.

map<pair<ll, ll>, ll> weights;
map<pair<ll, ll>, ll> edges;

void update_connection(vector<vector<ll>> &G, vector<bool> &visited, map<ll, ll> &destination_edge, ll server, ll d_limit){
	deque<pair<ll, ll>> dq;
	dq.push_back({server, CETC});

	while(!dq.empty()){
		pair<ll, ll> p = dq.front();
		dq.pop_front();
		ll ind = p.first;
		ll height = p.second;
		visited[ind] = true;
		if(destination_edge.find(ind) != destination_edge.end() && destination_edge[ind] == 1){
			destination_edge[ind] = 2;
		}
		if(height >= d_limit){
			continue;
		}
		if(height < d_limit){
			ll connected = G[ind].size();
			for(ll j=0;j<connected;j++){
				ll child = G[ind][j];
				ll sum = weights[{ind, child}];
				if((height+sum) <= d_limit){
					dq.push_back({child, height+sum});
					edges[{min(child, ind), max(child, ind)}] = 1;
				}
			}
		}
	}
	return;
}

int main(){
	ll V, E, R;          // V - Vertex, E - Edges,  R - Destination Edge Servers.
	cin>>V>>E>>R;

	ll d_limit, gamma;   // EDD time constraint limit - d_limit and ratio of cost of C2E and 1-hop E2E transmission - gamma. 
	cin>>d_limit>>gamma;

	vector<vector<ll>> G(V+1, vector<ll>());      // Graph G(V, E).
	map<ll, ll> destination_edge;
	weights.clear();
	edges.clear();

	for(ll i=0;i<E;i++){        // Create the graph with given edges.
		ll node1, node2, w;
		cin>>node1>>node2>>w;
		G[node1].push_back(node2);
		G[node2].push_back(node1);
		weights[{node1, node2}] = w;
		weights[{node2, node1}] = w;
	}

	/*
		for(ll i=1;i<=V;i++){      // Add edges from cloud 'c' to each edge server 'v'.
			G[0].push_back(i);
			G[i].push_back(0);
		}
	*/

	for(ll i=0;i<=V;i++){    // Set the weights of edges between cloud and edge servers to some comstant default value.
		weights[{0, i}] = CETC;
		weights[{i, 0}] = CETC;
		weights[{0, 0}] = 0;
	}

	for(ll i=0;i<R;i++){    // Create an array of destination edge servers.
		ll node;
		cin>>node;
		destination_edge[node] = 1;
	}

	ll edge_server, count;
	vector<bool> visited(V+1, false);
	ll C2E = 0;
	ll E2E = 0;
	ll costC2E = 0;
	ll costE2E = 0;
	srand(time(0));

	do{
		ll count = 0;
		for(ll i=1;i<=V;i++){
			if(destination_edge.find(i) != destination_edge.end()){
				if(destination_edge[i] == 1){
					count++;
				}
			}
		}
		if(count == 0){
			break;
		}
		edge_server = (rand()%V) + 1;
		while(visited[edge_server]){
			edge_server = (rand()%V) + 1;
		}
		if(!visited[edge_server]){
			update_connection(G, visited, destination_edge, edge_server, d_limit); 	
			C2E += 1;
		}
	}while(count!=0);

	costC2E = (C2E * CETC);
	E2E = edges.size();
	for(auto i : edges){
		ll src = i.first.first;
		ll dest = i.first.second;
		// cout<<src<<" "<<dest<<endl;
		costE2E += weights[{src, dest}];
	}

	cout<<"Total Number Of C2E Edges: "<<C2E<<endl;
	cout<<"Cost C2E: "<<costC2E<<endl;
	cout<<"Total Number Of E2E Edges: "<<E2E<<endl;
	cout<<"Cost E2E: "<<costE2E<<endl;
	cout<<"Total Cost By RANDOM Algorithm: "<<(costC2E + costE2E)<<endl;

	return 0;
}
