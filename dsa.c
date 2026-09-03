#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * DSA GUIDE
 * Data structure = how data is organized.
 * Algorithm      = steps that solve a problem.
 *
 * O(1): constant, O(log n): halves work, O(n): visits all,
 * O(n log n): efficient sorting, O(n^2): nested full loops.
 */

void print_array(const int a[], size_t n)
{
    for (size_t i = 0; i < n; ++i)
        printf("%d%s", a[i], i + 1 == n ? "\n" : " ");
}

/* Q1: Linear search works on unsorted data. O(n). */
int linear_search(const int a[], size_t n, int target)
{
    for (size_t i = 0; i < n; ++i)
        if (a[i] == target) return (int)i;
    return -1;
}

/* Q2: Binary search needs sorted data. O(log n). */
int binary_search(const int a[], size_t n, int target)
{
    size_t left = 0, right = n; /* range [left, right) */
    while (left < right) {
        size_t middle = left + (right - left) / 2;
        if (a[middle] == target) return (int)middle;
        if (a[middle] < target) left = middle + 1;
        else right = middle;
    }
    return -1;
}

/* Q3: Bubble sort pushes the largest remaining value right. O(n^2). */
void bubble_sort(int a[], size_t n)
{
    for (size_t end = n; end > 1; --end) {
        bool swapped = false;
        for (size_t i = 1; i < end; ++i) {
            if (a[i - 1] > a[i]) {
                int t = a[i - 1]; a[i - 1] = a[i]; a[i] = t;
                swapped = true;
            }
        }
        if (!swapped) return;
    }
}

/* Q4: Selection sort selects the smallest remaining value. O(n^2). */
void selection_sort(int a[], size_t n)
{
    for (size_t current = 0; current < n; ++current) {
        size_t smallest = current;
        for (size_t i = current + 1; i < n; ++i)
            if (a[i] < a[smallest]) smallest = i;
        int t = a[current]; a[current] = a[smallest]; a[smallest] = t;
    }
}

/* Q5: Insertion sort grows a sorted left section. O(n^2). */
void insertion_sort(int a[], size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        int key = a[i];
        size_t position = i;
        while (position > 0 && a[position - 1] > key) {
            a[position] = a[position - 1];
            --position;
        }
        a[position] = key;
    }
}

/* Q6: Stack = LIFO (last in, first out). */
#define CAPACITY 5
typedef struct { int data[CAPACITY]; size_t size; } Stack;

bool push(Stack *s, int value)
{
    if (s == NULL || s->size == CAPACITY) return false;
    s->data[s->size++] = value;
    return true;
}

bool pop(Stack *s, int *value)
{
    if (s == NULL || value == NULL || s->size == 0) return false;
    *value = s->data[--s->size];
    return true;
}

/* Q7: Circular queue = FIFO (first in, first out). */
typedef struct {
    int data[CAPACITY];
    size_t front;
    size_t size;
} Queue;

bool enqueue(Queue *q, int value)
{
    if (q == NULL || q->size == CAPACITY) return false;
    size_t rear = (q->front + q->size) % CAPACITY;
    q->data[rear] = value;
    ++q->size;
    return true;
}

bool dequeue(Queue *q, int *value)
{
    if (q == NULL || value == NULL || q->size == 0) return false;
    *value = q->data[q->front];
    q->front = (q->front + 1) % CAPACITY;
    --q->size;
    return true;
}

/* Q8: Singly linked list: each dynamic node points to the next. */
typedef struct Node { int value; struct Node *next; } Node;

bool list_push_front(Node **head, int value)
{
    if (head == NULL) return false;
    Node *node = malloc(sizeof *node);
    if (node == NULL) return false;
    node->value = value;
    node->next = *head;
    *head = node;
    return true;
}

void list_reverse(Node **head)
{
    if (head == NULL) return;
    Node *previous = NULL;
    Node *current = *head;
    while (current != NULL) {
        Node *next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    *head = previous;
}

void list_print(const Node *node)
{
    while (node != NULL) {
        printf("%d%s", node->value, node->next ? " -> " : "\n");
        node = node->next;
    }
}

void list_destroy(Node **head)
{
    if (head == NULL) return;
    while (*head != NULL) {
        Node *old = *head;
        *head = old->next;
        free(old);
    }
}

int main(void)
{
    const int sorted[] = {1, 3, 5, 7, 9, 11};
    size_t n = sizeof sorted / sizeof sorted[0];
    printf("linear(7)=%d binary(7)=%d\n",
           linear_search(sorted, n, 7), binary_search(sorted, n, 7));

    int a[] = {7, 2, 9, 1, 5}, b[] = {7, 2, 9, 1, 5};
    int c[] = {7, 2, 9, 1, 5};
    n = sizeof a / sizeof a[0];
    bubble_sort(a, n); selection_sort(b, n); insertion_sort(c, n);
    printf("bubble: "); print_array(a, n);
    printf("selection: "); print_array(b, n);
    printf("insertion: "); print_array(c, n);

    Stack stack = {{0}, 0};
    push(&stack, 10); push(&stack, 20); push(&stack, 30);
    int value;
    printf("stack pops:");
    while (pop(&stack, &value)) printf(" %d", value);
    printf("\n");

    Queue queue = {{0}, 0, 0};
    enqueue(&queue, 10); enqueue(&queue, 20); enqueue(&queue, 30);
    printf("queue removes:");
    while (dequeue(&queue, &value)) printf(" %d", value);
    printf("\n");

    Node *head = NULL;
    list_push_front(&head, 30); list_push_front(&head, 20);
    list_push_front(&head, 10);
    printf("list: "); list_print(head);
    list_reverse(&head);
    printf("reversed: "); list_print(head);
    list_destroy(&head);
    return 0;
}
