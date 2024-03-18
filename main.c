#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define EDGE struct edge

//structure of edge of graph
EDGE
{
    int start;
    int end;
    int weight;
};


//find minimal value
int min(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}


//find edge index with minimal length in graph
long long extract_min(int edge_num, int processed[], long long min_edge[])
{
    long long min_length = LONG_LONG_MAX;
    int min_index;
    for (int i = 0; i < edge_num; i++)
    {
        if (processed[i] == 0 && min_edge[i] < min_length)
        {
            min_length = min_edge[i];
            min_index = i;
        }
    }
    return min_index;
}


//Prim's algorithm
void prim(long long** graph, int edge_num, int vertex_num)
{
    int processed[edge_num];
    long long min_edge[edge_num], min_edge_index, parent[edge_num], len;

    for (int i = 0; i < edge_num; i++)
    {
        min_edge[i] = UINT_MAX;
        processed[i] = 0;
    }

    min_edge[0] = 0;
    parent[0] = -1;

    for (int i = 0; i < min(vertex_num, edge_num); i++)
    {
        min_edge_index = extract_min(edge_num, processed, min_edge);
        processed[min_edge_index] = 1;
        
        for (int j = 1; j < min(vertex_num, edge_num) + 1; j++)
        {
            if (graph[min_edge_index][j] && processed[j] == 0 && 
            graph[min_edge_index][j] <= min_edge[j])
            {
                parent[j] = min_edge_index;
                min_edge[j] = graph[min_edge_index][j];
            }
        }
    }

//print a result
    len = 0;
    for (int i = 1; i < min(vertex_num, edge_num); i++)
        len += graph[i][parent[i]];

    if (len == 0)
        puts("no spanning tree");
    else
        for (int i = 1; i < vertex_num; i++)
            printf("%lld %d\n", parent[i] + 1, i + 1);
}


//check for errors in input
int error_check(int vertex_num, int edge_num)
{
    if (vertex_num < 0 || vertex_num > 5000)
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


int main()
{
    FILE* file = fopen("in.txt", "r");

    int vertex_num, edge_num, start, end;
    long long weight;

    fscanf(file, "%d", &vertex_num);
    fscanf(file, "%d", &edge_num);

    if (error_check(vertex_num, edge_num) == -1)
        return 0;

    if (vertex_num == 1)
        return 0;

    long long** graph = (long long**)malloc(vertex_num * sizeof(long long *));
    int cnt = 0;

    for (int i = 0; i < vertex_num; i++)
        graph[i] = (long long *)calloc(vertex_num, sizeof(long long));

    for (int i = 0; i < edge_num; i++)
    {
        if (fscanf(file, "%d %d %lld", &start, &end, &weight) != 3)
        {
            fclose(file);
            puts("bad number of lines");
            return 0;
        };

        if (weight < 0 || weight > INT_MAX)
        {
            puts("bad length");
            return 0;
        }

        if (start < 1 || start > vertex_num ||
            end < 1 || end > vertex_num)
        {
            puts("bad vertex");
            return 0;
        }

        if (end == vertex_num || start == vertex_num)
            cnt++;

        graph[start - 1][end - 1] = weight;
        graph[end - 1][start - 1] = weight;
    }
    if (cnt == 0)
    {
        puts("no spanning tree");
        return 0;
    }

    if (edge_num + 1 == vertex_num)
    {
        for (int i = 0; i < edge_num + 1; i++)
            for (int j = i + 1; j < edge_num + 1; j++)
                if (graph[i][j] != 0)
                    printf("%d %d\n", i + 1, j + 1);
        return 0;
    }
    prim(graph, edge_num, vertex_num);

    for (int i = 0; i < vertex_num; i++)
        free(graph[i]);
    free(graph);

    fclose(file);
    return 0;
}