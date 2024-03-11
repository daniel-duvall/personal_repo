/*
Programming Assignment 3 - CS1 - Torosdigali

Daniel DuVall - 03/04/2024
*/

#include <stdio.h>
#include <stdlib.h>

#define NAME_LEN 51
#define MAX_Q 12
#define MAX 2000000000 

typedef struct Customer
{
    char name[NAME_LEN];
    int num_t;
    int arrival_time;
    int line;//represents what queue the person is in 
}Customer;

typedef struct Node
{
    Customer* person;
    struct Node* next;
} Node;

typedef struct Queue
{
    Node* front;
    Node* rear; 
    int size;
    int curr_size; //made this variable so I could traverse the linked list without worrying about going out of bouds  
}Queue;

typedef struct booth
{
    int num_q;
    int *qs;//integer array to store the number of each queue
}booth;

//loads info into customer struct and returns it 
Customer* loadCustomer()
{
    Customer* cst = (Customer*)malloc(sizeof(Customer));
    scanf("%s %d %d", cst->name, &cst->num_t, &cst->arrival_time);
    cst->line = 0;
    return cst;
}

//loads data into node struct and return 
Node* createNode()
{
    Node* nd = (Node*)malloc(sizeof(Node));
    Customer* cst = loadCustomer();
    nd->person = cst;
    nd->next = NULL;

    return nd;
}

//initialize a queue pointer and returns it 
Queue* createQueues()
{
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    q->curr_size = 0;
    q->size = 0;

    return q;
}

//returns 1 if queue is empty, 0 otherwise 
int isEmpty(Queue *q)
{
    return q->curr_size == 0 ? 1 : 0;
}

//enqueue function meant to add a node to a linked list within a queue
void enqueue(Queue* q, Node* n)
{
    //if rear == null, list is empty. make front and rear ptr point to same node
    if(q->rear == NULL)
    {
        q->rear = q->front = n;
    }
    else
    {
        //set node after rear ptr
        q->rear->next = n;
        //set rear to point to next
        q->rear = q->rear->next;
    }
    //increment size variables accordingly 
    q->curr_size++;
    q->size++;
}

//peek function - returns the customer struct at the front of the queue but does not dequeue it 
Customer* peek(Queue *q)
{
    if(isEmpty(q))
        return NULL;
    else
        return q->front->person;
}

//dequeues the customer struct at the front of the queue and dequeues it
Customer* dequeue(Queue * q)
{
    Customer* c;
    //there is only one node in the list, remove and set both front and rear to NULL
    if(q->curr_size == 1)
    {
        c = q->front->person;
        free(q->front);
        q->front = q->rear = NULL;
        q->curr_size = 0;
    }
    else if(!isEmpty(q))
    {
        c = peek(q);
        //use temp to avoid use after free
        Node * temp = q->front;

        //front ptr equals second node in list
        q->front = q->front->next;

        //free old head ptr
        free(temp);

        q->curr_size--;
    }
    else if(isEmpty(q)) //returns NULL if queue is empty 
        return NULL; 

    return c;
}

//this functions assigns the customers to their proper queues and then their proper booths, per assignment requirements 
void assignQueues(Queue **q_arr, booth* b_arr, int num_c, int num_b)
{
    //loops through all the people 
    for(int i=0; i<num_c; i++)
    {
        Node* temp = createNode();//temp node created to add to the new queue
        if(temp == NULL)
            break;
        char f_letter = temp->person->name[0];
        int q_num = f_letter%13;
        //find out which queue each person belongs in
        if(q_num!=0)//if their name % 13 ==0, or their name started with an A or an N
        {
            temp->person->line = q_num;
            enqueue(q_arr[q_num-1], temp);
        }
        else if(q_num==0)
        {
            //min algorithm to find which non-empty queue as the smallest size that has at least a size of one 
            int min_q_index = 0;
            int min_q_size = MAX;
            for(int j=0; j<MAX_Q; j++)//loop through the queues 
            {
                if((q_arr[j]->size<min_q_size) && (q_arr[j]->size>=1))
                {
                    min_q_size = q_arr[j]->size;
                    min_q_index = j;
                }
            }
            //assigns queue number and then adds the node to the que 
            temp->person->line = min_q_index+1;
            enqueue(q_arr[min_q_index], temp);
        }
    }

    //ASSIGNING QUEUES TO BOOTHS - essentially 2nd phase of this function

    int non_empty_q = 0;//find out how many non_empty queues there are 
    for(int i=0; i<MAX_Q; i++)
    {
        if(q_arr[i]->size > 0)
            non_empty_q++;
    }
    int ispopulated[non_empty_q]; //array that stores numbers representing non-empty queues 
    int k=0;
    for(int i=0; i<MAX_Q; i++)
    {
        if(q_arr[i]->size > 0)
        {
            ispopulated[k] = i+1; //
            k++;
        }
    }
    //calculates how many queues per booth there are and how much of a remainder there is 
    int q_per_booth = non_empty_q/num_b;
    int remainder = non_empty_q%num_b;
    k=0;
    for(int i=0; i<num_b; i++) //loops through booths 
    {
        if(remainder>0)//assigns the queues to the booths appropriately 
        {
            b_arr[i].num_q = q_per_booth+1;
            remainder--;
        }
        else
            b_arr[i].num_q = q_per_booth;

        //allocate integer list for list of queues 
        b_arr[i].qs = (int*)malloc(b_arr[i].num_q*sizeof(int));
        
        for(int j=0; j<b_arr[i].num_q; j++)
        {
            b_arr[i].qs[j] = ispopulated[k];
            k++;
        }
    }
}
//finds the person with the smallest arrival_time and return the queue number 
int arrived_first(Queue** q_arr, int* q_nums, int num_q)
{
    int min_time = MAX; 
    int min_queue = -1;
    int temp_time = 0;

    //finds which customer arrives first 
    for(int i=0; i<num_q; i++)
    {
        if(peek(q_arr[q_nums[i]-1])==NULL)//peek function to check if there's at least one person
        {
            temp_time=MAX;
        }
        else
        {
            temp_time = peek(q_arr[q_nums[i]-1])->arrival_time; // checks the arrival time of each person
        }
        if(temp_time < min_time)
        {
            min_time = temp_time;
            min_queue = i;
        }
    }
// if min_time never changed then return -1
    if(min_time==MAX)
    {
        return -1;
    }
    return q_nums[min_queue];//return index of queue 
}

//does checkout process for each booth, ran in for loop
void checkout_booth(Queue** q_arr, booth* bth)
{
    Customer* cst; 
    int min_queue = -1; 
    int time = 0;
    int running = 1; //indicatore to determine if the simulation is still running 

    while (running)
    {
        min_queue = arrived_first(q_arr, bth->qs, bth->num_q);//gets the person who arrived first 
        if(min_queue==-1)//if min_queue ==-1 that means that there's no one left to checkout 
        {
            running=0;//break out of the while loop and then set running to 0
            break;
        }
        cst = dequeue(q_arr[min_queue-1]);//gets the customer at the front of the queue and removes it form the list 
        //checks if the customer is null
        if(cst == NULL)
        {
            running = 0;
            break;             
        }
        if(cst->arrival_time < time)
            cst->arrival_time = time;
        if(cst->arrival_time > time)
            time = cst->arrival_time;
        //calculate checkout time and prints info 
        time = time+30+(cst->num_t * 5);
        printf("%s from line %d checks out at time %d.\n", cst->name, min_queue, time);
        free(cst);//frees the customer 
    } 
    free(bth->qs);//free the dynamically allocated integer array in the booth struct 
}

//function i made to free the queues 
void freeQueues(Queue **q)
{
    for(int i=0; i<MAX_Q; i++)
    {
        while(q[i]->rear) 
            dequeue(q[i]);
        free(q[i]);
    }
    free(q);
}

int main(void)
{
    int num_c, num_b;
    scanf("%d %d", &num_c, &num_b);//gets number of customers and booths 

//array of queue pointers(i prefer using the arrows) of size 12(MAX_Q)
    Queue **q_ptr_arr = (Queue**)malloc(MAX_Q*sizeof(Queue*));//creates an array of queue pointers of size 12. 
    for(int i=0; i<MAX_Q; i++)
        q_ptr_arr[i] = createQueues();
    
    //creats array of booth structs 
    booth* booth_arr = (booth*)malloc(num_b*sizeof(booth));

//assign queues and booths, and then checkouts everyone 
    assignQueues(q_ptr_arr, booth_arr, num_c, num_b);

    for(int i=0; i<num_b; i++)
    {
        printf("Booth %d\n", i+1);
        checkout_booth(q_ptr_arr, &booth_arr[i]);
        printf("\n");
    }
//free any remaining dynamically allocated memory 
    free(booth_arr);
    freeQueues(q_ptr_arr);
    return 0;
}