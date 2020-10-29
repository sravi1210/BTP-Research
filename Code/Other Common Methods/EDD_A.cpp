#include <bits/stdc++.h>
using namespace std;

#define S second
#define F first
#define ll long long int

void printTree(vector<vector<ll>> dp){
	ll size = dp.size();
	for(ll i=0;i<size;i++){
		ll temp = dp[i].size();
		if(temp > 0){
			cout<<i<<": ";
			for(ll j=0;j<temp;j++){
				cout<<dp[i][j]<<" ";
			}
			cout<<endl;
		}
	}
	return;
}

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

void BFS(vector<vector<ll>> &cmst, vector<ll> &depths, ll node, ll height){
	deque<pair<ll, ll>> dq;
	dq.push_back({node, height});
	depths[node] = height;
	while(!dq.empty()){
		pair<ll, ll> p = dq.front();
		dq.pop_front();
		ll index = p.F;
		ll height = p.S;
		ll size = cmst[index].size();
		for(ll i=0;i<size;i++){
			ll child = cmst[index][i];
			if(depths[child] > height + 1){
				depths[child] = height + 1;
				dq.push_back({child, height+1});
			}
		}
	}
	return;
}

void DFS(vector<vector<ll>> &cmst, vector<vector<ll>> &eddA, vector<ll> &depths, map<ll, ll> &destination_edge, vector<bool> &visited, ll index, ll d_limit){
	ll size = cmst[index].size();
	for(ll i=0;i<size;i++){
		ll child = cmst[index][i];
		if(!visited[child] && (depths[child] == depths[index]+1)){
			if(depths[child] <= d_limit && destination_edge.find(child) != destination_edge.end()){
				eddA[index].push_back(child);
				visited[child] = true;
				DFS(cmst, eddA, depths, destination_edge, visited, child, d_limit);
			}
			else if(destination_edge.find(child) == destination_edge.end()){
				eddA[index].push_back(child);
				visited[child] = true;
				DFS(cmst, eddA, depths, destination_edge, visited, child, d_limit);
			}
			else if(depths[child] > d_limit && destination_edge.find(child) != destination_edge.end()){
				depths[child] = 1;
				eddA[0].push_back(child);
				visited[child] = true;
				BFS(cmst, depths, child, 1);
				DFS(cmst, eddA, depths, destination_edge, visited, child, d_limit);
			} 
		}
	}
	return;
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

vector<vector<ll>> EDD_A(vector<vector<ll>> &cmst, map<ll, ll> &destination_edge, ll V, ll d_limit){
	vector<vector<ll>> eddA(V+1);
	vector<ll> depths(V+1, INT_MAX);
	BFS(cmst, depths, 0, 0);
	vector<bool> visited(V+1, false);
	visited[0] = true;
	DFS(cmst, eddA, depths, destination_edge, visited, 0, d_limit);
	return eddA;
}

int main(){
	ll V, E, R;          // V - Vertex, E - Edges,  R - Destination Edge Servers.
	cin>>V>>E>>R;

	ll d_limit, gamma;   // EDD time constraint limit - d_limit and ratio of cost of C2E and 1-hop E2E transmission - gamma. 
	cin>>d_limit>>gamma;

	d_limit += 1;

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
	printTree(cmst);

	vector<vector<ll>> eddA = EDD_A(cmst, destination_edge, V, d_limit);
	cout<<endl<<"Final EDD Approximated Tree is:"<<endl;
	printTree(eddA);

	ll C2E = 0;
	ll E2E = 0;
	for(ll i=0;i<=V;i++){
		if(i==0){
			C2E += eddA[i].size();
		}
		else{
			E2E += eddA[i].size();
		}
	}

	cout<<endl<<"Cost C2E: "<<C2E<<" times of 1-Hop E2E"<<endl;
	cout<<"Cost E2E: "<<E2E<<" times of 1-Hop E2E"<<endl;
	cout<<"Total Cost By EDD-A Algorithm: "<<((gamma*C2E)+E2E)<<" times of 1-Hop E2E"<<endl; 

	return 0;
}