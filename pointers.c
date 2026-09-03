#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * POINTER GUIDE
 * int number = 10;       normal integer
 * int *p = &number;      p stores number's address
 * &number                address-of operator
 * *p                     dereference: access value at stored address
 */

void address_and_dereference(void)
{
    int number = 10;
    int *p = &number;
    printf("number=%d address=%p p=%p *p=%d\n",
           number, (void *)&number, (void *)p, *p);
    *p = 99; /* write through pointer */
    printf("after *p=99, number=%d\n", number);
}

/* Arrays and pointers are related, but an array is not a pointer. */
void pointer_arithmetic(void)
{
    int a[] = {10, 20, 30, 40};
    int *p = a; /* points to a[0] */
    printf("a[2]=%d and *(p+2)=%d\n", a[2], *(p + 2));
    for (size_t i = 0; i < 4; ++i)
        printf("p+%zu=%p value=%d\n", i, (void *)(p + i), p[i]);
    /* p+1 moves sizeof(int) bytes. p[i] means *(p+i). */
}

/* A pointer parameter allows modification of the caller's object. */
void swap(int *left, int *right)
{
    if (left == NULL || right == NULL) return;
    int temporary = *left;
    *left = *right;
    *right = temporary;
}

/* int ** lets this function modify the caller's pointer itself. */
bool allocate_integer(int **output, int initial_value)
{
    if (output == NULL) return false;
    int *memory = malloc(sizeof *memory);
    if (memory == NULL) return false;
    *memory = initial_value;
    *output = memory;
    return true;
}

/*
 * DYNAMIC MEMORY ALLOCATION
 * malloc: uninitialized memory
 * calloc: zero-initialized memory
 * realloc: resize allocation
 * free: release allocation
 *
 * Do not confuse dynamic allocation with hardware DMA (Direct Memory
 * Access). Hardware DMA is normally configured through a device driver;
 * user code uses driver APIs, ioctl/mmap, or platform frameworks.
 */
int *create_sequence(size_t count)
{
    if (count == 0 || count > SIZE_MAX / sizeof(int)) return NULL;
    int *a = malloc(count * sizeof *a);
    if (a == NULL) return NULL;
    for (size_t i = 0; i < count; ++i) a[i] = (int)(i + 1);
    return a;
}

void calloc_demo(void)
{
    size_t n = 5;
    int *a = calloc(n, sizeof *a);
    if (a == NULL) return;
    printf("calloc:");
    for (size_t i = 0; i < n; ++i) printf(" %d", a[i]);
    printf("\n");
    free(a);
}

void realloc_demo(void)
{
    size_t old_count = 3, new_count = 6;
    int *a = create_sequence(old_count);
    if (a == NULL) return;

    /* Use a temporary: directly assigning can lose a if realloc fails. */
    int *resized = realloc(a, new_count * sizeof *a);
    if (resized == NULL) {
        free(a);
        return;
    }
    a = resized;
    for (size_t i = old_count; i < new_count; ++i) a[i] = (int)(i + 1);
    printf("realloc:");
    for (size_t i = 0; i < new_count; ++i) printf(" %d", a[i]);
    printf("\n");
    free(a);
}

void const_pointer_demo(void)
{
    int a = 10, b = 20;
    const int *pointer_to_const = &a; /* pointer may change; value may not */
    pointer_to_const = &b;
    int *const const_pointer = &a;    /* pointer fixed; value may change */
    *const_pointer = 11;
    const int *const both_const = &b; /* neither may change */
    printf("pointer-to-const=%d const-pointer=%d both=%d\n",
           *pointer_to_const, *const_pointer, *both_const);
}

int main(void)
{
    puts("--- Address and dereference ---");
    address_and_dereference();
    puts("\n--- Pointer arithmetic ---");
    pointer_arithmetic();

    int a = 10, b = 20;
    swap(&a, &b);
    printf("\nAfter swap: a=%d b=%d\n", a, b);

    int *dynamic = NULL;
    if (allocate_integer(&dynamic, 42)) {
        printf("pointer-to-pointer allocated: %d\n", *dynamic);
        free(dynamic);
        dynamic = NULL; /* do not keep a dangling address */
    }

    int *sequence = create_sequence(5);
    if (sequence == NULL) return 1;
    printf("malloc:");
    for (size_t i = 0; i < 5; ++i) printf(" %d", sequence[i]);
    printf("\n");
    free(sequence);
    sequence = NULL;

    calloc_demo();
    realloc_demo();
    const_pointer_demo();
    return 0;
}
