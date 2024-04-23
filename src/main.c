#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "graph.h"
#include <pthread.h>

#define DAMPING_FACTOR 0.85
#define ITERATIONS 50
#define TRUE 1
#define FALSE 0

long num_nodes = 0;

vertex** parse_file(FILE *input);
void printGraph(struct vertex** v);
void export_csv(FILE *output_file, vertex** verts);
void export_time(struct timeval start, struct timeval end);
void *calculate_pagerank(void * id);

typedef struct thrd_data{
    long from_id;
    long to_id;
} thrd_data;

thrd_data* init_thread_data(long from_id, long to_id);

vertex ** vertices;
pthread_barrier_t barrier;

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

    /** --- parse file & construct graph --- **/

    vertices = parse_file(input_file);

    /** --- Thread Work --- */

    gettimeofday(&start, NULL); //Start timer 

    pthread_t threads[num_threads];
    pthread_barrier_init(&barrier, NULL, num_threads);

    long node_split = (num_nodes / num_threads); //Number of nodes each thread will be responsible for
    for(int i = 0; i < num_threads; i++){

        thrd_data *data;
        if(i == (num_threads - 1))
            data = init_thread_data(i * node_split , num_nodes + 1);
        else
            data = init_thread_data(i * node_split, (i + 1) * node_split);

       //printf("Thread %d: from_id: %ld to_id: %ld\n", i, data->from_id, data->to_id);
  
       pthread_create(&threads[i], NULL, calculate_pagerank, (void *)data);
    }

    //wait for all threads to finish
    for(int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);   //Stop timer

    /** --- export results --- **/

    if(graph_info)
        printGraph(vertices);

    export_csv(output_file, vertices);
    export_time(start, end);

    /** --- free allocated memory --- **/

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

void *calculate_pagerank(void * arg){
    thrd_data *data = (thrd_data *)arg;

    for(int i = 0; i < ITERATIONS; i++){
        pthread_barrier_wait(&barrier);

        for(long id = data->from_id; id < data->to_id; id++){
            if(vertices[id] != NULL){
                double inc_sum = 0;
                
                adjListNode *tmp = vertices[id]->incEdges;
                //Calcualtion Formula from Wikipedia : PR(Xi) = (1-d) + d * SUM_OF_INC_EDGES_ON_X( PR(Yi) / #OUT_EDGES_OF_Y )
                while(tmp != NULL){
                    inc_sum += vertices[tmp->id]->pageRank / vertices[tmp->id]->num_outEdges;
                    tmp = tmp->next;
                }
                vertices[id]->tmp_rank = (1 - DAMPING_FACTOR) + DAMPING_FACTOR * inc_sum;
            }
        }
        pthread_barrier_wait(&barrier);
        //im not sure that the temp is actually nedeed in 50 iterations, i think that it is irrelevant eventually. 
        //Im keeping it this way since it doesnt affect execution time alot (and the results are the same either way)
        for(long id = data->from_id; id < data->to_id; id++){
            if(vertices[id] != NULL)
                vertices[id]->pageRank = vertices[id]->tmp_rank;
        }

    }

    return NULL;
}

thrd_data* init_thread_data(long from_id, long to_id){
    thrd_data *data = (thrd_data *)malloc(sizeof(thrd_data)); 
    data->from_id = from_id;
    data->to_id = to_id;

    return data;
}