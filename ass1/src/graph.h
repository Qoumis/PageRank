
//To store vretecies
typedef struct node {
    long vertex;
    double pagerank;
    struct node* next;
}node;

//To store the actual graph
typedef struct Graph {
    long numVertices;
    struct node** adjLists;
}Graph;

node* newNode(long v);
Graph* createGraph(long numVertices);
void addEdge(struct Graph* g, long src, long dest);