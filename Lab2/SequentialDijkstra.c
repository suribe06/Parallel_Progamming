#include<stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define V 10000 // Number of vertices in the graph
static int graph[V][V];
//static int graph[V][V] = {{0,2,4,0,0,0},{0,0,1,7,0,0},{0,0,0,0,3,0},{0,0,0,0,0,1},{0,0,0,2,0,5},{0,0,0,0,0,0}};
int dist[V];
bool visited[V];

void random_erdos(double p){
	double random;
	for (int i = 0; i < V; i++) {
		for (int j = i+1; j < V; j++) {
			random = (rand() + 1.0) / (RAND_MAX+2.0);
			if (random < p) {
				int w = (rand() % (50 - 1 + 1)) + 1;
				graph[i][j] = w;
			}
		}
	}
}

int minDistance(){
	int min = INT_MAX, min_index;
	for (int v = 0; v < V; v++)
		if (visited[v] == false && dist[v] < min)
			min = dist[v], min_index = v;

	return min_index;
}

void printSolution(int src){
	for (int i = 0; i < V; i++)
		if(i != src)
			printf("La ditancia del nodo source al nodo %d es %d \n", i, dist[i]);
}

void dijkstra(int src){
	// Initialize all distances as INFINITE and visited[] as false
	for (int i = 0; i < V; i++){
		dist[i] = INT_MAX;
		visited[i] = false;
	}

	dist[src] = 0;
	// Find shortest path for all vertices
	for (int count = 0; count < V; count++) {
		// Pick the minimum distance vertex from the set of vertices not processed yet.
		int u = minDistance();
		visited[u] = true;

		// Update dist value of the adjacent vertices of the picked vertex.
		for (int v = 0; v < V; v++)
			if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
				dist[v] = dist[u] + graph[u][v];
	}
}

int main(int argc, char* argv[]){
	//construct graph with the model Erdos-Renyi
	double prob = 0.5; //Probability that the edge (u,v) exists
	random_erdos(prob);
	dijkstra(0);
	//printSolution(src);
	return 0;
}
