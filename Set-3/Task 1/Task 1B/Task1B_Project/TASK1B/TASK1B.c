/*
 * TASK1B.c
 *
 * Created: 04/11/2017 22:58:28
 *  Author: Avinash Dubey
 */ 


#include <avr/io.h>
#define F_CPU 14745600
#include <util/delay.h>
#include "buzzer.h"
#include "path_planning.h"

//global start variable to store the starting position of the firebird V and initializing it with the first starting position
int start = 45;

//global present direction to store the present direction at which the firebird is pointing
int present_direction = 1;

/** Structure Name: Direction
 ** Purpose: Used to store the directions to be traversed by the firebird. It has three values -1,0,1 and additional value of 2 to stop.
 **          -1 - left, 0 - straight, 1 - right, 2 - destination has been reached.
 **/
struct Direction {
	int directions[X];
};


struct Direction moveInThePath(struct Path_Array path) {
	
	int next_direction;
	int path_it = 1;
	int next_node, present_node;
	present_node = path.path[path_it-1];
	next_node = path.path[path_it];
	struct Direction direction;

	while(next_node!=-1) {
		if(next_node - present_node == 1) {
			next_direction = 2;
		} else if(next_node - present_node == -1) {
			next_direction = 0;
		} else if(next_node - present_node == 7) {
			next_direction = 3;
		} else if(next_node - present_node == -7) {
			next_direction = 1;
		}

		int turn = next_direction - present_direction;

		if(turn == -1 || turn == 3) {
			direction.directions[path_it-1] = -1;
		} else if(turn == 1 || turn == -3) {
			direction.directions[path_it-1] = 1;
		} else if(turn == 0) {
			direction.directions[path_it-1] = 0;
		}

		present_node = next_node;
		next_node = path.path[++path_it];
		present_direction = next_direction;
	}

	direction.directions[path_it-1] = 2;

	return 	direction;
}


void ReachDestinationAvoidingNode(unsigned char Xd,unsigned char Yd,unsigned char Xn,unsigned char Yn)
{
	//TODO:: Please write your application code. You can make your own functions & header files

	//creating the graph and removing the obstacle from it
	struct Graph graph = createGraph();
	graph = removeObstacle(graph,Xn,Yn);

	//mapping the coordinates of the destination node on the flex to the node number in the graph
	int finish = coordinateMapping(Xd,Yd);

	//finding the shortest path from start finish after removing the obstacle.
	struct Path_Array path_array = findPath(graph,start,finish);

	//next start position will the destination node of the firebirds current path
	start = finish;

	//call the function to run the firebird V on the specified shortest path.
	//localization of the firebird V
	struct Direction directions = moveInThePath(path_array);

}


//Do not make changes in main function

int main(void)
{
       ReachDestinationAvoidingNode(5,'D',6,'D');
	   buzzer_on();
	   _delay_ms(500);
	   buzzer_off();
	   ReachDestinationAvoidingNode(2,'F',2,'D');
	   buzzer_on();
	   _delay_ms(500);
	   buzzer_off();
	   ReachDestinationAvoidingNode(2,'A',2,'C');
	   buzzer_on();
}