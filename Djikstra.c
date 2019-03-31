#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "PQ.h"
#include "Dijkstra.h"
#include <assert.h>
#include <limits.h>

typedef struct PredNode *PN;
void deletePreds(PN pn);
PN appendPred(PN pn, int v);

ShortestPaths dijkstra(Graph g, Vertex v) 
{

   ShortestPaths sp; //initialise SP struct
   sp.src = v; //set source node to input
   //allocate space for arrays for pred ptrs and distances
   sp.pred = malloc(numVerticies(g) * sizeof(PN));
   assert(sp.pred != NULL);
   sp.dist = malloc(numVerticies(g) * sizeof(int));
   assert(sp.dist != NULL);
   sp.noNodes = numVerticies(g); 
   
   PQ vertices = newPQ(g); //create priority queue

   ItemPQ vIt; //create generic pq item for use of adding to pq
   Vertex s; //later stores minimum distance vertex
   AdjList t; //stores vertex with edge coming from s in AdjList form

   int i; //loop increment   
   for (i = 0; i < numVerticies(g) ; i++) {
   //initialise distances
      if (i == v) {
         sp.dist[v] = 0; 
         vIt.value = 0;     
      } else {
         sp.dist[i] = INT_MAX;  //INT_MAX is used for infinity
         vIt.value = INT_MAX;   //INT_MAX is used for infinity
      }
       //initialise elements in array of pred node pointers
      sp.pred[i] = NULL;
      vIt.key = i; 
      addPQ(vertices, vIt); //adds vertex with dist[v] to PQ
   }
   
   
   while (!PQEmpty(vertices)) {
      s = dequeuePQ(vertices).key;
      for (t = outIncident(g,s); t != NULL; t = t->next) {
         if (sp.dist[s] == INT_MAX) continue;
         if ((sp.dist[s] + t->weight < sp.dist[t->w])) {
            sp.dist[t->w] = sp.dist[s] + t->weight;
            
            deletePreds(sp.pred[t->w]);                       
            sp.pred[t->w] = appendPred(sp.pred[t->w], s);  
              
            vIt.key = t->w;
            vIt.value = sp.dist[t->w];
            updatePQ(vertices, vIt);           
      
         } else if ((sp.dist[s] + t->weight == sp.dist[t->w])) {
            sp.pred[t->w] = appendPred(sp.pred[t->w], s);         
         }
      }
   }
   return sp;
}

void deletePreds(PN pn) {
   PN prev, curr;
   prev = pn;
   while (prev != NULL) {
      curr = prev->next;
      free(prev);
      prev = curr;
   }
}

PN appendPred(PN pn, int v) { //
   PN head;
   head = pn;
   PN new = malloc(sizeof(PredNode));
   new->v = v;
   new->next = NULL;
   if (pn == NULL) {
      head = new; 
   } else {
      while (pn->next != NULL) {
         pn = pn->next;
      }
      if (pn == NULL ) {
         head = new;  
      } else {
         pn->next = new;
      }
   }
   return head;
}

void  showShortestPaths(ShortestPaths p) {

}

void  freeShortestPaths(ShortestPaths p ) {

}

void updatePreds(ShortestPaths *sp, int nV) {
   PredNode * cpy;
   int i;
   for (i = 0; i < nV ; i++) {
      if (sp->dist[i] == 0) continue; //if we are just at the starting node
      if (sp->dist[i] == INT_MAX) continue; //if node is unreachable from start
      while (sp->dist[sp->pred[i]->v] != 0) {
         cpy = malloc(sizeof(PredNode));
         cpy->v = sp->pred[sp->pred[i]->v]->v;
         cpy->next = sp->pred[i];
         sp->pred[i] = cpy;      
      }
   }
}


void  showShortestPaths(ShortestPaths p) {

}

void  freeShortestPaths(ShortestPaths p ) {

}
