#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "mpi.h"
#define V 10000 //number of nodes
static int G[V][V];// = {{0,2,4,0,0,0},{0,0,1,7,0,0},{0,0,0,0,3,0},{0,0,0,0,0,1},{0,0,0,2,0,5},{0,0,0,0,0,0}};
int distance[V];
bool visited[V];
int p;
int tag=202;
MPI_Status status;

void print_solution(){
   for (int i = 0; i < V; i++) {
      printf("%d ", distance[i]);
   }
   printf("\n");
}

void random_erdos(double pr){
	double random;
	for (int i = 0; i < V; i++) {
		for (int j = i+1; j < V; j++) {
			random = (rand() + 1.0) / (RAND_MAX+2.0);
			if (random < pr) {
				int w = (rand() % (50 - 1 + 1)) + 1;
				G[i][j] = w;
			}
		}
	}
}

void dijkstra(int source, int my_rank){
   int mindistance, u, i, k, ans;
   int pair[2], global_pair[2];
   distance[source] = 0;
   for(k=0; k<V; k++){
      mindistance = INT_MAX;
      for(i=my_rank; i<V; i+=p){
         if(!visited[i] && distance[i] < mindistance){
   	      u = i;
            mindistance = distance[i];
         }
      }
		pair[0] = mindistance;
		pair[1] = my_rank;

      MPI_Allreduce(pair, global_pair, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
		if(my_rank == global_pair[1]){
			for(int i =0; i<p; i++){
				if(i != my_rank){
					MPI_Send(&u, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
				}
			}
		}else{
			MPI_Recv(&ans, 1, MPI_INT, global_pair[1], tag, MPI_COMM_WORLD, &status);
			u = ans;
		}

      visited[u] = true;
		distance[u] = global_pair[0];
      for(i=my_rank; i<V; i+=p){
         if(!visited[i] && G[u][i] != 0 && distance[u]+G[u][i] < distance[i]){
            distance[i] = distance[u] + G[u][i];
         }
      }
   }
}

int main(int argc, char* argv[]){
   //construct graph with the model erdos rengi
	int my_rank;
	double prob = 0.5; //Probability that the edge (u,v) exists in E
	random_erdos(prob);
	/* start up MPI */
	MPI_Init(&argc, &argv);
	/* find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	//initialize distance[] and visited[]
   for(int i=my_rank; i<V; i+=p){
      visited[i] = false;
		distance[i] = INT_MAX;
   }
   int source = 0;
	dijkstra(source, my_rank);
	/* shut down MPI */
	MPI_Finalize();
	/*if(my_rank == 0){
		print_solution();
	}*/
	return 0;
}
