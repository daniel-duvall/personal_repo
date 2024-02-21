#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct node
{
    int value;
    struct node *next;
};

void insert1(struct node *pArgHead, int value)
{
    struct node *newNode = malloc(sizeof(struct node));
    newNode->value = value;
    newNode->next = NULL;

    newNode->next = pArgHead;
    pArgHead = newNode;
}

void insert2(struct node **ppArgHead, int value)
{
    struct node *newNode = malloc(sizeof(struct node));
    newNode->value = value;
    newNode->next = NULL;

    newNode->next = *ppArgHead;
    *ppArgHead = newNode;
}

struct node *insert3(struct node *pArgHead, int value)
{
    struct node *newNode = malloc(sizeof(struct node));
    newNode->value = value;
    newNode->next = NULL;
    
    newNode->next = pArgHead;
    pArgHead = newNode;

    return newNode;
}

void display(struct node *pHead)
{
    struct node *tmp = pHead;
    printf("-------------------\n");
    while(tmp!=NULL)
    {
        printf("%d ", tmp->value);
        tmp = tmp->next;
    }
    printf("\n-------------------\n");
}

int main(void)
{
    int arr[3] = {3, 2, 1};
    struct node *head = NULL;

    printf("insert1:\n");
    for(int i=0; i<3; i++)
    {
        insert1(head, arr[i]);
        display(head);
    }

    printf("insert2:\n");
    for(int i=0; i<3; i++)
    {
        insert2(&head, arr[i]);
        display(head);
    }

    printf("insert3:\n");
    for(int i=0; i<3; i++)
    {
        head = insert3(head, arr[i]);
        display(head);
    }
    return 0;
}