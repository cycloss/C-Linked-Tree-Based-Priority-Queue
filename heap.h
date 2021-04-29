#include <stdbool.h>

#define len(x) sizeof(x) / sizeof(x[0])

const int INT_BIT_COUNT = sizeof(int) * 4;

typedef struct Node {
    int val;
    struct Node* lc;
    struct Node* rc;
} Node;

typedef struct {
    Node* topNode;
    //* DO NOT MODIFY
    int nodeCount;
} PriorityQueue;

Node* createNode(int val);
int getTop(PriorityQueue* pq);
int removeTop(PriorityQueue* pq);
void handleRemoval(PriorityQueue* pq);
int removeLastNode(Node** currentAddr, int* bitArr, int i);
void reshuffleDown(Node* current);
void insert(PriorityQueue* pq, Node* node);
void handleReshuffle(PriorityQueue* pq);
void reshuffleUp(Node* current, int* bitArr, int i);
void swapNodeVals(Node* n1, Node* n2);
PriorityQueue* createPriorityQueue();
Node** findNextSpot(PriorityQueue* pq);
int allocBitArr(int* bits, int num);
void printHeap(PriorityQueue* pq);
int main();