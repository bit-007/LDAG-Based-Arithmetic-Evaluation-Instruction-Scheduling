#include <stdio.h>
#include <stdlib.h>

// LDAG Node Structure
typedef struct LDAGNode {
    char operation; // Operation symbol: '+', '*', '/'
    char operand;   // Variable symbol: 'A', 'B', 'C', 'D'
    int level;      // Level of the node in the LDAG
    int qhPosition; // QH position relative to the node
    struct LDAGNode* left;
    struct LDAGNode* right;
} LDAGNode;

// Queue Node Structure
typedef struct QueueNode {
    LDAGNode* node;
    struct QueueNode* next;
} QueueNode;

// Queue Structure
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// Function to create a new LDAG node
LDAGNode* createNode(char operation, char operand, int level) {
    LDAGNode* newNode = (LDAGNode*)malloc(sizeof(LDAGNode));
    newNode->operation = operation;
    newNode->operand = operand;
    newNode->level = level;
    newNode->qhPosition = 0;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to create a Queue
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

// Function to free queue memory
void freeQueue(Queue* queue) {
    QueueNode* temp;
    while (queue->front != NULL) {
        temp = queue->front;
        queue->front = queue->front->next;
        free(temp);
    }
    free(queue);
}

// Function to print queue content
void printQueueContent(Queue* queue) {
    QueueNode* temp = queue->front;
    while (temp != NULL) {
        if (temp->node->operation == '\0')
            printf("%c\t", temp->node->operand);
        else
            printf("%c\t", temp->node->operation);
        temp = temp->next;
    }
    printf("\n");
}

// Function to enqueue a node in queue
void enqueue(Queue* queue, LDAGNode* node) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->node = node;
    newNode->next = NULL;
    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Function to enqueue nodes in in-order traversal
void enqueueInOrder(Queue* queue, LDAGNode* node) {
    if (node != NULL) {
        enqueueInOrder(queue, node->left);
        enqueue(queue, node);
        enqueueInOrder(queue, node->right);
    }
}

// Function to perform symbolic execution (calculate QH positions)
void symbolicExecution(LDAGNode* root, int qhPosition) {
    if (root != NULL) {
        root->qhPosition = qhPosition;
        symbolicExecution(root->left, qhPosition);
        symbolicExecution(root->right, qhPosition + 1);
    }
}

// Function to count nodes in level-order traversal
int countNodesInLevelOrder(LDAGNode* root) {
    if (root == NULL) {
        return 0;
    }
    int count = 0;
    int front = 0, rear = 0;
    LDAGNode* queue[100]; // Assuming max 100 nodes
    queue[rear++] = root;

    while (front < rear) {
        LDAGNode* current = queue[front++];
        count++;
        if (current->left) queue[rear++] = current->left;
        if (current->right) queue[rear++] = current->right;
    }
    return count;
}

// Function to calculate the distance between two nodes using BFS
int calculateDistance(LDAGNode* u, LDAGNode* v) {
    if (u == NULL || v == NULL) return -1;

    // Queue for BFS
    LDAGNode* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = u;

    // Hash table for node distances
    int distanceArray[100];
    for (int i = 0; i < 100; i++) distanceArray[i] = -1;
    
    distanceArray[u->qhPosition] = 0;

    while (front < rear) {
        LDAGNode* current = queue[front++];
        int currDist = distanceArray[current->qhPosition];

        if (current->left && distanceArray[current->left->qhPosition] == -1) {
            queue[rear++] = current->left;
            distanceArray[current->left->qhPosition] = currDist + 1;
        }
        if (current->right && distanceArray[current->right->qhPosition] == -1) {
            queue[rear++] = current->right;
            distanceArray[current->right->qhPosition] = currDist + 1;
        }
    }
    return distanceArray[v->qhPosition];
}

// Function to print node levels
void printLDAGNodeLvl(LDAGNode* node) {
    if (node != NULL) {
        printLDAGNodeLvl(node->left);
        printf("Operation: %c, Variable: %c, Level: %d\n", node->operation, node->operand, node->level);
        printLDAGNodeLvl(node->right);
    }
}

// Function to print node QH positions
void printLDAGNodeQH(LDAGNode* node) {
    if (node != NULL) {
        printLDAGNodeQH(node->left);
        printf("Operation: %c, Variable: %c, Level: %d, QH Position: %d\n", node->operation, node->operand, node->level, node->qhPosition);
        printLDAGNodeQH(node->right);
    }
}

// Main function
int main() {
    LDAGNode* root = createNode('/', '\0', 1);
    root->left = createNode('+', '\0', 2);
    root->left->left = createNode('A', 'A', 3);
    root->left->right = createNode('*', '\0', 3);
    root->left->right->left = createNode('B', 'B', 4);
    root->left->right->right = createNode('C', 'C', 4);
    root->right = createNode('D', 'D', 2);

    printf("Calculating levels:\n");
    printLDAGNodeLvl(root);

    int nodeCount = countNodesInLevelOrder(root);
    printf("\nNumber of nodes in LDAG: %d\n\n", nodeCount);

    printf("Symbolic execution - calculating offsets:\n");
    symbolicExecution(root, 0);
    printLDAGNodeQH(root);

    printf("\nCalculating distance between nodes A and B:\n");
    int distance = abs(calculateDistance(root->left->left, root->left->right->left));
    printf("Distance between nodes A and B: %d\n\n", distance);

    printf("\nInstruction Scheduling:\n");
    Queue* schedulingQueue = createQueue();
    enqueueInOrder(schedulingQueue, root);
    printQueueContent(schedulingQueue);

    freeQueue(schedulingQueue);
    return 0;
}