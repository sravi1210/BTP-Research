/**
 *    author:  sravi1210
 *    created: 07.11.2020 04:08:33       
**/

#include <bits/stdc++.h>
using namespace std;

#define F first
#define S second
#define ll long long int

// Global variables for Graph, EDDA, CMST, destination edge servers, CMST nodes and edges.
map<ll, ll> Wnodes;
vector<vector<ll>> G;
vector<vector<ll>> TG;
map<ll, ll> CSTNodes;
vector<vector<ll>> CST;
vector<vector<ll>> EDD;
vector<vector<ll>> Gdash;
map<ll, map<ll, ll>> ZST;
vector<vector<ll>> triples;
map<ll, map<ll, ll>> GRdash;
vector<pair<ll,ll>> MSTedges;
map<ll, ll> destination_edge;
map<pair<ll, ll>, ll> CSTEdges;
vector<vector<ll>> fineTunedEDD;
map<pair<ll, ll>, ll> seenEdges;
vector<pair<ll, ll>> centroidTriples;

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

void printMachine(map<ll, map<ll, ll>> dp){
	for(auto i : dp){
		ll a = i.first;
		if(dp[a].size() > 0){
			cout<<a<<": ";
		}
		for(auto j : dp[a]){
			ll b = j.first;
			cout<<"("<<b<<" "<<dp[a][b]<<") ";
		}
		cout<<endl;
	}
	return;
}

// Function to initialize global variables.
void initialize(ll V){
	G.clear();
	TG.clear();
	ZST.clear();
	CST.clear();
	EDD.clear();
	Gdash.clear();
	Wnodes.clear();
	GRdash.clear();
	triples.clear();
	MSTedges.clear();
	CSTEdges.clear();
	CSTNodes.clear();
	seenEdges.clear();
	fineTunedEDD.clear();
	centroidTriples.clear();
	destination_edge.clear();

	vector<ll> xtemp;
	for(ll i=0;i<=V;i++){
		vector<ll> temp(V+1, -1);
		G.push_back(temp);
		TG.push_back(xtemp);
		CST.push_back(xtemp);
		EDD.push_back(xtemp);
		Gdash.push_back(temp);
		fineTunedEDD.push_back(xtemp);
	}
	return;
}

void readInputGraph(ll V, ll E, ll R, ll gamma){
	for(ll i=0;i<E;i++){        // Create the graph with given edges.
		ll node1, node2;
		cin>>node1>>node2;
		G[node1][node2] = 1;
		G[node2][node1] = 1;
		TG[node1].push_back(node2);
		TG[node2].push_back(node1);
	}

	for(ll i=0;i<R;i++){    // Create a map of destination edge servers.
		ll node;
		cin>>node;
		destination_edge[node] = 1;
	}

	return;
}

void createGpDash(ll V){
	for(ll i=0;i<=V;i++){
		vector<ll> distance(V+1, LLONG_MAX);
		vector<bool> visited(V+1, false);
		distance[i] = 0;
		visited[i] = true;
		deque<ll> dq;
		dq.push_back(i);
		while(!dq.empty()){
			ll parent = dq.front();
			dq.pop_front();
			ll size = TG[parent].size();
			for(ll j=0;j<size;j++){
				ll child = TG[parent][j];
				if(!visited[child]){
					dq.push_back(child);
					visited[child] = true;
					distance[child] = distance[parent] + 1;
				}
			}
		}
		for(ll j=0;j<=V;j++){
			Gdash[i][j] = distance[j];
		}
	}
	return;
}

void createGRdash(ll V){
	vector<ll> Rnodes;
	for(ll i=0;i<=V;i++){
		if(destination_edge.find(i) != destination_edge.end()){
			Rnodes.push_back(i);
		}	
	}
	ll size = Rnodes.size();
	for(ll i=0;i<size;i++){
		ll x = Rnodes[i];
		for(ll j=i;j<size;j++){
			ll y = Rnodes[j];
			GRdash[x][y] = Gdash[x][y];
			GRdash[y][x] = Gdash[y][x];
		}
	}
	return;
}

void createTriples(ll V){
	vector<ll> Rnodes;
	for(ll i=0;i<=V;i++){
		if(destination_edge.find(i) != destination_edge.end()){
			Rnodes.push_back(i);
		}	
	}
	ll size = Rnodes.size();
	for(ll i=0;i<size;i++){
		ll x = Rnodes[i];
		for(ll j=i+1;j<size;j++){
			ll y = Rnodes[j];
			for(ll k=j+1;k<size;k++){
				ll z = Rnodes[k];
				if(x!=y && y!=z && x!=z){
					vector<ll> temp;
					temp.push_back(x);
					temp.push_back(y);
					temp.push_back(z);
					triples.push_back(temp);
				}
			}
		}
	}
	return;
}

map<ll, map<ll, ll>> createCopy(map<ll, map<ll, ll>> dp){
	return dp;
}

void solveTriples(ll V){
	ll size = triples.size();
	for(ll i=0;i<size;i++){
		ll index = -1;
		ll dist = LLONG_MAX;
		for(ll j=0;j<=V;j++){
			if(destination_edge.find(j) == destination_edge.end()){
				ll node1 = triples[i][0];
				ll node2 = triples[i][1];
				ll node3 = triples[i][2];
				ll d = Gdash[j][node1] + Gdash[j][node2] + Gdash[j][node3];
				if(d < dist){
					dist = d;
					index = j;
				}
			}
		}
		if(index!=-1){
			centroidTriples.push_back({index, dist});
		}
	}
	return;
}

ll findTParent(vector<ll> parent, ll node){
	if(parent[node] != node){
		parent[node] = findTParent(parent, parent[node]);
	}
	return parent[node];
}

ll findParent(vector<ll> &parent, ll node){
	if(parent[node] != node){
		parent[node] = findParent(parent, parent[node]);
	}
	return parent[node];
}

void Union(vector<ll> &parent, vector<ll> &rank, ll node1, ll node2){
	ll xroot = findParent(parent, node1);
	ll yroot = findParent(parent, node2);

	if(rank[xroot] < rank[yroot]){
		parent[xroot] = yroot;
	}
	else if(rank[xroot] > rank[yroot]){
		parent[yroot] = xroot;
	}
	else{
		parent[yroot] = xroot;
		rank[xroot]++;
	}
	return;
}

ll solveMST(map<ll, map<ll, ll>> dp, ll V){
	MSTedges.clear();
	ll size = dp.size();
	vector<ll> parent(V+1);
	vector<ll> rank(V+1, 0);
	iota(parent.begin(), parent.end(), 0);

	ll weightSum = 0;
	ll count = 0;
	ll end = 0;
	vector<pair<ll, pair<ll, ll>>> edges;
	for(auto i : dp){
		ll a = i.F;
		for(auto j : dp){
			ll b = j.F;
			edges.push_back({dp[a][b], {a, b}});
			end++;
		}
	}

	sort(edges.begin(), edges.end());

	ll start = 0;
	while(count<size-1 && end>start){
		pair<ll, pair<ll, ll>> data = edges[start];
		ll edgeLen = data.F;
		ll a = data.S.F;
		ll b = data.S.S;
		ll aroot = findTParent(parent, a);
		ll broot = findTParent(parent, b);
		if(aroot != broot){
			weightSum += edgeLen;
			Union(parent, rank, a, b);
			MSTedges.push_back({a,  b});
			count++;
		}
		start++;
	}
	return weightSum;
}

map<ll, map<ll, ll>> findSubGraph(map<ll, map<ll, ll>> T, ll node, ll V){
	vector<bool> visited(V+1, false);
	map<ll, map<ll, ll>> ret;
	deque<pair<ll, ll>> dq;
	map<ll, ll> temp;
	ret[node] = temp;
	dq.push_back({node, -1});
	visited[node] = true;

	while(!dq.empty()){
		pair<ll, ll> p = dq.front();
		dq.pop_front();
		ll child = p.first;
		ll parent = p.second;
		if(parent != -1){
			ret[parent][child] = T[parent][child];
			ret[child][parent] = T[child][parent];
		}
		for(auto i : T[child]){
			ll index = i.first;
			if(!visited[index] && T[child][index]!=-1){
				dq.push_back({index, child});
				visited[index] = true;
			}
		}
	}
	return ret;
}

void findSave(map<ll, map<ll, ll>> &save, map<ll, map<ll, ll>> T, ll V){
	ll maxEdge = LLONG_MIN;
	ll node1 = -1, node2 = -1;

	for(auto i : T){
		ll a = i.first;
		for(auto j : T[a]){
			ll b = j.first;
			if(T[a][b] > maxEdge){
				maxEdge = T[a][b];
				node1 = a;
				node2 = b;
			}
		}
	}

	if(node1 != -1 && node2 != -1 && maxEdge != LLONG_MIN){
		map<ll, map<ll, ll>> T1;
		map<ll, map<ll, ll>> T2;
		T[node1][node2] = -1;
		T[node2][node1] = -1;
		T1 = findSubGraph(T, node1, V);
		T2 = findSubGraph(T, node2, V);
		for(auto i : T1){
			ll a = i.first;
			for(auto j : T2){
				ll b = j.first;
				save[a][b] = maxEdge;
				save[b][a] = maxEdge;
			}
		}
		findSave(save, T1, V);
		findSave(save, T2, V);
	}

	return;
}

map<ll, map<ll, ll>> createMST(){
	map<ll, map<ll, ll>> T;
	ll size = MSTedges.size();
	for(ll i=0;i<size;i++){
		ll x = MSTedges[i].F;
		ll y = MSTedges[i].S;
		T[x][y] = GRdash[x][y];
		T[y][x] = GRdash[y][x];
	}
	return T;
}

void reduceWeight(map<ll, map<ll, ll>> &dp, ll index){
	ll node1 = triples[index][0];
	ll node2 = triples[index][1];
	ll node3 = triples[index][2];
	dp[node1][node2] = 0;
	dp[node2][node1] = 0;
	dp[node2][node3] = 0;
	dp[node3][node2] = 0;
	return;
}

void fillW(ll V){
	map<ll, map<ll, ll>> F;
	F = createCopy(GRdash);
	ll size = triples.size();
	vector<bool> visited(size, false);

	while(true){
		solveMST(F, V);
		map<ll, map<ll, ll>> T;
		T = createMST();
		map<ll, map<ll, ll>> save;
		findSave(save, T, V);
		ll win = LLONG_MIN;
		ll VZ = -1;
		ll index = -1;
		for(ll i=0;i<size;i++){
			if(!visited[i]){
				ll DZ = centroidTriples[i].S;
				ll node1 = triples[i][0];
				ll node2 = triples[i][1];
				ll node3 = triples[i][2];
				ll maxWin = max(save[node1][node2], max(save[node2][node3], save[node1][node3]));
				ll minWin = min(save[node1][node2], min(save[node2][node3], save[node1][node3]));
				if(win < (maxWin + minWin - DZ)){
					win = maxWin + minWin - DZ;
					VZ = centroidTriples[i].F;
					index = i;
				}
			}
		}
		if(win <= 0 || VZ == -1 || index == -1){
			return;
		}
		// cout<<win<<" ";
		visited[index] = true;
		reduceWeight(F, index);
		Wnodes[VZ] = 1;
	}
	return;
}

void findST(ll V){
	vector<ll> nodes;
	for(ll i=0;i<=V;i++){
		if((destination_edge.find(i) != destination_edge.end()) || (Wnodes.find(i) != Wnodes.end())){
			nodes.push_back(i);
		}
	}
	ll size = nodes.size();
	map<ll, map<ll, ll>> ZSTtemp;
	for(ll i=0;i<size;i++){
		ll x = nodes[i];
		for(ll j=0;j<size;j++){
			ll y = nodes[j];
			ZSTtemp[x][y] = Gdash[x][y];
		}
	}

	solveMST(ZSTtemp, V);
	size = MSTedges.size();
	for(ll i=0;i<size;i++){
		ll x = MSTedges[i].F;
		ll y = MSTedges[i].S;
		ZST[x][y] = Gdash[x][y];
		ZST[y][x] = Gdash[y][x];
	}
	return;
}

bool DFS_CloudST(deque<ll> &sk, ll node, ll distance, ll curr, ll parent){
	ll size = G.size();
	if(distance==0 && curr==node){
		return true;
	}
	else if(distance<=0){
		return false;
	}
	for(ll i=0;i<size;i++){
		if(i!= parent && G[curr][i] != -1){
			sk.push_back(i);
			if(DFS_CloudST(sk, node, distance-1, i, curr)){
				return true;
			}
			sk.pop_back();
		}
	}
	return false;
}

void BFS_CloudST(vector<bool> &visited, ll index){
	deque<ll> dq;
	dq.push_back(index);
	visited[index] = true;
	while(!dq.empty()){
		ll curr = dq.front();
		dq.pop_front();

		for(auto i : ZST[curr]){
			ll node = i.F;
			if(!visited[node]){
				if(ZST[curr][node] == 1){
					CST[curr].push_back(node);
					CSTEdges[{curr, node}] = 1;
					CSTNodes[curr] = 1;
					CSTNodes[node] = 1;
				}
				else{
					ll distance = ZST[curr][node];
					deque<ll> sk;
					sk.push_back(curr);
					DFS_CloudST(sk, node, distance, curr, -1);
					ll a = sk.front();
					sk.pop_front();
					while(!sk.empty()){
						ll b = sk.front();
						sk.pop_front();
						CST[a].push_back(b);
						CSTEdges[{a, b}] = 1;
						CSTNodes[a] = 1;
						CSTNodes[b] = 1;
						a = b;
					}
				}
				visited[node] = true;
				dq.push_back(node);
			}
		}
	}
	return;
}

void addCloudST(ll V){
	ll maxConnectivity = -1;
	ll index = -1;
	for(auto i : ZST){
		ll node = i.F;
		ll size = ZST[node].size();
		if(size > maxConnectivity){
			maxConnectivity = size;
			index = node;
		}
	}
	CST[0].push_back(index); // Add cloud server to the highest connectivity node in the approximated optimal Steiner Tree.
	CSTEdges[{0, index}] = 1;
	CSTNodes[0] = 1;
	CSTNodes[index] = 1;
	vector<bool> visited(V+1, false);
	visited[0] = true;
	BFS_CloudST(visited, index);
	return;
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
			ll child = i;
			if(G[parent][i]!= -1 && !visited[child] && !nvisited[child]){
				if(depths[child] > height + 1){
					depths[child] = height + 1;
					dq.push_back({child, height+1});
					if(CSTEdges.find({parent, child}) == CSTEdges.end()){
						CSTEdges[{parent, child}] = 1;
						CSTNodes[parent] = 1;
						CSTNodes[child] = 1;
						CST[parent].push_back(child);
					}
					nvisited[child] = true;
				}
			}
		}
	}

	return;
}

// Function to do BFS traversal on the graph.
void BFS_EDD(vector<ll> &depths, ll node, ll height){
	deque<pair<ll, ll>> dq;
	dq.push_back({node, height});
	depths[node] = height;
	while(!dq.empty()){
		pair<ll, ll> p = dq.front();
		dq.pop_front();
		ll index = p.F;
		ll height = p.S;
		ll size = CST[index].size();
		for(ll i=0;i<size;i++){
			ll child = CST[index][i];
			if(depths[child] > height + 1){
				depths[child] = height + 1;
				dq.push_back({child, height+1});
			}
		}
	}
	return;
}

// Function to do DFS traversal on the graph.
void DFS_EDD(vector<ll> &depths, vector<bool> &visited, ll index, ll d_limit){
	ll size = CST[index].size();
	for(ll i=0;i<size;i++){
		ll child = CST[index][i];
		if(!visited[child] && (depths[child] == depths[index]+1)){
			if(depths[child] <= d_limit && destination_edge.find(child) != destination_edge.end()){
				EDD[index].push_back(child);
				visited[child] = true;
				DFS_EDD(depths, visited, child, d_limit);
			}
			else if(destination_edge.find(child) == destination_edge.end()){
				EDD[index].push_back(child);
				visited[child] = true;
				DFS_EDD(depths, visited, child, d_limit);
			}
			else if(depths[child] > d_limit && destination_edge.find(child) != destination_edge.end()){
				depths[child] = 1;
				EDD[0].push_back(child);
				visited[child] = true;
				updateEdges(depths, visited, child);
				BFS_EDD(depths, child, 1);
				DFS_EDD(depths, visited, child, d_limit);
			} 
		}
	}
	return;
}

// Fnction to calculate EDD approximation on the calculated steiner tree.
void calculateEDD(ll V, ll d_limit){
	vector<ll> depths(V+1, LLONG_MAX);
	BFS_EDD(depths, 0, 0);
	vector<bool> visited(V+1, false);
	visited[0] = true;
	DFS_EDD(depths, visited, 0, d_limit);
	return;
}

// Function to fine Tune the EDD Steiner Tree, for removing the non-essential edges.
void FineTune_EDD(deque<ll> &eddDQ, vector<bool> &seen){
	ll index = eddDQ.back();
	ll size = EDD[index].size();
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
		ll child = EDD[index][i];
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
	readInputGraph(V, E, R, gamma);
	createGpDash(V);
	createGRdash(V);
	createTriples(V);
	solveTriples(V);
	fillW(V);
	findST(V);
	addCloudST(V);
	
	cout<<"Minimum Steiner Tree Edge List:"<<endl;
	printTree(CST);

	calculateEDD(V, d_limit);

	cout<<endl<<"EDD Approximated Tree:"<<endl;
	printTree(EDD);

	deque<ll> eddDQ;
	eddDQ.push_back(0);
	vector<bool> seen(V+1, false);
	seen[0] = true;
	
	FineTune_EDD(eddDQ, seen);
	
	cout<<endl<<"Fine-Tuned final EDD Approximated Tree is:"<<endl;
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
	cout<<"Total Cost By EDD-NSTE Algorithm: "<<((gamma*C2E)+E2E)<<" times of 1-Hop E2E"<<endl;

	return 0;
}