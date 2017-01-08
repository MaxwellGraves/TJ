#ifndef Q_H_INCLUDED
#define Q_H_INCLUDED

typedef struct Node
{
    int row;
    int col;
    struct Node* next;
} QNode;

typedef struct Queue
{
    QNode* head;
    QNode* tail;
} Q;

void printQ(Q* q);
QNode* makeNode(int r, int c);
Q* makeQ();
void enQ(Q* q, int r, int c);
QNode* deQ(Q* q);


#endif // Q_H_INCLUDED
