#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\\..\\Task 1A\\MazeSolve\\src.c"

struct Solution
{
    int maze_sol[5][25];
};

struct Solution parseSolution(char fileName[70])
{
    FILE *file;
    struct Solution soln;
    for(int i = 0; i < 25; i++)
    {
        soln.maze_sol[0][i] = -1; soln.maze_sol[1][i] = -1; soln.maze_sol[2][i] = -1;
        soln.maze_sol[3][i] = -1; soln.maze_sol[4][i] = -1;
    }
    int i = 0, j = 0;
    char buffer[200];
    file = fopen(fileName, "r");
    fgets(buffer, 200, (FILE*)file);

    char *token = strtok(buffer, " ");
    while(token != NULL)
    {
       int temp = atoi(token);
       soln.maze_sol[i][j] = temp;
       j++;
       if(temp == 24)
       {
           j = 0; i++;
       }
       //printf("%d\t", temp);
       token = strtok(NULL, " ");
    }
    return soln;
}

int main()
{
    struct Solution solution = parseSolution("..\\..\\Task 1A\\TestCases\\MazeSolution.txt");
    char fileName[70] = "..\\..\\Task 1A\\TestCases\\maze_0.txt";
    char fileName2[30] = "maze_0.txt";
    int passed = 0;
    printf("========================================================");
    for(int i = 0; i < 5; i++)
    {
        struct Maze maze;
        struct Path_Array path;
        int j = 0, flag = 0;
        fileName[29] = i + '0';
        fileName2[5] = i + '0';
        maze = parseFile(fileName);

        path = main_function(maze,0,24);

        printf("\n");
        while(solution.maze_sol[i][j] != -1)
        {
            if(solution.maze_sol[i][j] != path.path[j])
            {
                flag = 1;
                break;
            }
            j++;
        }
        if (flag == 1) printf("Test Case %s failed !\n", fileName2);
        else
        {
            printf("Test Case %s passed !\n", fileName2);
            passed++;
        }
    }
    printf("========================================================");
    if (passed == 5) printf("\nPerfect!! You are good to go!!");
    else printf("\n %d test case(s) failed. Try Harder!!", 5-passed);
    return 0;
}

