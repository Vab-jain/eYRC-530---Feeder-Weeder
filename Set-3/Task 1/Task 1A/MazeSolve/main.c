#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src.c"

/**Function Name: main
Input: None
Output: None
Logic: This function first reads the Test Case file and constructs Maze structure.
       Then it constructs Path_Array structure by calling main_function()
       Finally it prints the constructed path array.
Example Call: struct Path_Array path = main_function(maze,0,24);
**/

int main()
{
    struct Maze maze;
    int start = 0, finish = 24;
    maze = parseFile("..\\..\\Task 1A\\TestCases\\maze_0.txt");
    struct Path_Array path = main_function(maze, start, finish);
    for(int i = 0; i < X; i++)
    {
        if(path.path[i] == -1)
        {
            break;
        }
        printf("%d ", path.path[i]);
    }
    return 0;
}

