// Graph ADT
//Make local functions static if needed 

#include "Graph.h"
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct GraphRep {
   AdjList *edges;  // array of lists
   //edges are maintained in order
   int   nV;     // #vertices
   int   nE;     // #edges
} GraphRep;

int validV(Vertex v, Graph g);
void showList(AdjList l);
AdjList duplicateNode(AdjList l);


int validV(Vertex v, Graph g) {
   if (v >= 0 && v < g->nV) 
      return 1;
   else
      return 0;
}

void showList(AdjList l) {
   AdjList curr;
   for (curr = l ; curr != NULL; curr = curr->next)
      printf("[%d]->", l->w);
   printf("[X]\n");
}

AdjList duplicateNode(AdjList l) {
   AdjList new = malloc(sizeof(adjListNode));
   new->w = l->w;
   new->weight = l->weight;
   new->next = NULL;
   return new;
}

Graph newGraph(int nV) {
   assert(nV >= 0);
   int i;

   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = nV;
   g->nE = 0;

   // allocate memory for array of lists
   g->edges = malloc(nV * sizeof(adjListNode));
   assert(g->edges != NULL);
   for (i = 0; i < nV; i++)
      g->edges[i] = NULL;

   return g;
}


void insertEdge(Graph g, Vertex src, Vertex dest, int weight) 
{
//inserts edge in order
   assert(g != NULL && validV(src) && validV(dest) && weight >= 0);

   AdjList curr = g->edges[src]; //current node
   AdjList prev = NULL;
   while (curr != NULL && curr->w < dest) { 
      prev = curr;
      curr = curr->next;     
   }
   if (curr->w == dest) {
      printf("Error: edge already in graph\n");
      assert(0);
   }
   
   //creates edge node
   AdjList newNode = malloc(sizeof(adjListNode));
   assert(newNode != NULL);
   newNode->w = dest;
   newNode->weight = weight;
   
   if (prev != NULL) {
      prev->next = newNode;
   }
   newNode->next = curr;
   
}

void  removeEdge(Graph g, Vertex src, Vertex dest) 
{
   assert(g != NULL && validV(src) && validV(dest));

   AdjList curr = g->edges[src]; //current node
   AdjList prev = NULL;
   while (curr != NULL && curr->w < dest) { 
      prev = curr;
      curr = curr->next;     
   }
   if (curr != NULL && curr->w == dest) { //alternatively there is no edge to remove
      prev->next = curr->next;
      free(curr);
   } 
}


bool adjacent(Graph g, Vertex src, Vertex dest) {
   assert(g != NULL && validV(src) && validV(dest));

   AdjList curr = g->edges[src]; //current node
   AdjList prev = NULL;
   
   while (curr != NULL && curr->w < dest) { 
      prev = curr;
      curr = curr->next;     
   }
   if (curr != NULL && curr->w == dest) { //is adjacent
      return TRUE;
   } else {
      return FALSE;
   }
}

void showGraph(Graph g) {
    assert(g != NULL);
    int i;

    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (i = 0; i < g->nV; i++) {
       printf("%d - ", i);
       showList(g->edges[i]);
    }
}

void freeGraph(Graph g) {
   assert(g != NULL);
   int i;

   for (i = 0; i < g->nV; i++)
      freeLL(g->edges[i]);

   free(g);
}

int  numVerticies(Graph g) {
   assert(g != NULL);
   return g->nV;
}

/*
 * Returns a list of adjacent vertices
 * on outgoing edges from a given vertex.
**/
AdjList outIncident(Graph g, Vertex v) {
   assert(g != NULL && validV(v)); 
   return g->edges[v];
}
/*

 * Returns a list of adjacent vertices
 * on incoming edges from a given vertex.
**/
AdjList inIncident(Graph g, Vertex v) {
   AdjList inList = malloc(sizeof(adjListNode));
   assert(inList != NULL);
   AdjList lsEnd = NULL; //stores pointer to end of list for use in appending
   int i;
   for (i = 0 ; i < g->nV ; i++) {
      while (curr != NULL && curr->w < v) { 
         prev = curr;
         curr = curr->next;    
      }
      if (curr->w == v) {
            if (lsEnd == NULL) { //first element of the list
               inList = duplicateNode(curr);
               lsEnd = inList;
            } else { //any other element
               lsEnd->next = duplicateNode(curr);
               lsEnd = lsEnd->next;
            }
      }           
   }
   return inList;
}
