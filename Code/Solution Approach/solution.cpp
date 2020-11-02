#include <bits/stdc++.h>
using namespace std;

#define F first
#define S second
#define ll long long int

// Global variables for Graph, EDDA, CMST, destination edge servers, CMST nodes and edges.
map<ll, ll> Wnodes;
vector<vector<ll>> G;
vector<vector<ll>> ZST;
vector<vector<ll>> Gdash;
vector<vector<ll>> GRdash;
vector<vector<ll>> triples;
vector<pair<ll,ll>> ZSTedges;
map<ll, ll> destination_edge;
vector<pair<ll, ll>> centroidTriples;

void printMachine(vector<vector<ll>> dp){
	ll row = dp.size();
	ll col = dp[0].size();
	for(ll i=0;i<row;i++){
		for(ll j=0;j<col;j++){
			cout<<dp[i][j]<<" ";
		}
		cout<<endl;
	}
	return;
}

// Function to initialize global variables.
void initialize(ll V){
	G.clear();
	ZST.clear();
	Gdash.clear();
	Wnodes.clear();
	GRdash.clear();
	triples.clear();
	ZSTedges.clear();
	centroidTriples.clear();
	destination_edge.clear();

	for(ll i=0;i<=V;i++){
		vector<ll> temp(V+1, -1);
		G.push_back(temp);
		ZST.push_back(temp);
		Gdash.push_back(temp);
		GRdash.push_back(temp);
	}
	return;
}

void readInputGraph(ll V, ll E, ll R, ll gamma){
	for(ll i=0;i<E;i++){        // Create the graph with given edges.
		ll node1, node2;
		cin>>node1>>node2;
		G[node1][node2] = 1;
		G[node2][node1] = 1;
	}

	for(ll i=1;i<=V;i++){      // Add edges from cloud 'c' to each edge server 'v'.
		G[0][i] = gamma;
		G[i][0] = gamma;
	}
	
	for(ll i=0;i<R;i++){    // Create a map of destination edge servers.
		ll node;
		cin>>node;
		destination_edge[node] = 1;
	}
	destination_edge[0] = 1;
	return;
}


ll minDistance(vector<ll> &distance, vector<bool> &visited, ll V){
	ll index = -1;
	ll value = LLONG_MAX;
	for(ll i=0;i<=V;i++){
		if(distance[i] <= value && !visited[i]){
			value = distance[i];
			index = i;
		}
	}
	return index;
}

void createGpDash(ll V){
	for(ll i=0;i<=V;i++){
		vector<ll> distance(V+1, LLONG_MAX);
		vector<bool> visited(V+1, false);
		distance[i] = 0;

		for(ll count = 0;count<=V;count++){
			ll index = minDistance(distance, visited, V);
			if(index == -1){
				continue;
			}
			visited[index] = true;
			for(ll j=0;j<=V;j++){
				if((!visited[j]) && (G[index][j]!=-1) && (distance[j] > distance[index] + G[index][j]) && (distance[index] != LLONG_MAX)){
					distance[j] = distance[index] + G[index][j];
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

vector<vector<ll>> createCopy(vector<vector<ll>> dp){
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

ll solveMST(vector<vector<ll>> dp, bool check){
	ll row = dp.size();
	ll col = dp[0].size();
	vector<bool> visited(row, false);
	ll weightSum = 0;
	ll count = 0;
	while(count<row){
		ll minEdge = LLONG_MAX;
		ll x = -1;
		ll y = -1;
		for(ll i=0;i<row;i++){
			for(ll j=0;j<col;j++){
				if(i!=j && dp[i][j]!=-1){
					if(!(visited[i] && visited[j])){
						if(dp[i][j] < minEdge){
							minEdge = dp[i][j];
							x = i;
							y = j;
						}
					}
				}
			}
		}
		if(minEdge!= LLONG_MAX && x!= -1 && y!= -1){
			weightSum += minEdge;
			visited[x] = true;
			visited[y] = true;
			if(check){
				ZSTedges.push_back({x, y});
			}
		}
		count++;
	}
	return weightSum;
}

void reduceWeight(vector<vector<ll>> &dp, ll index){
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
	vector<vector<ll>> F(V+1, vector<ll>(V+1, -1));
	F = createCopy(Gdash);
	ll size = triples.size();
	vector<bool> visited(size, false);

	while(true){
		ll mstF = solveMST(F, false);
		ll win = LLONG_MIN;
		ll VZ = -1;
		ll index = -1;
		for(ll i=0;i<size;i++){
			if(!visited[i]){
				ll DZ = centroidTriples[i].S;
				vector<vector<ll>> FZ(V+1, vector<ll>(V+1, -1));
				FZ = createCopy(F);
				reduceWeight(FZ, i);
				ll mstFZ = solveMST(FZ, false);
				if(win < (mstF - mstFZ - DZ)){
					win = mstF - mstFZ - DZ;
					VZ = centroidTriples[i].F;
					index = i;
				}
			}
		}
		if(win <= 0){
			return;
		}
		visited[index] = true;
		vector<vector<ll>> FZ(V+1, vector<ll>(V+1, -1));
		FZ = createCopy(F);
		reduceWeight(FZ, index);
		F = createCopy(FZ);
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
	vector<vector<ll>> ZSTtemp(V+1, vector<ll>(V+1, -1));
	for(ll i=0;i<size;i++){
		ll x = nodes[i];
		for(ll j=0;j<size;j++){
			ll y = nodes[j];
			ZSTtemp[x][y] = Gdash[x][y];
		}
	}

	solveMST(ZSTtemp, true);
	size = ZSTedges.size();
	for(ll i=0;i<size;i++){
		ll x = ZSTedges[i].F;
		ll y = ZSTedges[i].S;
		cout<<x<<" "<<y<<endl;
		ZST[x][y] = Gdash[x][y];
		ZST[y][x] = Gdash[y][x];
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
	// printMachine(ZST);

	return 0;
}