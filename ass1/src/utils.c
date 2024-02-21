#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


Graph * parse_file(FILE *input){
    char line[128];
    long src, dest;
    long num_nodes = 0;

    //first we need to count the number of nodes
    //We dont read just the last line of the file, because we are not sure if the nodes come in order
    while (fgets(line, sizeof(line), input) != NULL) {
        //ignore lines starting with #
        if (line[0] == '#')
            continue;

        if(fscanf(input, "%ld", &src) == 1){
            if(src > num_nodes)
                num_nodes = src;
        }
    }
    rewind(input); //reset the fp, to read the file from the beginning

    //Construct the graph, reading from the file
    Graph* g = createGraph(num_nodes + 1);
    while (fgets(line, sizeof(line), input) != NULL) {
        if (line[0] == '#')
            continue;

        if(fscanf(input, "%ld %ld", &src, &dest) == 2){
            addEdge(g, src, dest);
        }
    }  

    return g;
}

node *newNode(long v){
    node *new_node = (node *)malloc(sizeof(node));
    new_node->vertex = v;
    new_node->next = NULL;
    return new_node;
}

Graph *createGraph(long numVertices){
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = numVertices;

    graph->adjLists = (node **)malloc(numVertices * sizeof(node *));

    for(long i = 0; i < numVertices; i++){
        graph->adjLists[i] = NULL;
    }

    return graph;
}

void addEdge(Graph *g, long src, long dest){
    node *new_node = newNode(dest);
    new_node->next = g->adjLists[src];
    g->adjLists[src] = new_node;
}

// Delete later!!!
void printGraph(struct Graph* graph) {
  for (long v = 0; v < graph->numVertices; v++) {
    struct node* temp = graph->adjLists[v];
    if(temp)
        printf("\n Vertex %ld: \n", v);
    while (temp) {
      printf("%ld -> ", temp->vertex);
      temp = temp->next;
    }
    printf("\n");
  }
}


int export_csv(FILE *output_file, int data){
    fprintf(output_file, "node,pagerank\n");

    for(int i = 0; i < data; i++){
        fprintf(output_file, "test: %d,%f\n", i, 0.85);
    }
    
    return 0;
}