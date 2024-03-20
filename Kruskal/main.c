#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define EDGE struct EDGE


//structure of edge
EDGE
{
    int start;
    int end;
    long long weight;
};


//compare two values for qsort function
int compare(const void* a, const void* b)
{
    return ((EDGE *) a)->weight - ((EDGE *) b)->weight;
}


//find first father of vertex
int find_parent(int vertex, int parent[])
{
    if (parent[vertex] == -1)
        return vertex;
    return find_parent(parent[vertex], parent);
}


//Kruskal"s algorithm
void kruskal(EDGE* graph_edges, int edge_num, int vertex_num)
{
    int* parent = (int*)malloc(vertex_num * sizeof(int));
    EDGE* result = (EDGE*)malloc((vertex_num - 1) * sizeof(EDGE));
    int spanning_edges = 0;
    
    for (int i = 0; i < vertex_num; ++i)
        parent[i] = -1;

    qsort(graph_edges, edge_num, sizeof(EDGE), compare);

    int i = 0;
    while (spanning_edges < vertex_num - 1 && i < edge_num)
    {
        EDGE current_edge = graph_edges[i++];

        int first_parent = find_parent(current_edge.start, parent);
        int last_parent = find_parent(current_edge.end, parent);

        if (first_parent != last_parent)
        {
            result[spanning_edges++] = current_edge;
            parent[first_parent] = last_parent;
        }
    }

//print a result    
    if (spanning_edges < vertex_num - 1)
    {
        puts("no spanning tree");
    }
    else
    {
        for (int i = 0; i < vertex_num - 1; ++i)
            printf("%d %d\n", result[i].end + 1, result[i].start + 1);
    }
    free(parent);
    free(result);
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

    if (vertex_num == 1)
        return -1;

    return 0;
}


int main()
{
    int vertex_num, edge_num;
    FILE* file = fopen("in.txt", "r");
    fscanf(file, "%d %d", &vertex_num, &edge_num);

    if (error_check(vertex_num, edge_num) == -1)
        return 0;

    EDGE* edges = (EDGE*)malloc(edge_num * sizeof(EDGE));
    int cnt = 0;
    for (int i = 0; i < edge_num; i++)
    {
        if (fscanf(file, "%d %d %lld",
                   &edges[i].start, &edges[i].end, &edges[i].weight) != 3)
        {
            fclose(file);
            puts("bad number of lines");
            return 0;
        };

        if (edges[i].weight < 0 || edges[i].weight > INT_MAX)
        {
            puts("bad length");
            return 0;
        }

        if (edges[i].start < 1 || edges[i].start > vertex_num ||
        edges[i].end< 1 || edges[i].end > vertex_num)
        {
            puts("bad vertex");
            return 0;
        }

        if (edges[i].end == vertex_num || edges[i].start == vertex_num)
            cnt++;

        edges[i].start--;
        edges[i].end--;

    }
    if (cnt == 0)
    {
        puts("no spanning tree");
        return 0;
    }

    kruskal(edges, edge_num, vertex_num);

    free(edges);
    fclose(file);
    return 0;
}
