/**
 *    author:  sravi1210
 *    created: 20.10.2020 02:46:06       
**/

#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

ll update_connection(vector<vector<ll>> &G, vector<bool> &visited, map<ll, ll> &destination_edge, ll server, ll d_limit){
	map<pair<ll, ll>, ll> edges;

	deque<pair<ll, ll>> dq;
	dq.push_back({server, 0});

	while(!dq.empty()){
		pair<ll, ll> p = dq.front();
		dq.pop_front();
		ll ind = p.first;
		ll height = p.second;
		visited[ind] = true;
		if(destination_edge.find(ind) != destination_edge.end() && destination_edge[ind] == 1){
			destination_edge[ind] = 2;
		}

		if(height < d_limit){
			ll connected = G[ind].size();
			for(ll j=0;j<connected;j++){
				ll child = G[ind][j];
				dq.push_back({child, height+1});
				edges[{min(child, ind), max(child, ind)}] = 1;
			}
		}
	}
	return edges.size();
}

ll max_connectivity(vector<vector<ll>> &G, vector<bool> &visited, map<ll, ll> &destination_edge, ll d_limit){
	ll size = G.size();
	ll reach = 0;
	ll index = 0;
	for(ll i=1;i<size;i++){
		if(!visited[i]){
			set<ll> vault;
			deque<pair<ll, ll>> dq;
			dq.push_back({i, 0});
			while(!dq.empty()){
				pair<ll, ll> p = dq.front();
				dq.pop_front();
				ll ind = p.first;
				ll height = p.second;

				if(destination_edge.find(ind) != destination_edge.end() && destination_edge[ind] == 1){
					vault.insert(ind);
				}	

				if(height < d_limit){
					ll connected = G[ind].size();
					for(ll j=0;j<connected;j++){
						ll child = G[ind][j];
						dq.push_back({child, height+1});
					}
				}
			}
			if(vault.size() > reach){
				reach = vault.size();
				index = i;
			}
		}
	}

	return index;
}

int main(){
	ll V, E, R;          // V - Vertex, E - Edges,  R - Destination Edge Servers.
	cin>>V>>E>>R;

	ll d_limit, gamma;   // EDD time constraint limit - d_limit and ratio of cost of C2E and 1-hop E2E transmission - gamma. 
	cin>>d_limit>>gamma;

	vector<vector<ll>> G(V+1, vector<ll>());      // Graph G(V, E).
	map<ll, ll> destination_edge;

	for(ll i=0;i<E;i++){        // Create the graph with given edges.
		ll node1, node2;
		cin>>node1>>node2;
		G[node1].push_back(node2);
		G[node2].push_back(node1);
	}

	/*
		for(ll i=1;i<=V;i++){      // Add edges from cloud 'c' to each edge server 'v'.
			G[0].push_back(i);
			G[i].push_back(0);
		}
	*/

	for(ll i=0;i<R;i++){    // Create a map of destination edge servers.
		ll node;
		cin>>node;
		destination_edge[node] = 1;
	}

	ll edge_server, count;
	vector<bool> visited(V+1, false);
	ll C2E = 0;
	ll E2E = 0;


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
		edge_server = max_connectivity(G, visited, destination_edge, d_limit);
		ll edges = update_connection(G, visited, destination_edge, edge_server, d_limit); 
		E2E += edges;
		C2E += 1;
	}while(count!=0);

	cout<<"Cost C2E: "<<C2E<<" times of 1-Hop E2E"<<endl;
	cout<<"Cost E2E: "<<E2E<<" times of 1-Hop E2E"<<endl;
	cout<<"Total Cost By Greedy Algorithm: "<<((gamma*C2E)+E2E)<<" times of 1-Hop E2E"<<endl; 

	return 0;
}