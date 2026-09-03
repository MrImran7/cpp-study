#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/*
 * ARRAY GUIDE
 * An array stores same-type elements next to each other in memory.
 *
 *   int a[5] = {10, 20, 30, 40, 50};
 *   index       0   1   2   3   4
 *
 * Indexes start at 0. C does not check bounds; a[5] is invalid here.
 */

void print_array(const int a[], size_t n)
{
    printf("[");
    for (size_t i = 0; i < n; ++i)
        printf("%d%s", a[i], i + 1 == n ? "" : ", ");
    printf("]\n");
}

/* Q1: How is array length calculated? */
void size_demo(void)
{
    int a[] = {10, 20, 30, 40, 50};
    size_t n = sizeof a / sizeof a[0];
    printf("array bytes=%zu, element bytes=%zu, elements=%zu\n",
           sizeof a, sizeof a[0], n);
    /* This formula works only where a is an actual array, not a parameter. */
}

/* Q2: Multiply each element. O(n) time, O(1) extra space. */
void multiply(int a[], size_t n, int factor)
{
    for (size_t i = 0; i < n; ++i)
        a[i] *= factor;
}

/* Q3: Sum and average. */
long long sum(const int a[], size_t n)
{
    long long total = 0;
    for (size_t i = 0; i < n; ++i)
        total += a[i];
    return total;
}

double average(const int a[], size_t n)
{
    return n == 0 ? 0.0 : (double)sum(a, n) / (double)n;
}

/* Q4: Return minimum and maximum through pointer output parameters. */
bool min_max(const int a[], size_t n, int *minimum, int *maximum)
{
    if (n == 0 || minimum == NULL || maximum == NULL)
        return false;

    *minimum = *maximum = a[0];
    for (size_t i = 1; i < n; ++i) {
        if (a[i] < *minimum) *minimum = a[i];
        if (a[i] > *maximum) *maximum = a[i];
    }
    return true;
}

/* Q5: Linear search returns index or -1. O(n). */
int search(const int a[], size_t n, int target)
{
    for (size_t i = 0; i < n; ++i)
        if (a[i] == target) return (int)i;
    return -1;
}

/* Q6: Reverse in place with two indexes. */
void reverse(int a[], size_t n)
{
    if (n == 0) return;
    size_t left = 0, right = n - 1;
    while (left < right) {
        int temporary = a[left];
        a[left++] = a[right];
        a[right--] = temporary;
    }
}

/*
 * Q7: Insert into a fixed-capacity array.
 * Shift elements right first. *n is the number currently used.
 */
bool insert_at(int a[], size_t capacity, size_t *n,
               size_t index, int value)
{
    if (n == NULL || *n == capacity || index > *n) return false;
    for (size_t i = *n; i > index; --i)
        a[i] = a[i - 1];
    a[index] = value;
    ++(*n);
    return true;
}

/* Q8: Delete by shifting later elements left. */
bool delete_at(int a[], size_t *n, size_t index)
{
    if (n == NULL || index >= *n) return false;
    for (size_t i = index; i + 1 < *n; ++i)
        a[i] = a[i + 1];
    --(*n);
    return true;
}

/* Q9: Move zeroes to the end without changing non-zero order. */
void move_zeroes(int a[], size_t n)
{
    size_t write = 0;
    for (size_t read = 0; read < n; ++read)
        if (a[read] != 0) a[write++] = a[read];
    while (write < n) a[write++] = 0;
}

int main(void)
{
    puts("--- Size ---");
    size_demo();

    puts("\n--- Traverse, multiply, sum, search, reverse ---");
    int a[] = {2, 3, 5, 7, 8};
    size_t n = sizeof a / sizeof a[0];
    printf("original: "); print_array(a, n);
    multiply(a, n, 3);
    printf("x3: "); print_array(a, n);
    printf("sum=%lld average=%.2f index-of-15=%d\n",
           sum(a, n), average(a, n), search(a, n, 15));
    int minimum, maximum;
    if (min_max(a, n, &minimum, &maximum))
        printf("minimum=%d maximum=%d\n", minimum, maximum);
    reverse(a, n);
    printf("reversed: "); print_array(a, n);

    puts("\n--- Insert and delete ---");
    int editable[10] = {10, 20, 30, 40};
    n = 4;
    insert_at(editable, 10, &n, 2, 25);
    print_array(editable, n);
    delete_at(editable, &n, 1);
    print_array(editable, n);

    puts("\n--- Move zeroes ---");
    int z[] = {0, 1, 0, 3, 12};
    n = sizeof z / sizeof z[0];
    move_zeroes(z, n);
    print_array(z, n);
    return 0;
}
