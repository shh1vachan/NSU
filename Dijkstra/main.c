#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define GRAPH struct graph
#define NODE struct NODE
#define VISITED struct VisitedNode


//structure of node
NODE
{
    int vertex;
    long long weight;
    NODE* next;
};


//structure of graph with adjacency list
GRAPH
{
    int vert_num;
    NODE** adjlist;
};


//info about visited vertexes
VISITED
{
    long long distance;
    int processed;
    int parent;
};


//create a graph
GRAPH* create_graph(int vert)
{
    GRAPH* graph = (GRAPH*)malloc(sizeof(GRAPH));
    graph->vert_num = vert;
    graph->adjlist = (NODE**)malloc(vert * sizeof(NODE*));
    for (int i = 0; i < vert; i++)
        graph->adjlist[i] = NULL;
    return graph;
}


//create vertex of a graph
NODE* create_node(int vertex, long long weight)
{
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    newNode->vertex = vertex;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}


//add edge between two vertexes
void add_edge(GRAPH* graph, int start, int end, long long weight)
{
    NODE* newNode = create_node(end, weight);
    newNode->next = graph->adjlist[start - 1];
    graph->adjlist[start - 1] = newNode;

    newNode = create_node(start, weight);
    newNode->next = graph->adjlist[end - 1];
    graph->adjlist[end - 1] = newNode;
}


//find minimal edge in adjacency list
int extract_min(NODE* priority_queue, VISITED* VisitedNodes)
{
    if (!priority_queue)
        return -1;

    long long min_distance = LLONG_MAX;
    int min_vertex = -1;
    NODE* current = priority_queue;

    while (current)
    {
        int vertex = current->vertex;
        if (!VisitedNodes[vertex].processed && VisitedNodes[vertex].distance
                                               <= min_distance)
        {
            min_distance = VisitedNodes[vertex].distance;
            min_vertex = vertex;
        }
        current = current->next;
    }
    return min_vertex;
}


//Dijkstra's algorithm
void dijkstra(GRAPH* graph, int start_vert, int end_vert)
{
    VISITED* VisitedNodes = (VISITED*)malloc(graph->vert_num *sizeof(VISITED));
    for (int i = 0; i < graph->vert_num; i++)
    {
        VisitedNodes[i].distance = LLONG_MAX;
        VisitedNodes[i].processed = 0;
        VisitedNodes[i].parent = -1;
    }

    VisitedNodes[start_vert - 1].distance = 0;
    NODE* priority_queue = NULL;

    NODE* newNode = create_node(start_vert - 1, 0);
    priority_queue = newNode;

    while (priority_queue)
    {
        int min_edge = extract_min(priority_queue, VisitedNodes);
        if (min_edge == -1)
            break;
        
        VisitedNodes[min_edge].processed = 1;
        NODE* node_adj = graph->adjlist[min_edge];

        while (node_adj)
        {
            int adj_vertex = node_adj->vertex - 1;
            long long alt = VisitedNodes[min_edge].distance + node_adj->weight;

            if (alt <= VisitedNodes[adj_vertex].distance)
            {
                VisitedNodes[adj_vertex].distance = alt;
                VisitedNodes[adj_vertex].parent = min_edge;

                NODE* newNode = create_node(adj_vertex, alt);
                newNode->next = priority_queue;
                priority_queue = newNode;
            }
            node_adj = node_adj->next;
        }
    }

//print a result    
    int path[graph->vert_num];
    int path_len = 0;

    int current = end_vert - 1;
    while (current != -1)
    {
        path[path_len++] = current + 1;
        current = VisitedNodes[current].parent;
    }

    for (int i = 0; i < graph->vert_num; i++)
    {
        if (VisitedNodes[i].distance == LLONG_MAX)
            printf("oo ");
        else if (VisitedNodes[i].distance > INT_MAX)
            printf("INT_MAX+ ");
        else
            printf("%lld ", VisitedNodes[i].distance -
            VisitedNodes[start_vert - 1].distance);
    }
    printf("\n");

    int checker1 = 0;
    int checker2 = 0;

    for (int i = 0; i < path_len; i++)
    {
        if (path[i] == start_vert || path[i] == end_vert)
            checker1++;
        if (VisitedNodes[i + 1].distance >= INT_MAX &&
        VisitedNodes[i + 1].distance != LLONG_MAX)
            checker2++;
    }

    if (checker1 != 2 && start_vert != end_vert)
    {
        puts("no path");
        exit(0);
    }
    else if (VisitedNodes[path_len - 1].distance > INT_MAX && checker2 > 2)
    {
        printf("overflow");
        exit(0);
    }

    for (int i = 0; i < path_len; i++)
        printf("%d ", path[i]);

    printf("\n");
}


//check for errors in input 
void input_check(int vert_num, int start, int end, int edge_num)
{
    if (vert_num < 0 || vert_num > 5000)
    {
        puts("bad number of vertices");
        exit(0);
    }
    if (start < 1 || start > vert_num || end < 1 || end > vert_num )
    {
        puts("bad vertex");
        exit(0);
    }
    if (edge_num < 0 || edge_num > vert_num * (vert_num - 1) / 2)
    {
        puts("bad number of edges");
        exit(0);
    }
}


int main()
{
    int vert_num, start, end, edge_num;
    int edge_start, edge_end;
    long long edge_len;
    GRAPH* graph;

    scanf("%d", &vert_num);
    scanf("%d", &start);
    scanf("%d", &end);
    scanf("%d", &edge_num);
    input_check(vert_num, start, end, edge_num);

    graph = create_graph(vert_num);

    int strcnt = 0;
    for (int i = 0; i < edge_num; i++)
    {
        scanf("%d", &edge_start);
        scanf("%d", &edge_end);
        scanf("%lld", &edge_len);

        if (edge_len < 0 || edge_len > INT_MAX)
        {
            puts("bad length");
            exit(0);
        }

        add_edge(graph, edge_start, edge_end, edge_len);
        strcnt++;
    }

    if (strcnt < edge_num)
    {
        printf("bad number of lines");
        return 0;
    }
    dijkstra(graph, start, end);

    return 0;
}
