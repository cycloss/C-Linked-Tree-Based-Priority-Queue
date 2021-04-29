# Linked Binary Tree Based Priority Queue in C

I wanted to avoid using an array based implementation for the heap because memory reallocation of the array on expansion can incur an `O(n)` time complexity if the entire array has to be moved to a different block of memory. Using a linked binary tree approach solves this.

A challenging part was determining where the next node's location should be in the tree. This can be achieved by converting the count of nodes in the tree including the node to be inserted to a binary number, dropping the msb, and then following the resulting bits as a map, going left for a `0` and right for a `1`.
