// Graph ADT interface for Ass2 (COMP2521)
#include <stdbool.h>
#ifndef GRAPH
	#define GRAPH
	#include "Graph.h"
#endif

typedef struct NodeValues {
   int noNodes;
   double* values;
} NodeValues;



NodeValues outDegreeCentrality(Graph);
NodeValues inDegreeCentrality(Graph);
NodeValues degreeCentrality(Graph); // for undirected graph

NodeValues closenessCentrality(Graph);

NodeValues betweennessCentrality(Graph);
NodeValues betweennessCentralityNormalised(Graph);


void  showNodeValues(NodeValues);
void  freeNodeValues(NodeValues);
