#include "LanceWilliamsHAC.h"
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

typedef struct DNode *Dendrogram;
typedef struct DNode {
   int  vertex;
   Dendrogram left, right;
} DNode;

Dendrogram createDNode(int vertex);
Dendrogram freeDendrogram(Dendrogram d);
Dendrogram mergeClusters(Dendrogram ci, Dendrogram cj);
int genKey(int i, int j, int numV); 
int deKeyMin(int key, int numV);
int deKeyMax(int key, int numV);
ItemPQ createPQItem(int key, int weight);
//each pairing is assigned a unique key

Dendrogram createDNode(int vertex) {
   Dendrogram d = malloc(sizeof(DNode));
   d->vertex = vertex;
   d->left = NULL;
   d->right = NULL;
   return d;
}

Dendrogram mergeClusters(Dendrogram ci, Dendrogram cj) {
   Dendrogram root = createDNode(-1);
   root->left = ci;
   root->right = cj;
   return root;
}

int genKey(int i, int j, int numV) {
   int min, max;
   if (i < j) {
      min = i;
      max = j;
   } else {
      min = j;
      max = i;
   }
   return min*numV + max;
}

int deKeyMin(int key, int numV) {
   return key / numV;
}

int deKeyMax(int key, int numV) {
   return key % numV;
}

ItemPQ createPQItem(int key, int weight) {
   ItemPQ it;
   it.key = key;
   it.weight = weight;
   return it;
}


//to distinguish between methods I will use:
// method 1: Single link dist[v][w] = 1/wt (disconnected infinite)
// method 2: Complete link dist[v][w] = wt (disconnected 0)
// This simplifies the problem to allow distance 
// between nodes to be minimised
//to calculate minimum distance priority queue is utilised

Dendrogram LanceWilliamsHAC(Graph g, int method) {
   assert(method == 1 || method == 2);
   int nv = numVerticies(g);
   double dist[numVerticies(g)][numVerticies[g]] = {INT_MAX}; 
   int v, w;
   AdjList edge;
   Dendrogram clus[numVerticies(g)]; //array of clusters
   Dendrogram d = NULL;
   PQ pq = newPQ();
   ItemPQ min;
   
   
   //initialise array   
   for (v = 0; v < nv; v++) {
      clus[v] = createDNode(v);
      for (w = 0; w < nv; w++) {
         if (w > v) continue;
         if (method == 1)
            dist[v][w] = INT_MAX; //set all to infinity initially
         else
            dist[v][w] = INT_MAX;
      }
      //then set to infinity
      for (edge = g->edges[v]; edge != NULL ; edge = edges->next) {
         w = edge->w;
         if (method == 1) {
            dist[v][w] = 1.0/edge->weight;         
         } else {
            dist[v][w] = edge->weight; 
         }
         if (w > v) {
            if (dist[v][w] > dist[w][v]) {
               dist[v][w] = dist[w][v];
            }
            addPQ(pq, createPQItem(genKey(v, w, nv), dist[v][w]));
         }
      }
   }
   for (v = 0; v < nv; v++) { 
   //Take dequeued value if both not in cluster
      min = dequeuePQ(pq);
      while (clus(deKeyMin(min.key)) == NULL || clus(deKeyMax(min.key)) == NULL) {
         min = dequeuePQ(pq);
      }
      
      d = mergeClusters(clus[deKeyMin(min.key)], clus[deKeyMax(min.key)]);
      clus[deKeyMin(min.key)] = d;
      clus[deKeyMax(min.key)]) = NULL; //denotes it is in a cluster
         //update distances
      for (w = 0; w < nv ; w++) {
         if ((deKeyMax(min.key) <= w) || (clus(w) == NULL)) continue;
         if (dist[deKeyMax(min.key)][w] <= dist[deKeyMin(min.key)][w]) {
            updatePQ(pq, createPQItem(genKey(w, deKeyMax(min.key)][w]), 
            dist[deKeyMax(min.key)][w]));//split to adhere to 72 char / line
         }
      }
   }
   return d;
}
   

void freeDendrogram(Dendrogram d) {
   if (d->left == NULL && d->right == NULL) {
      free(d);
   } else {
      freeDendrogram(d->left);
      freeDendrogram(d->right);
      free(d);
   }
}
