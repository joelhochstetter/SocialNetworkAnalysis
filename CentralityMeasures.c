#include <stdio.h>
#include <stdlib.h>
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <assert.h>

typedef struct PredNode *PN;
NodeValues newNV(int noNodes);
double getRatio(ShortestPaths sp, Vertex start, Vertex end, Vertex v);
int getPaths(ShortestPaths sp, Vertex start, Vertex end, Vertex v, int * throughV);

NodeValues newNV(int noNodes) {
   NodeValues nv;
   nv.noNodes = noNodes;
   nv.values = malloc(sizeof(double) * noNodes);
   int i;
   for (i = 0; i < noNodes; i++) 
      nv.values[i] = 0; //initialise array
   return nv;
}

NodeValues outDegreeCentrality(Graph g) {
   AdjList ls;
   NodeValues NV = newNV(numVerticies(g));

   int v = 0;
   for (v = 0; v < NV.noNodes; v++) {
      ls =  outIncident(g, v);
      while (ls != NULL) {
         NV.values[v] += 1.0;
         ls = ls->next;
      }   
   }   
   return NV;
}


NodeValues inDegreeCentrality(Graph g) {
   AdjList ls;
   NodeValues NV = newNV(numVerticies(g));
   int v = 0;
   for (v = 0; v < NV.noNodes; v++) {
      ls =  inIncident(g, v); //this will cause mem leak fix this
      while (ls != NULL) {
         NV.values[v] += 1.0;
         ls = ls->next;
      }   
   }   
   return NV; 
}


NodeValues degreeCentrality(Graph g){ //sum of in and out centralities
   NodeValues out = outDegreeCentrality(g);
   NodeValues in = inDegreeCentrality(g);
   int v;
   for (v = 0; v < numVerticies(g); v++) {
      out.values[v] += in.values[v]; //degreeCentrality is in + out
   }
   return out;
}

NodeValues closenessCentrality(Graph g) {
   AdjList ls;
   int distSum;
   ShortestPaths sp;
   NodeValues nv = newNV(numVerticies(g));
   
   int v = 0;
   int w;
   int nreach; //number of reachable nodes
   
   for (v = 0; v < nv.noNodes; v++) {
      ls =  outIncident(g, v);
      if (ls == NULL) {
         nv.values[v] = 0.0;
      } else {
         distSum = 0;
         nreach = 0;
         sp = dijkstra(g, v);
         for (w = 0; w < numVerticies(g) ; w++) {
            distSum += sp.dist[w];
            if (sp.dist[w] > 0) {
               nreach ++;
            }
         }
         nv.values[v] = (nreach - 1)*(nreach - 1)/(numVerticies(g) - 1)/distSum;          
      } 
   }   
   return nv;
}

//basically need to reimplement djikstra and track if the same vertex is encountered
//once path is stored 
//track as go of shortest paths that contain a given v and can reset when a shorter path found
//
NodeValues betweennessCentrality(Graph g) {
   NodeValues NV = newNV(numVerticies(g));

   //get all possible shortest paths
   int i;
   ShortestPaths spArr[numVerticies(g)];   
   for (i = 0 ; i < numVerticies(g); i++) {
      spArr[i] = dijkstra(g,i);
   }
   
   int s, t, v;
   for (s = 0; s < numVerticies(g); s++) {
      for (t = 0; t < numVerticies(g); t++) {
         for (v = 0; v < numVerticies(g); v++) {
            if ((s == t) || (t == v) || (t == s)) continue;
            NV.values[v] += getRatio(spArr[s], s, t, v);
         }
      }
   }
   for (i = numVerticies(g) - 1 ; i >= 0; i--) {
      freeShortestPaths(spArr[i]);
   }
   return NV;
}

double getRatio(ShortestPaths sp, Vertex start, Vertex end, Vertex v) {
   double result;
   PN curr = sp.pred[end];
   if (curr == NULL) {
      result = 0;
   } else {
      int throughV = 0; //updated using a pointer
      int numPaths = getPaths(sp, start, end, v, &throughV);
      result = throughV/numPaths;
   }
   return result;
}

int getPaths(ShortestPaths sp, Vertex start, Vertex end, Vertex v, int * throughV) {
   int numPaths = 0;
   assert(throughV != NULL);
   if (end == v) {
      *throughV += 1;
   } 
   if (start == end) {
      numPaths = 1;
   }
   
   PN curr; 
   for (curr = sp.pred[end]; curr != NULL; curr = curr->next) {
      numPaths += getPaths(sp, start, sp.pred[curr->v]->v, v, throughV);
   }
   
   return numPaths;
}

NodeValues betweennessCentralityNormalised(Graph g) {
   NodeValues bcN = betweennessCentrality(g);
   int v;
   for (v = 0; v < bcN.noNodes; v++) {
      bcN.values[v] *= 1/(numVerticies(g)-1)/(numVerticies(g)-2);
   }
   return bcN;
}


void  showNodeValues(NodeValues NV) {
   int i;
   for (i = 0; i < NV.noNodes; i++) {
      printf("v: %d val %lf\n", i, NV.values[i]);
   }
}

void  freeNodeValues(NodeValues NV) {
   free(NV.values);  
}
