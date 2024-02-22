#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define DAMPING_FACTOR 0.85


extern long num_nodes;

vertex **parse_file(FILE *input){
    char line[128];
    long src, dest;

    //first we need to count the number of nodes
    //We dont read just the last line of the file, because we are not sure if the nodes come in order
    while (fgets(line, sizeof(line), input) != NULL) {
        //ignore lines starting with #
        if (line[0] == '#')
            continue;

        if(fscanf(input, "%ld %ld", &src, &dest) == 2){
            if(src > num_nodes)
                num_nodes = src;
            if(dest > num_nodes)
                num_nodes = dest;
        }
    }
    rewind(input); //reset the fp, to read the file from the beginning

    //Construct the graph, reading from the file
    vertex **verts = malloc((num_nodes + 1) * sizeof(vertex*)); 
    while (fgets(line, sizeof(line), input) != NULL) {
        if (line[0] == '#')
            continue;

        if(fscanf(input, "%ld %ld", &src, &dest) == 2){
            //if the vertex does not already exist, create it (if it hasnt been seen in the file yet)
            if(verts[src] == NULL)
                verts[src] = create_vertex(src);
            if(verts[dest] == NULL)
                verts[dest] = create_vertex(dest);

            add_edge_toList(verts[dest], src);  //add incoming edge to the destination vertex
            verts[src]->num_outEdges++;         //increment the number of outgoing edges for the source vertex
        }
    }  

    return verts;
}

//Initializes a new vertex of the graph
vertex *create_vertex(long id){
    vertex *new_vert = (vertex *)malloc(sizeof(vertex));
    new_vert->id = id;
    new_vert->pageRank = 1.0;
    new_vert->num_outEdges = 0;
    new_vert->incEdges = NULL;
    
    new_vert->num_incEdges = 0;

    return new_vert;
}

//Initializes a new node for the adjList
adjListNode *create_adj_node(long id){
    adjListNode *new_node = (adjListNode *)malloc(sizeof(adjListNode));
    new_node->id = id;
    new_node->next = NULL;

    return new_node;
}

//Adds a node with the given id to the adjList (list of incoming verticies) for the given vertex 
void add_edge_toList(vertex *vert, long id){
    adjListNode *new_node = create_adj_node(id);
    new_node->next = vert->incEdges;
    vert->incEdges = new_node;
    vert->num_incEdges++;
}

//Prints all the nodes of the graph along with their number of outgoing & incoming edges
void printGraph(struct vertex** v) {
    printf("num_nodes: %ld\n", num_nodes);

    for(long i = 0; i < num_nodes + 1; i++){
        if(v[i] != NULL)
            printf("vertex: %ld, num_outEdges: %d num_incEdges: %d\n", v[i]->id, v[i]->num_outEdges, v[i]->num_incEdges);
    }
}

//Calculates the pagerank for the vertex with the given id
void calculate_pagerank(vertex** verts, long id){
    double inc_sum = 0;

    adjListNode *tmp = verts[id]->incEdges;

    //Calcualtion Formula from Wikipedia : PR(Xi) = (1-d) + d * SUM_OF_INC_EDGES_ON_X( PR(Yi) / #OUT_EDGES_OF_Y )
    while(tmp != NULL){
        inc_sum += verts[tmp->id]->pageRank / verts[tmp->id]->num_outEdges;
        tmp = tmp->next;
    }

    verts[id]->pageRank = (1 - DAMPING_FACTOR) + DAMPING_FACTOR * inc_sum;

}

int export_csv(FILE *output_file, vertex** verts){
    fprintf(output_file, "node,pagerank\n");

    for(int i = 0; i < num_nodes + 1; i++){
        if(verts[i] != NULL)
            fprintf(output_file, "%ld,%f\n", verts[i]->id, verts[i]->pageRank);
    }
    
    return 0;
}