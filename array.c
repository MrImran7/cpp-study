#include <stdio.h>
#include <stddef.h>

/*
 * Q1: How do you multiply every element of an array?
 * A: Visit each element once and multiply it by the given factor.
 *    Time: O(n), extra space: O(1).
 */
void multiply_array(int values[], size_t size, int factor)
{
    for (size_t i = 0; i < size; ++i) {
        values[i] *= factor;
    }
}

/*
 * Q2: How do you find the largest element?
 * A: Keep the best value seen so far. The caller must provide a non-empty array.
 */
int find_max(const int values[], size_t size)
{
    int maximum = values[0];

    for (size_t i = 1; i < size; ++i) {
        if (values[i] > maximum) {
            maximum = values[i];
        }
    }
    return maximum;
}

/* Q3: Reverse an array in place using two indexes. */
void reverse_array(int values[], size_t size)
{
    if (size == 0) {
        return;
    }

    for (size_t left = 0, right = size - 1; left < right; ++left, --right) {
        const int temporary = values[left];
        values[left] = values[right];
        values[right] = temporary;
    }
}

void print_array(const int values[], size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        printf("%d%s", values[i], i + 1 == size ? "\n" : " ");
    }
}

int main(void)
{
    int values[] = {2, 3, 5, 7, 8};
    const size_t size = sizeof values / sizeof values[0];

    printf("Original: ");
    print_array(values, size);

    multiply_array(values, size, 3);
    printf("Multiplied by 3: ");
    print_array(values, size);

    printf("Maximum: %d\n", find_max(values, size));

    reverse_array(values, size);
    printf("Reversed: ");
    print_array(values, size);
    return 0;
}
