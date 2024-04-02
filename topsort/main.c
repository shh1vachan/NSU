#include <stdio.h>
#include <stdlib.h>

#define GRAPH struct graph


//structure of graph
GRAPH
{
    int vertex_num;
    int *adjacency;
    int *incoming_edges;
};


//initialize graph
GRAPH* create_graph(int vertex_num)
{
    GRAPH* graph = (GRAPH*)malloc(sizeof(GRAPH));
    graph->vertex_num = vertex_num;
    graph->adjacency = (int*)calloc(vertex_num * vertex_num, sizeof(int));
    graph->incoming_edges = (int*)calloc(vertex_num, sizeof(int));
    return graph;
}


//check for input errors
int error_check(int edge_num, int vertex_num)
{
    if (vertex_num < 0 || vertex_num > 2000)
    {
        puts("bad number of vertices");
        return -1;
    }

    if (edge_num < 0 || edge_num > ((vertex_num * (vertex_num + 1)) / 2))
    {
        puts("bad number of edges");
        return -1;
    }

    return 0;
}


//topological sort
void topsort(GRAPH *graph)
{
    int *result = (int*)malloc(graph->vertex_num * sizeof(int));
    int result_index = 0;

    int *queue = (int*)malloc(graph->vertex_num * sizeof(int));
    int queue_front = 0;
    int queue_rear = 0;

    for (int i = 0; i < graph->vertex_num; i++)
    {
        if (graph->incoming_edges[i] == 0)
            queue[queue_rear++] = i;
    }

    while (queue_front != queue_rear)
    {
        int current_vertex = queue[queue_front++];

        result[result_index++] = current_vertex + 1;

        for (int i = 0; i < graph->vertex_num; i++)
        {
            if (graph->adjacency[current_vertex * graph->vertex_num + i] == 1)
            {
                graph->incoming_edges[i]--;

                if (graph->incoming_edges[i] == 0)
                    queue[queue_rear++] = i;
            }
        }
    }

//print a result
    if (result_index != graph->vertex_num)
        puts("impossible to sort");

    else
    {
        for (int i = 0; i < graph->vertex_num; i++)
            printf("%d ", result[i]);

        printf("\n");
    }

    free(result);
    free(queue);
}


int main()
{
    int vertex_num, edge_num;
    int edge_start, edge_end;
    FILE* file = fopen("in.txt", "r");

    fscanf(file, "%d", &vertex_num);
    fscanf(file, "%d", &edge_num);

    GRAPH* graph = create_graph(vertex_num);

    if (fscanf(file,"%d %d", &edge_start, &edge_end) != 2)
    {
        fclose(file);
        puts("bad number of lines");
        return 0;
    }
    graph->adjacency[(edge_start - 1) * vertex_num + (edge_end - 1)] = 1;
    graph->incoming_edges[edge_end - 1]++;

    if (error_check(edge_num, vertex_num) == -1)
        return 0;

//check for errors in edge data
    for (int i = 1; i < edge_num; i++)
    {
        if (fscanf(file,"%d %d", &edge_start, &edge_end) != 2)
        {
            fclose(file);
            puts("bad number of lines");
            return 0;
        }

        if (edge_start == edge_end)
        {
            puts("bad number of edges");
            return 0;
        }

        if (edge_start < 1 || edge_start > vertex_num || edge_end < 1 
        || edge_end > vertex_num) 
        {
            puts("bad vertex");
            return 0;
        }

        graph->adjacency[(edge_start - 1) * vertex_num + (edge_end - 1)] = 1;
        graph->incoming_edges[edge_end - 1]++;
    }

    topsort(graph);

    return 0;
}
