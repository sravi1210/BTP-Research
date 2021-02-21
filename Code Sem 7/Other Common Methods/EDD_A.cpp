/**
 *    author:  sravi1210
 *    created: 30.10.2020 01:25:15       
**/

#include <bits/stdc++.h>
using namespace std;

#define S second
#define F first
#define ll long long int

// Global variables for Graph, EDDA, CMST, destination edge servers, CMST nodes and edges.
vector<vector<ll>> G;
map<ll, ll> cmstNodes;
vector<vector<ll>> cmst;
vector<vector<ll>> eddA;
map<ll, ll> destination_edge;
map<pair<ll, ll>, ll> cmstEdges;
vector<vector<ll>> fineTunedEDD;
map<pair<ll, ll>, ll> seenEdges;

// Function to initialize global variables.
void initialize(ll V){
	G.clear();
	cmst.clear();
	eddA.clear();
	destination_edge.clear();
	cmstNodes.clear();
	cmstEdges.clear();
	fineTunedEDD.clear();
	seenEdges.clear();

	for(ll i=0;i<=V;i++){
		vector<ll> temp;
		G.push_back(temp);
		eddA.push_back(temp);
		cmst.push_back(temp);
		fineTunedEDD.push_back(temp);
	}
	return;
}

// Function to print the tree.
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

// Function to return closest nodes to the current set of visited nodes.
map<pair<ll, ll>, pair<ll, ll>> closestNodes(vector<bool> &visited){
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

// Function to update edges following triangle inequality.
void updateEdges(vector<ll> &depths, vector<bool> &visited, ll index){
	ll size = visited.size();
	vector<bool> nvisited(size, false);
	nvisited[index] = true;

	deque<pair<ll, ll>> dq;
	dq.push_back({index, 1});
	while(!dq.empty()){
		pair<ll, ll> p = dq.front();
		dq.pop_front();
		ll parent = p.first;
		ll height = p.second;
		size = G[parent].size();
		for(ll i=0;i<size;i++){
			ll child = G[parent][i];
			if(!visited[child] && !nvisited[child]){
				if(depths[child] > height + 1){
					depths[child] = height + 1;
					dq.push_back({child, height+1});
					if(cmstEdges.find({parent, child}) == cmstEdges.end()){
						cmstEdges[{parent, child}] = 1;
						cmstNodes[parent] = 1;
						cmstNodes[child] = 1;
						cmst[parent].push_back(child);
					}
					nvisited[child] = true;
				}
			}
		}
	}

	return;
}

// Function to do BFS traversal on the graph.
void BFS(vector<ll> &depths, ll node, ll height){
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

// Function to do DFS traversal on the graph.
void DFS(vector<ll> &depths, vector<bool> &visited, ll index, ll d_limit){
	ll size = cmst[index].size();
	for(ll i=0;i<size;i++){
		ll child = cmst[index][i];
		if(!visited[child] && (depths[child] == depths[index]+1)){
			if(depths[child] <= d_limit && destination_edge.find(child) != destination_edge.end()){
				eddA[index].push_back(child);
				visited[child] = true;
				DFS(depths, visited, child, d_limit);
			}
			else if(destination_edge.find(child) == destination_edge.end()){
				eddA[index].push_back(child);
				visited[child] = true;
				DFS(depths, visited, child, d_limit);
			}
			else if(depths[child] > d_limit && destination_edge.find(child) != destination_edge.end()){
				depths[child] = 1;
				eddA[0].push_back(child);
				visited[child] = true;
				updateEdges(depths, visited, child);
				BFS(depths, child, 1);
				DFS(depths, visited, child, d_limit);
			} 
		}
	}
	return;
}

// Function to calculate the connectivity oriented minimum Steiner Tree on the graph.
void CMST(ll V){
	vector<bool> visited(V+1, false);
	visited[0] = true;
	cmstNodes[0] = 1;

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
		
		map<pair<ll, ll>, pair<ll, ll>> mp = closestNodes(visited);

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
			cmstNodes[child] = 1;
			cmstNodes[parent] = 1;
			cmstEdges[{parent, child}] = 1;
			visited[child] = true;
		}
	}

	return;
}

// Fnction to calculate EDD approximation on the calculated steiner tree.
void EDD_A(ll V, ll d_limit){
	vector<ll> depths(V+1, INT_MAX);
	BFS(depths, 0, 0);
	vector<bool> visited(V+1, false);
	visited[0] = true;
	DFS(depths, visited, 0, d_limit);
	return;
}

void FineTune_EDD(deque<ll> &eddDQ, vector<bool> &seen){
	ll index = eddDQ.back();
	ll size = eddA[index].size();
	if(size == 0){
		ll end = eddDQ.size()-1;
		while(end>0){
			if(destination_edge.find(eddDQ[end]) == destination_edge.end()){
				end--;
			}
			else{
				break;
			}
		}
		for(ll i=0;i+1<=end;i++){
			ll node1 = eddDQ[i];
			ll node2 = eddDQ[i+1];
			if(seenEdges.find({node1, node2}) == seenEdges.end()){
				seenEdges[{node1, node2}] = 1;
				fineTunedEDD[node1].push_back(node2);
			}
		}
	}
	for(ll i=0;i<size;i++){
		ll child = eddA[index][i];
		if(!seen[child]){
			eddDQ.push_back(child);
			FineTune_EDD(eddDQ, seen);
			eddDQ.pop_back();
		}
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

	CMST(V);
	cout<<"Minimum Steiner Tree Edge List:"<<endl; 
	printTree(cmst);

	EDD_A(V, d_limit);

	deque<ll> eddDQ;
	eddDQ.push_back(0);
	vector<bool> seen(V+1, false);
	seen[0] = true;
	
	FineTune_EDD(eddDQ, seen);

	cout<<endl<<"Final EDD Approximated Tree is:"<<endl;
	printTree(fineTunedEDD);

	ll C2E = 0;
	ll E2E = 0;
	for(ll i=0;i<=V;i++){
		if(i==0){
			C2E += fineTunedEDD[i].size();
		}
		else{
			E2E += fineTunedEDD[i].size();
		}
	}


	cout<<endl<<"Cost C2E: "<<C2E<<" times of 1-Hop E2E"<<endl;
	cout<<"Cost E2E: "<<E2E<<" times of 1-Hop E2E"<<endl;
	cout<<"Total Cost By EDD-A Algorithm: "<<((gamma*C2E)+E2E)<<" times of 1-Hop E2E"<<endl; 

	return 0;
}