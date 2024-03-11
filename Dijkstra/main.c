#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define GRAPH struct graph
#define NODE struct NODE
#define VISITED struct VisitedNODE

NODE
{
    int vertex;
    long long weight;
    NODE* next;
};

GRAPH
{
    int vert_num;
    NODE** adjlist;
};

VISITED
{
    long long distance;
    int processed;
    int parent;
};

GRAPH* create_graph(int vert)
{
    GRAPH* graph = (GRAPH*)malloc(sizeof(GRAPH));
    graph->vert_num = vert;
    graph->adjlist = (NODE**)malloc(vert * sizeof(NODE*));
    for (int i = 0; i < vert; i++)
        graph->adjlist[i] = NULL;
    return graph;
}

void add_edge(GRAPH* graph, int start, int end, long long weight)
{

    NODE* newNODE = (NODE*)malloc(sizeof(NODE));
    newNODE->vertex = end;
    newNODE->weight = weight;
    newNODE->next = graph->adjlist[start - 1];
    graph->adjlist[start - 1] = newNODE;
}

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

void dijkstra(GRAPH* graph, int start_vert, int end_vert)
{
    VISITED* VisitedNodes = (VISITED*)malloc(graph->vert_num * sizeof(VISITED));
    for (int i = 0; i < graph->vert_num; i++)
    {
        VisitedNodes[i].distance = LLONG_MAX;
        VisitedNodes[i].processed = 0;
        VisitedNodes[i].parent = -1;
    }

    VisitedNodes[start_vert - 1].distance = 0;
    NODE* priority_queue = NULL;

    NODE* newNODE = (NODE*)malloc(sizeof(NODE));
    newNODE->vertex = start_vert - 1;
    newNODE->weight = 0;
    newNODE->next = NULL;
    priority_queue = newNODE;

    while (priority_queue)
    {
        int u = extract_min(priority_queue, VisitedNodes);
        if (u == -1)
            break;

        VisitedNodes[u].processed = 1;

        NODE* temp = graph->adjlist[u];

        while (temp)
        {
            int v = temp->vertex - 1;
            long long alt = VisitedNodes[u].distance + temp->weight;
            if (alt <= VisitedNodes[v].distance)
            {
                VisitedNodes[v].distance = alt;
                VisitedNodes[v].parent = u;

                NODE* newNODE = (NODE*)malloc(sizeof(NODE));
                newNODE->vertex = v;
                newNODE->weight = alt;
                newNODE->next = priority_queue;
                priority_queue = newNODE;
            }
            temp = temp->next;
        }
    }

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

    int tmp = 0;
    int tmp2 = 0;
    for (int i = 0; i < path_len; i++)
    {
        if (path[i] == start_vert || path[i] == end_vert)
            tmp++;
        if (VisitedNodes[i + 1].distance >= INT_MAX &&
        VisitedNodes[i + 1].distance != LLONG_MAX)
            tmp2++;
    }

    if (tmp != 2 && start_vert != end_vert)
    {
        puts("no path");
        exit(0);
    }
    else if (VisitedNodes[path_len - 1].distance > INT_MAX && tmp2 > 2)
    {
        printf("overflow");
        exit(0);
    }

    for (int i = 0; i < path_len; i++)
        printf("%d ", path[i]);

    printf("\n");
}


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

        if (edge_end < edge_start)
            add_edge(graph, edge_end, edge_start, edge_len);
        else
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
