#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

FILE *input_file, *output_file;

Graph * parse_file(FILE *input);
void printGraph(struct Graph* graph);

int main(int argc, char **argv) {

    if(argc != 3){
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

    //parse file & construct graph
    Graph * graph = parse_file(input_file);

    printGraph(graph);
    printf("komplit\n");

    fclose(input_file);
    fclose(output_file);

    return 0;
}