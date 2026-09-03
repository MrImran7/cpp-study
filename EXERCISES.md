# C/C++ Practice Questions

Compile and run:

```bash
gcc -std=c17 -Wall -Wextra -Wpedantic array.c -o array
gcc -std=c17 -Wall -Wextra -Wpedantic dsa.c -o dsa
gcc -std=c17 -Wall -Wextra -Wpedantic pointers.c -o pointers
g++ -std=c++17 -Wall -Wextra -Wpedantic cpp17.cpp -o cpp17
```

## Array questions

1. Multiply every array element by a number.
2. Find the minimum and maximum in one traversal.
3. Reverse an array without another array.
4. Move all zeros to the end while preserving order.
5. Find the second-largest distinct value.
6. Rotate an array left by `k` positions.
7. Remove duplicates from a sorted array.
8. Merge two sorted arrays.

## DSA questions

1. Implement linear and binary search.
2. Explain why binary search needs sorted input.
3. Implement bubble, selection, and insertion sort.
4. Reverse a singly linked list.
5. Detect a loop in a linked list.
6. Implement a stack using an array.
7. Implement a circular queue.
8. Check balanced parentheses using a stack.
9. Traverse a binary tree using DFS and BFS.
10. Compare time and space complexity for each solution.

## Pointer and memory questions

1. Explain `const int*`, `int* const`, and `const int* const`.
2. Swap two values using pointers.
3. Allocate and free a dynamic array safely.
4. Explain a dangling pointer, wild pointer, and null pointer.
5. Find and repair a memory leak.
6. Explain shallow copy versus deep copy.
7. Write an RAII wrapper for a Linux file descriptor in C++.

## Modern C++ questions

1. Replace a raw owning pointer with `std::unique_ptr`.
2. Demonstrate copy and move construction.
3. Use `std::optional` instead of a sentinel return value.
4. Use `std::variant` with `std::visit`.
5. Use structured bindings with a map.
6. Rewrite a loop using a standard algorithm.
7. Explain when a `std::string_view` can dangle.
8. Build a thread-safe queue with a condition variable.
9. Constrain a template using a C++20 concept.
10. Return a value or error using C++23 `std::expected`.

## Extension rule

For every exercise, record:

- Input and expected output
- Time and space complexity
- Boundary cases
- Ownership and lifetime assumptions
- Compiler warnings and sanitizer results
