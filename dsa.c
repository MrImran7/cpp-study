#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/*
 * Q1: Linear search
 * Works on any array. Time: O(n), space: O(1).
 */
int linear_search(const int values[], size_t size, int target)
{
    for (size_t i = 0; i < size; ++i) {
        if (values[i] == target) {
            return (int)i;
        }
    }
    return -1;
}

/*
 * Q2: Binary search
 * Requires a sorted array. Time: O(log n), space: O(1).
 */
int binary_search(const int values[], size_t size, int target)
{
    size_t left = 0;
    size_t right = size;

    while (left < right) {
        const size_t middle = left + (right - left) / 2;

        if (values[middle] == target) {
            return (int)middle;
        }
        if (values[middle] < target) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }
    return -1;
}

/* Q3: Bubble sort with early exit. Time: O(n^2), space: O(1). */
void bubble_sort(int values[], size_t size)
{
    for (size_t end = size; end > 1; --end) {
        bool swapped = false;

        for (size_t i = 1; i < end; ++i) {
            if (values[i - 1] > values[i]) {
                const int temporary = values[i - 1];
                values[i - 1] = values[i];
                values[i] = temporary;
                swapped = true;
            }
        }

        if (!swapped) {
            return;
        }
    }
}

int main(void)
{
    int values[] = {7, 2, 9, 1, 5};
    const size_t size = sizeof values / sizeof values[0];

    printf("Linear search for 9: index %d\n", linear_search(values, size, 9));

    bubble_sort(values, size);
    printf("Sorted:");
    for (size_t i = 0; i < size; ++i) {
        printf(" %d", values[i]);
    }
    printf("\nBinary search for 5: index %d\n",
           binary_search(values, size, 5));
    return 0;
}
