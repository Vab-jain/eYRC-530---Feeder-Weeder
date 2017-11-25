/*********************************************** path_planning.h *****************************************************/

/* Header file for all functions that contribute to the creation of the graph from flex and finding the shortest path from it */ 

#ifndef Header_files
#define Header_files
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#endif

#define Y 7
#define X 49   // number of nodes in the flex
#define A 65  
#define MAX 100
#define false 0
#define true 1 

/** Structure Name: Graph
 ** Purpose: Used to store the Graph constructed by the createGraph() function.
 **          The graph is represented in form of a 2D array wrapped in a structure.
 **/
struct Graph
{
	_Bool graph[X][X];
};

/** Structure Name: Path_Array
 ** Purpose: Used to store the final path Array constructed by the findPath() function.
 **          The path is represented in form of a 1D array wrapped in a structure.
 **/
struct Path_Array {
	int path[X];
};


/************************ All Function Prototypes **************************/

struct Graph createGraph();
struct Graph removeObstacle(struct Graph graph, unsigned char Xn, unsigned char Yn);
struct Graph addObstacle(struct Graph graph, unsigned char Xn, unsigned char Yn);
void insertEdge(struct Graph *graph, _Bool edge, int i);
struct Path_Array init_path();
int find_min(int arr[], int size, _Bool spt_set[]);
void set_Path_Array(struct Path_Array *path, int parent[], int node, int *counter);
struct Path_Array findPath(struct Graph graph, int start, int finish);


/************************* Defining all the functions **********************/

/** Function Name: insertEdge
 ** Input: graph - it is the pointer to the graph reperesenting the flex
 		   edge - boolean value to initialize the graph so as to insert the edge
 		   node - it is the present node of the graph whose in/out edges are to inserted into the graph 
 ** Output: none
 **/
void insertEdge(struct Graph *graph, _Bool edge, int node) {
	if((node+1)%Y!=0) {  // extreme right node condition
		graph->graph[node][node+1] = edge;
	}
	if(node%Y!=0) {  // extreme left node condition
		graph->graph[node][node-1] = edge;
	}
	if(node+Y<X) {   // extreme bottom node condition 
		graph->graph[node][node+Y] = edge;
	}
	if(node-Y>=0) {  // extreme top node condition
		graph->graph[node][node-Y] = edge;
	}
}

/** Function Name: coordinateMapping
 ** Input: Xn - x coordinate of any point on the flex
 		   Yn - y coordinate of any point on the flex
 ** Output: returns node representing the present coordinate on the flex
 **/
int coordinateMapping(unsigned char Xn, unsigned char Yn) {
	int y = (Yn%A);
	return (((Xn-1)*Y) + y);
}

/** Function Name: removeObstacle 
 ** Input: graph - it is the structure representing the flex as a graph of 2D-Matrix
  		   Xn - x coordinate of the obstacle on the flex
  		   Yn - y coordinate of the obstacle on the flex
 ** Output: Returns a graph structure after removing the obstacle from the given graph
 **/
struct Graph removeObstacle(struct Graph graph, unsigned char Xn, unsigned char Yn) {
	int node = coordinateMapping(Xn,Yn);
	insertEdge(&graph,0,node);
	return graph;
}

/** Function Name: addObstacle 
 ** Input: graph - it is the structure representing the flex as a graph of 2D-Matrix
  		   Xn - x coordinate of the obstacle on the flex
  		   Yn - y coordinate of the obstacle on the flex
 ** Output: Returns a graph structure after adding the obstacle to the given graph
 **/
struct Graph addObstacle(struct Graph graph, unsigned char Xn, unsigned char Yn) {
	int node = coordinateMapping(Xn,Yn);
	insertEdge(&graph,1,node);
	return graph;
}

/** Function Name: createGraph
 ** Input: None
 ** Output: Returns a graph structure after creating and initializing it with zeroes by calling the init_graph() function
 **/
struct Graph createGraph() {

	// initializing the graph with zeroes
	struct Graph graph = {0}; 

	for(int i=0 ; i<X ; i++) {
		insertEdge(&graph,1,i);
	}
	return graph;
}


/** Function Name: init_path()
 ** Input: None
 ** Output: Returns a Path_Array structure in which all elements of array are initialized to -1
 ** Example Call: struct Path_Array path = init_path();
 **/
struct Path_Array init_path()
{	
	struct Path_Array path;
	for(int i = 0; i < X; i++)
		path.path[i] = -1;
	return path;
}


/**
 ** Function Name: find_min
 ** Input: arr - containing the distances
 **        size - size of the array
 **        spt_set - boolean array to show that whether the node is inserted in the shortest path or not
 ** Output: returns the index of the minimum distance from the array
 **/
int find_min(int arr[], int size, _Bool spt_set[]) {
    int min = MAX, min_pos = -1;
    for(int i = 0; i < size; i++) {
        if(min > arr[i] && spt_set[i] == false) {
            min = arr[i];
            min_pos = i;
        }
    }
    return min_pos;
}

/**
 ** Function Name: set_Path_Array
 ** Input: Path_Array Structure - which contains the path in the maze from the src to dst.
           parent Array - stores the parent of the corresponding node in the path
           node - represent the node in the traversal of the path
           counter - stores the position of the element to be inserted into the Path_Array.path 1D array
 ** Output: Nothing
 **/
 void set_Path_Array(struct Path_Array *path, int parent[], int node, int *counter) {

    if(parent[node] == -1) {
        path->path[(*counter)++] = node;
        return;
    }

    set_Path_Array(path,parent,parent[node],counter);

    path->path[(*counter)++] = node;

 }
 

/** Function Name: findPath
 ** Input: graph - Graph structure
 **        src - start point of path
 **        dst - end point of path
 ** Output: Returns a Path_Array structure which contains the path in the maze from src to dst.
 ** Example Call: struct Path_Array path = findPath(graph, 0, 24)
 **/
struct Path_Array findPath(struct Graph graph, int start, int finish)
{
	struct Path_Array path = init_path();

    /**
     ** array containing the distance of any point from the source
     **/
    int distance[X];

    /**
     ** array to store the parent of the corresponding node occurring in the shortest path
     **/
    int parent[X];

    /**
     ** this array will be set to true if the node in inserted in the shortest path
     ** else it will be set to false
     **/
    _Bool spt_set[X] = {};

    /**
     ** Initializing the distance array with maximum value
     **/
    for(int i = 0; i < X; i++) {
        distance[i] = MAX;
        parent[i] = -1;
    }

    distance[start] = 0;

    int present_node = start;

    while(present_node != finish) {

        present_node = find_min(distance,X,spt_set);

        if(present_node == -1) { break; }

        spt_set[present_node] = true;

        for(int j = 0; j < X; j++) {

            if(graph.graph[present_node][j] && !spt_set[j] && distance[j]>distance[present_node]+1) {
                distance[j] = distance[present_node] + 1;
                parent[j] = present_node;
            }
        }

    }

    int counter = 0;

    set_Path_Array(&path,parent,finish,&counter);

    return path;
}

#endif

/***************** end of the header file ***********************/