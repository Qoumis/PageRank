#include <stdlib.h>
#include "graph.h"

#ifndef CONSTS_H

#define DAMPING_FACTOR 0.85
#define ITERATIONS 50

#endif

void calculate_pagerank(vertex** verts, long id){
    double inc_sum = 0;

    adjListNode *tmp = verts[id]->incEdges;
    while(tmp != NULL){
        inc_sum += verts[tmp->id]->pageRank / verts[tmp->id]->num_outEdges;
        tmp = tmp->next;
    }

    verts[id]->pageRank = (1 - DAMPING_FACTOR) + DAMPING_FACTOR * inc_sum;


}
