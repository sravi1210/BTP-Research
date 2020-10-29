# Different Approaches in Comparison:
## 1. Greedy Connectivity (GC) 
	In this approach, we define the connectivity of an edge server as the number of edge servers in R (set of the destination
	edge servers i.e. servers which can be directly connected to the cloud) that have yet to receive the data. This approach 
	always selects the edge servers with the highest connectivity to receive data from the cloud, which will then transmit
	the data to other destination edge servers in R, until all the	destination edge servers in R can receive the data within
	the EDD (Edge Data Distribution) time constraint limit of dlimit.

## 2. Random 
	This approach randomly selects the edge servers to receive the data from
	the cloud, which then transmit the data to other destination edge servers
	in R, one after another, until all the destination edge servers in R
	receive the data within the EDD (Edge Data Distribution) time constraint
	limit of dlimit.

## 3. EDD-A Algorithm
	In this approach, we first calculate a Connectivity-oriented Minimum
	Steiner Tree (CMST) using an O(2) approximation method. This method
	calculates a minimum Steiner Tree having cost at most twice of the optimal
	Steiner Tree on the graph G(V, E). After that we use a heuristic algorithm
	to calculate the minimum cost of transmission incurred using edge-to-edge
	and cloud-to-edge servers on the approximated CMST. This algorithm is
	proved to have an O(k) approximation solution.

# To Compile Program Use:
	$ g++ fileName.cpp

# To Run Program Use:
	$ ./a.out < ../TestCases/inputFileName.txt 

