#ifndef MY_UTILITIES
#define MY_UTILITIES
#include "TASK1B.c"

#define X 49
#define A 65

struct Graph
{
	_Bool graph[X][X];
};

struct Graph init_graph();
struct Graph createGraph();
struct Graph removeObstacle(struct Graph graph);
void insertEdge(struct Graph graph, _Bool edge);

//defining the functions

struct Graph init_graph() {
	struct Graph graph;
	for(int i=0 ; i<X ; i++) {
		for(int j=0 ; j<X ; j++) {
			// initialzing the graph with zeroes
			graph.graph[i][j] = 0;
		}
	}
}

void insertEdge(struct Graph &graph, _Bool edge, int i, int j) {
	if(j+1<X) {
		graph.graph[i][j+1] = edge; 
	}
	if(j-1>=0) {
		graph.graph[i][j-1] = edge;
	}
	if(i+1<X) {
		graph.graph[i+1][j] = edge;
	}
	if(i-1>=0) {
		graph.graph[i-1][j] = edge;
	}
}

struct Graph removeObstacle(struct Graph graph, unsigned char Xn, unsigned char Yn) {
	int x = Xn-1;
	int y = (Yn%A);
	insertEdge(graph,0,x,y);
}

struct Graph createGraph() {
	struct Graph graph = init_graph();
	for(int i=0 ; i<X ; i++) {
		for(int j=0 ; j<X ; j++) {
			insertEdge(graph,1,i,j);
		}
	}
}