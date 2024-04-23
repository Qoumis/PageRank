
//To store incoming edges of each vertex
typedef struct adjListNode {
    long id;            //the id of the vertex that the list belongs to
    struct adjListNode* next;
}adjListNode;

//To store vretecies of the graph
typedef struct vertex {
    long id;
    double pageRank;
    int num_outEdges;        //number of outgoing edges (links according to wikipedia example)
    adjListNode* incEdges;  //this will hold the list of incoming edges
    int num_incEdges;      //Probably not needed, added mostly for debugging
    double tmp_rank;       //used to store the new pagerank value before it is updated (its not needed since we have 50 ieterations?)
}vertex;


vertex* create_vertex(long id);
adjListNode* create_adj_node(long id);
void add_edge_toList(vertex *v, long id);  //add 
