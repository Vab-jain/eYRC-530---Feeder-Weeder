#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define X 25
#define Y 5

#define MAX 100
#define false 0
#define true 1

/** Structure Name: Maze
 ** Purpose: Used to store maze information retrieved from file.
 **          The maze is represented in form of a 2D array wrapped in a structure.
 **/
struct Maze
{
    int maze_array[Y][Y];
};

/** Structure Name: Graph
 ** Purpose: Used to store the Graph constructed by the buildGraph() function.
 **          The graph is represented in form of a 2D array wrapped in a structure.
 **/
struct Graph
{
    _Bool graph_array[X][X];
};

/** Structure Name: Path_Array
 ** Purpose: Used to store the final path Array constructed by the findPath() function.
 **          The path is represented in form of a 1D array wrapped in a structure.
 **/
struct Path_Array
{
	int path[X];
};

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
/** Function Name: buildGraph()
 ** Input: Accepts a Maze structure as input
 ** Output: Returns a Graph structure as output
 ** Purpose: Constructs a adjacency matrix graph using the maze information passed as argument
 ** Example Call: struct Graph matrix = buildGraph(maze);
 **/
struct Graph buildGraph(struct Maze maze)
{
    struct Graph adj_matrix;

    /**
     ** Initializing the adjacency matrix of the graph
     **/
    for(int i = 0; i < X; i++) {
        for(int j = 0; j < X;  j++) {
            adj_matrix.graph_array[i][j] = 0;
        }
    }

    for(int i = 0; i < Y; i++) {

        for(int j = 0; j < Y; j++) {
            _Bool north = false, east = false, south = false, west = false;
            int cell_value = maze.maze_array[i][j];
            int node_number = Y*i+j;

            /**
             ** checking the empty direction of the cell
             **/

            if(cell_value%2 == 0) {
                north = true;
            } else cell_value-=1;


            if((cell_value-2) != 0 && (cell_value-2) != 4 && (cell_value-2) != 8 && (cell_value-2) != 12) {
                east = true;
            }

            if((cell_value-4) != 0 && (cell_value-4) != 2 && (cell_value-4) != 8 && (cell_value-4) != 10) {
                south = true;
            }

            if((cell_value-8) != 0 && (cell_value-8) != 2 && (cell_value-8) != 4 && (cell_value-8) != 6) {
                west = true;
            }

            /**
             ** setting the value of corresponding node in the adjacency matrix
             **/

            if(east == true) {
                adj_matrix.graph_array[node_number][node_number+1] = 1;
            }

            if(west == true) {
                adj_matrix.graph_array[node_number][node_number-1] = 1;
            }

            if(north == true) {
                adj_matrix.graph_array[node_number-1][node_number] = 1;
            }

            if(south == true) {
                adj_matrix.graph_array[node_number+1][node_number] = 1;
            }

        }

    }

    return adj_matrix;
}

/**
 ** My Utility Function
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
 ** My Utility Function
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

            if(graph.graph_array[present_node][j] && !spt_set[j] && distance[j]>distance[present_node]+1) {
                distance[j] = distance[present_node] + 1;
                parent[j] = present_node;
            }
        }

    }

    int counter = 0;

    set_Path_Array(&path,parent,finish,&counter);

    return path;
}

/** Function Name: main_function
 ** Input: maze - Maze structure
 **        start - start point of path
 **        finish - end point of path
 ** Output: Returns a Path_Array structure which contains the path in the maze from start to finish.
 ** Logic: This function first constructs a graph from the maze passed as argument and then finds out
 **        path from start to finish. The path is returned as a Path_Array structure.
 ** Example Call: struct Path_Array path = main_function(maze,0,24);
 **/
struct Path_Array main_function(struct Maze maze, int start, int finish)
{
    struct Graph adjacency_matrix = buildGraph(maze);
    struct Path_Array path = findPath(adjacency_matrix, start, finish);
	return path;
}

/** Function Name: parseFile
 ** Input: fileName -  a string containing the relative path of the test case
 ** Output: Returns a Maze structure.
 ** Logic: This function reads the the specified txt file in the TestCases folder and constructs
           a Maze structure from the information.
 ** Example Call: struct Maze maze = parseFile("..\\..\\Task 1A\\TestCases\\maze_4.txt");
 **/
struct Maze parseFile(char fileName[70])
{
    FILE *fp;
    char buff[100];
    int i = 0, j = 0;
    struct Maze maze;
    fp = fopen(fileName,"r");
    fgets(buff, 100, (FILE*)fp);
    char *token = strtok(buff," ");
    while(token != NULL)
    {
        int temp = atoi(token);
        maze.maze_array[i][j] = temp;
        j++;
        if(j == 5)
        {
            i++;
            j = 0;
        }
        token = strtok(NULL, " ");
    }
    return maze;
}

/**************************************** Utility Functions *********************************/

/** Function Name: coord_to_flat
 ** Input: Accepts coordinates x and y of maze cell as input.
 ** Logic: The maze structure is represented as a 2D array. The cells of the maze can be numbered
 **        in a sequential form (0,1,2,3,4,5....23,24) so that it is easier to construct a graph.
 **        This function accepts the maze cell coordinates and returns an integer.
 ** Output: Returns an integer representing the position of maze cell relative to the start position.
 ** Example Call: coord_to_flat()
 **/
int coord_to_flat(int x, int y)
{
    return x*Y + y;
}

/**************************************** Utility Functions *********************************/
