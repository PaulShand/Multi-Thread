# Multithreaded Queue Editing and Object Locking

## Overview

This assignment explores the concepts of multithreading in C by implementing two critical components: a custom reader-writer lock (`rwlock.c`) and a thread-safe queue (`queue.c`). Both these components use pthreads to manage concurrency and ensure thread safety.

## Reader-Writer Lock (`rwlock.c`)

### Functionality

- **Purpose**: Provides a mechanism to allow multiple threads to read or exclusively write to a shared resource.
- **Priority Schemes**: Supports three priority schemes for managing reader and writer access:
  - `READERS`: Prioritizes reader access.
  - `WRITERS`: Prioritizes writer access.
  - `N_WAY`: Allows a certain number of readers before switching priority to writers.
- **Structure**: Includes counters for active and waiting readers and writers, a reader pass counter, and condition variables for synchronization.

### Key Operations

- **Reader Lock**: Manages reader access, ensuring no active writers or writer preference (based on the priority scheme).
- **Writer Lock**: Grants exclusive access to a writer, blocking if there are active readers or other writers.

## Thread-Safe Queue (`queue.c`)

### Functionality

- **Purpose**: Implements a circular queue that is safe for concurrent access by multiple threads.
- **Structure**: Contains pointers for queue items, indices for insertion and removal, a counter for the number of items, and synchronization primitives.

### Key Operations

- **Queue Push**: Adds an item to the queue, waiting if the queue is full.
- **Queue Pop**: Removes an item from the queue, waiting if the queue is empty.

## Testing and Validation

- **Multithreading Scenarios**: Both components are tested in various multithreaded scenarios to ensure proper synchronization and data integrity.
- **Memory Management**: Ensures no memory leaks or race conditions.

## Conclusion

This assignment demonstrates the effective use of multithreading and synchronization mechanisms in C to manage shared resources safely. The implementation of a custom reader-writer lock and a thread-safe queue provides a practical understanding of concurrency control in software development.

