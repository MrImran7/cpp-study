#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * Q1: What is the difference between a pointer and a reference?
 * A: C has pointers, not C++ references. A pointer stores an address, may be
 *    NULL, may be reassigned, and must be dereferenced to access the object.
 */
void swap_values(int *left, int *right)
{
    if (left == NULL || right == NULL) {
        return;
    }

    const int temporary = *left;
    *left = *right;
    *right = temporary;
}

/*
 * Q2: How is a dynamic array safely allocated?
 * A: Validate the requested count, check malloc, and free exactly once.
 */
int *create_sequence(size_t count)
{
    if (count == 0 || count > SIZE_MAX / sizeof(int)) {
        return NULL;
    }

    int *values = malloc(count * sizeof *values);
    if (values == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        values[i] = (int)(i + 1);
    }
    return values;
}

int main(void)
{
    int first = 10;
    int second = 20;
    swap_values(&first, &second);
    printf("After swap: %d %d\n", first, second);

    const size_t count = 5;
    int *sequence = create_sequence(count);
    if (sequence == NULL) {
        fprintf(stderr, "Allocation failed\n");
        return 1;
    }

    for (size_t i = 0; i < count; ++i) {
        printf("%d%s", sequence[i], i + 1 == count ? "\n" : " ");
    }

    free(sequence);
    sequence = NULL;
    return 0;
}
