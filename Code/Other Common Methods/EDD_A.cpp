#include <bits/stdc++.h>
using namespace std;

#define S second
#define F first
#define ll long long int

map<pair<ll, ll>, pair<ll, ll>> closestNodes(vector<vector<ll>> &G, vector<bool> &visited){
	map<pair<ll, ll>, pair<ll,ll>> mp;
	ll size = visited.size();

	for(ll i=0;i<size;i++){
		if(visited[i]){
			ll temp = G[i].size();
			for(ll j=0;j<temp;j++){
				int child = G[i][j];
				if(!visited[child]){
					ll cost = (i==0)?20:1;
					mp[{i, child}] = {cost, G[child].size()};
				}
			}
		}
	}

	return mp;
}

vector<vector<ll>> CMST(vector<vector<ll>> &G, map<ll, ll> &destination_edge, ll V){
	vector<vector<ll>> cmst(V+1, vector<ll>());
	vector<bool> visited(V+1, false);
	visited[0] = true;

	while(true){
		ll count = 0;
		for(auto v : destination_edge){
			if(!visited[v.F]){
				count++;
			} 
		}
		if(count == 0){
			break;
		}
		
		map<pair<ll, ll>, pair<ll, ll>> mp = closestNodes(G, visited);

		ll maxConnectivity = LONG_MIN;
		ll minCost = LONG_MAX;
		ll parent = -1, child = -1;
		for (auto p : mp){
			minCost = min(minCost, p.S.F);
		}
		for(auto p : mp){
			if((minCost == p.S.F) && (p.S.S > maxConnectivity)){
				maxConnectivity = p.S.S;
				parent = p.F.F;
				child = p.F.S;
			}
		}
		if(parent != -1 && child != -1){
			cmst[parent].push_back(child);
			visited[child] = true;
		}
	}

	return cmst;
}

void EDD_A(){
	return;
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

	for(ll i=1;i<=V;i++){      // Add edges from cloud 'c' to each edge server 'v'.
		G[0].push_back(i);
	}
	
	for(ll i=0;i<R;i++){    // Create a map of destination edge servers.
		ll node;
		cin>>node;
		destination_edge[node] = 1;
	}

	vector<vector<ll>> cmst = CMST(G, destination_edge, V);
	cout<<"Minimum Steiner Tree Edge List:"<<endl; 
	for(ll i=0;i<=V;i++){
		ll temp = cmst[i].size();
		if(temp > 0){
			cout<<i<<": ";
			for(ll j=0;j<temp;j++){
				cout<<cmst[i][j]<<" ";
			}
			cout<<endl;
		}
	}
	
	return 0;
}