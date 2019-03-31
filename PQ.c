#include <stdio.h>
#include <stdlib.h>
#include "PQ.h"
#include <assert.h>

typedef struct ItemPQ *Item;

//PQ represntation
typedef struct PQRep {
   Item *items;  // array of pointers to Items
   int  nitems;  // #items in array
   int  nslots;  // #elements in array
} PQRep;

void fixUp(Item *a, int i);
void swap(Item *a, int i, int j);
void fixDown(Item *a, int i, int N);

/* Creates new priority queue, that can store items of type ItemPQ.
*/
PQ newPQ() {
   PQ new = malloc(sizeof(PQRep));
   assert(new != NULL);
   new->items = malloc(sizeof(Item)*10);
   assert(new->items != NULL);
   new->nitems = 0;
   new->nslots = 10;
   return new;
}

/* Adds item (ItemPQ) to the priority queue.
   If an item with 'key' exists, it's 'value' is updated.
*/
void  addPQ(PQ pq, ItemPQ it) {
   assert(pq != NULL);
   if (pq->nitems >= pq->nslots) {
      pq = realloc(pq, 2*pq->nslots);
      pq->nslots *= 2;
   }
   assert(pq->nitems < pq->nslots);
   pq->nitems++;
   Item copy = malloc(sizeof(ItemPQ));
   copy->value = it.value;
   copy->key = it.key;
   pq->items[pq->nitems] = copy;
   fixUp(pq->items, pq->nitems);
}

// force value at a[i] into correct position
void fixUp(Item *a, int n)
{
   while (n > 1 && (a[n/2]->value > a[n]->value)) {
      swap(a, n, n/2);
      n = n/2;  // integer division
   }
}

void swap(Item *a, int i, int j)
{
   Item tmp = a[i];
   a[i] = a[j];
   a[j] = tmp;
}



/* Removes and returns the item (ItemPQ) with smallest 'value'.
   For items with equal 'value', observes FIFO.
   Returns null if this queue is empty.
*/
ItemPQ  dequeuePQ(PQ pq) {
   Item top = pq->items[1];
   // overwrite first by last
   pq->items[1] = pq->items[pq->nitems];
   pq->nitems--;
   // move new root to correct position
   fixDown(pq->items, 1, pq->nitems);

   return *top;
}

// force value at a[i] into correct position
// note that N gives max index *and* # items
void fixDown(Item *a, int i, int N)
{
   int j;
   while (2*i <= N) {
      // compute address of left child
      j = 2*i;
      // choose larger of two children
      if (j < N && (a[j]->value > a[j+1]->value)) j++;
      if (!(a[i]->value > a[j]->value)) break;
      swap(a, i, j);
      // move one level down the heap
      i = j;
   }
}

/* Updates item with a given 'key' value, by updating that item's value to the given 'value'.
   If item with 'key' does not exist in the queue, no action is taken
*/
void  updatePQ(PQ pq, ItemPQ it) {//this is O(n)
   int i;
   for (i = 1; i <= pq->nitems; i++) {
      if (pq->items[i]->key == it.key) {
         pq->items[i]->value = it.value;
         break;
      }
   } 
}

int PQEmpty(PQ pq) {
   return (pq->nitems == 0);
}

void  showPQ(PQ pq) {
   int i;
   for (i = 1; i <= pq->nitems; i++) {
      printf("%d (%d) ; ", pq->items[i]->key, pq->items[i]->value);
   }
   printf("\n");
}


void  freePQ(PQ pq) {
   int i;
   for (i = pq->nslots - 1; i >= 0; i--) {
      if (pq->items[i] != NULL) {
         free(pq->items[i]);
      }
   }
   free(pq);
}
