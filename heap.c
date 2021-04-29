#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

// TODO enable either min or max priority with a boolean in the constructor

Node* createNode(int val) {
    Node* node = malloc(sizeof(Node));
    *node = (Node) { val, NULL, NULL };
    return node;
}

// O(1)
int getTop(PriorityQueue* pq) {
    return pq->topNode->val;
}

// O(logn)
int removeTop(PriorityQueue* pq) {
    int top = pq->topNode->val;
    // store the top value for return
    // then get the last item in the tree and put it at top
    // when getting the last item, recurse down, stop when reached its parent
    // move last item to head
    // then free and remove the last item from parent
    // then bubble it downwards if not smallest, swapping each time it isn't
    handleRemoval(pq);
    pq->nodeCount--;

    return top;
}

void handleRemoval(PriorityQueue* pq) {
    int bits[INT_BIT_COUNT] = { 0 };
    // have not inserted so don't need to add 1
    int idx = allocBitArr(bits, pq->nodeCount);
    // pass back the last item
    int last = removeLastNode(&pq->topNode, bits, idx - 1);
    // if was null then removed was the last node, and thus head
    if (pq->topNode != NULL) {
        pq->topNode->val = last;
        // throw away msb
        reshuffleDown(pq->topNode);
    }
}

//* returns item of last node
int removeLastNode(Node** currentAddr, int* bitArr, int i) {
    Node** nextAddr = bitArr[i] ? &(*currentAddr)->rc : &(*currentAddr)->lc;
    if (*nextAddr == NULL) {
        // current node was head
        int val = (*currentAddr)->val;
        free(*currentAddr);
        *currentAddr = NULL;
        return val;
    }

    if (i <= 0) {
        int val = (*nextAddr)->val;
        free(*nextAddr);
        *nextAddr = NULL;
        return val;
    } else {
        return removeLastNode(nextAddr, bitArr, i - 1);
    }
}

// bubble the top item down
void reshuffleDown(Node* current) {
    Node *lc = current->lc, *rc = current->rc;

    // if lc is null, return as no children
    // if current smaller than both lc and rc, return

    if (lc == NULL) {
        return;
    }

    if (rc == NULL) {
        // check only lc
        bool currentLarger = current->val > lc->val;
        if (currentLarger) {
            swapNodeVals(current, lc);
            reshuffleDown(lc);
        }
        return;
    }

    // check both lc and rc
    bool currentGtLc = current->val > lc->val;
    bool currentGtRc = current->val > rc->val;
    bool lcGtRc = lc->val > rc->val;
    if (lcGtRc) {
        // rc smaller
        if (currentGtRc) {
            swapNodeVals(current, rc);
            reshuffleDown(rc);
        }
    } else {
        if (currentGtLc) {
            swapNodeVals(current, lc);
            reshuffleDown(lc);
        }
    }
}

// O(logn)
// always insert at end and bubble up if parent larger
void insert(PriorityQueue* pq, Node* node) {
    printf("Inserting %d\n", node->val);

    Node** nextSpot = findNextSpot(pq);
    *nextSpot = node;
    handleReshuffle(pq);
    pq->nodeCount++;
}

void handleReshuffle(PriorityQueue* pq) {
    int bits[INT_BIT_COUNT] = { 0 };
    int idx = allocBitArr(bits, pq->nodeCount + 1);
    // throw away msb
    reshuffleUp(pq->topNode, bits, idx - 1);
}

void reshuffleUp(Node* current, int* bitArr, int i) {

    Node* next = bitArr[i] ? current->rc : current->lc;
    if (next == NULL) {
        // if queue only has one element
        return;
    }
    // lsb is at start of array
    // stop on last direction, as next node will be last
    if (i <= 0) {
        bool shouldSwap = current->val > next->val;
        if (shouldSwap) {
            swapNodeVals(current, next);
        }
        return;
    } else {
        reshuffleUp(next, bitArr, i - 1);
        bool shouldSwap = current->val > next->val;
        if (shouldSwap) {
            swapNodeVals(current, next);
        }
    }
}

void swapNodeVals(Node* n1, Node* n2) {
    int val1 = n1->val;
    n1->val = n2->val;
    n2->val = val1;
}

PriorityQueue* createPriorityQueue() {
    PriorityQueue* pq = malloc(sizeof(PriorityQueue));
    *pq = (PriorityQueue) { NULL, 0 };
    return pq;
}

// double pointer because want the address in the parent where the node pointer should go
Node** findNextSpot(PriorityQueue* pq) {
    int bits[INT_BIT_COUNT] = { 0 };
    // plus one to add the new node to count, minus one to index as throw away msb
    int idx = allocBitArr(bits, pq->nodeCount + 1) - 1;
    Node** current = &pq->topNode;
    for (; idx >= 0; idx--) {
        // printf("%s\n", bits[idx] ? "right" : "left");
        if (bits[idx]) {
            current = &(*current)->rc;
        } else {
            current = &(*current)->lc;
        }
    }
    return current;
}

//* Fills an array with the bits of an integer in reverse, returning the index of the last bit
int allocBitArr(int* bits, int num) {
    int idx = 0;
    while (num) {
        int bit = num % 2;
        num /= 2;
        bits[idx++] = bit;
    }
    // minus as move back to last index
    return idx - 1;
}

//* Prints in order
void printHeap(PriorityQueue* pq) {
    int originalCount = pq->nodeCount;
    if (originalCount == 0) {
        printf("No items in heap\n");
        return;
    }
    printf("\n\nHEAP\n");
    int level = 1, current = 0;
    for (int i = 0; i < originalCount; i++) {
        pq->nodeCount = i;
        Node* node = *findNextSpot(pq);
        printf("%d ", node->val);
        if (++current == level) {
            printf("\n-------------\n");
            level *= 2;
            current = 0;
        }
    }
    printf("\n");
    pq->nodeCount = originalCount;
}

int main() {

    int SIZE = 20;

    PriorityQueue* pq = createPriorityQueue();

    for (int i = 0; i < SIZE; i++) {
        // printf("%d\n", i);
        Node* node = createNode(SIZE - i);
        insert(pq, node);
    }
    printHeap(pq);

    for (int i = 0; i < SIZE; i++) {
        int removed = removeTop(pq);
        printf("Index: %d, top val: %d\n", i, removed);
    }
    printf("\n\n");

    int vals[] = { 9, 6, 4, 2, 6, 8, 1, 3, 6 };

    for (int i = 0; i < len(vals); i++) {
        Node* node = createNode(vals[i]);
        insert(pq, node);
    }

    printHeap(pq);

    for (int i = 0; i < len(vals); i++) {
        int removed = removeTop(pq);
        printf("Top val: %d\n", i, removed);
    }
}
