#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "graph.h"

#define ITERATIONS 50
#define TRUE 1
#define FALSE 0

long num_nodes = 0;

vertex** parse_file(FILE *input);
void printGraph(struct vertex** v);
void export_csv(FILE *output_file, vertex** verts);
void export_time(struct timeval start, struct timeval end);
void calculate_pagerank(vertex** verts, long id);

int main(int argc, char **argv) {

    FILE *input_file, *output_file;
    int num_threads = 1;
    int graph_info = FALSE;

    struct timeval start, end;

    if(argc < 3){
        fprintf(stderr, "Invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }
    else if(!(input_file = fopen(argv[1], "r"))){
        fprintf(stderr, "Could not open input file: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    else if(!(output_file = fopen(argv[2], "w"))){
        fprintf(stderr, "Could not open output file: %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    for(int i = 3; i < argc; i++){
        if(strcmp(argv[i], "-t") == 0){
            if(atoi(argv[i + 1]) > 4)
                num_threads = 4;
            else if (atoi(argv[i + 1]) > 1)
                num_threads = atoi(argv[i + 1]);
            i++;
        }
        else if(strcmp(argv[i], "-ginfo") == 0){
            graph_info = TRUE;
        }
        else
            fprintf(stderr, "Argument: %s is invalid and was ignored.\n", argv[i]);

    }

    //parse file & construct graph
    vertex ** vertices = parse_file(input_file);

    gettimeofday(&start, NULL);

    for(int i = 0; i < ITERATIONS; i++){
        for(long i = 0; i < num_nodes + 1; i++){
            if(vertices[i] != NULL)
                calculate_pagerank(vertices, i);
        }
    }

    gettimeofday(&end, NULL);

    //export results
    if(graph_info)
        printGraph(vertices);

    export_csv(output_file, vertices);
    export_time(start, end);

    //free allocated memory
    for(long i = 0; i < num_nodes + 1; i++){
        if(vertices[i] != NULL){
            while(vertices[i]->incEdges != NULL){
                adjListNode *temp = vertices[i]->incEdges;
                vertices[i]->incEdges = vertices[i]->incEdges->next;
                free(temp);
            }
            free(vertices[i]);
        }
    }
    fclose(input_file);
    fclose(output_file);

    return 0;
}