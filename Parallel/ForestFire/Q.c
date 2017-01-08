#include "Q.h"
#include <stdio.h>
void printQ(Q* q)
{
    QNode* current = q->head;
    while ( current != NULL)
    {
        printf("%d,%d|", current->row, current->col);
        current = current->next;
    }
    printf("\n");
}

QNode* makeNode(int r, int c)
{
    QNode* temp = malloc(sizeof(QNode));
    temp->row = r;
    temp->col = c;
    temp->next = NULL;
    return temp;
}

Q* makeQ()
{
    Q* q = malloc(sizeof(Q));
    q->head = NULL;
    q->tail = NULL;
    return q;
}

void enQ(Q* q, int r, int c)
{
    QNode* current = makeNode(r, c);
    if( q->head == NULL)
    {
        q->head = current;
    }
    else
    {
        q->tail->next = current;
    }
    q->tail = current;
}

QNode* deQ(Q* q)
{

    QNode* current = q->head;
    if(current!= NULL)
    {
        q->head = current->next;
        current->next = NULL;
    }
    return current;
}
