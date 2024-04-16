#include <stdio.h>
#include <stdlib.h>

#define STACK struct stack


//structure of stack
STACK
{
    int value;
    STACK* next;
};


//create empty stack
STACK* create_stack()
{
    STACK* element = (STACK*)malloc(sizeof(STACK));
    element->next = NULL;
    return element;
}


//add new element to stack
void push(STACK* stack, int value)
{
    STACK* element = (STACK*)malloc(sizeof(STACK));
    element->value = value;
    element->next = stack->next;
    stack->next = element;
}


//delete element from stack
int pop(STACK* stack)
{
    int value = stack->next->value;
    STACK* temp = stack->next;
    stack->next = stack->next->next;
    free(temp);
    return value;
}


//depth first search function
void dfs(int* adj_matrix, int vertex_num, int* visited, int v, STACK* stack)
{
    visited[v] = 1;
    for (int i = 0; i < vertex_num; i++)
        if (adj_matrix[v * vertex_num + i] && !visited[i])
            dfs(adj_matrix, vertex_num, visited, i, stack);
    push(stack, v);
}


//reverse depth first search function
void rev_dfs(int* rev_adj_matrix, int vertex_num, int* visited,
             int v, STACK* result)
{
    visited[v] = 1;
    for (int i = 0; i < vertex_num; i++)
        if (rev_adj_matrix[v * vertex_num + i] && !visited[i])
            rev_dfs(rev_adj_matrix, vertex_num, visited, i, result);
    push(result, v);
}


//kosaraju algorithm
void kosaraju(int* adj_matrix, int* rev_adj_matrix, int vertex_num)
{
    int* visited = (int*)calloc(vertex_num, sizeof(int));
    STACK* stack = create_stack();
    STACK* result = create_stack();

    for (int i = 0; i < vertex_num; i++)
        if (!visited[i])
            dfs(adj_matrix, vertex_num, visited, i, stack);

    for (int i = 0; i < vertex_num; i++)
        visited[i] = 0;


 //print a result
    while (stack->next != NULL)
    {
        int vertex = pop(stack);
        if (!visited[vertex])
        {
            rev_dfs(rev_adj_matrix, vertex_num, visited, vertex, result);

            if (result->next->next == NULL)
                pop(result);
            else
            {
                while (result->next != NULL)
                    printf("%d ", pop(result) + 1);
                printf("\n");
            }
        }
    }
    free(visited);
    free(stack);
    free(result);
}


int main()
{
    int vertex_num, edge_num, edge_start, edge_end;

    scanf("%d %d", &vertex_num, &edge_num);

    int* adj_matrix = (int*)calloc(vertex_num * vertex_num, sizeof(int));
    int* rev_adj_matrix = (int*)calloc(vertex_num * vertex_num, sizeof(int));

    for (int i = 0; i < edge_num; i++)
    {
        scanf("%d %d", &edge_start, &edge_end);
        adj_matrix[(edge_start - 1) * vertex_num + (edge_end - 1)] = 1;
        rev_adj_matrix[(edge_end - 1) * vertex_num + (edge_start - 1)] = 1;
    }

    kosaraju(adj_matrix, rev_adj_matrix, vertex_num);

    free(adj_matrix);
    free(rev_adj_matrix);

    return 0;
}
