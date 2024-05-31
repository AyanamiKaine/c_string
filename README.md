# C Dynamic String Library (c_string)

This library provides a custom `String` structure and functions for working with dynamic strings in C. It aims to offer flexibility in memory management and efficient string manipulation.

## Features

- **Dynamic Memory Allocation:** Strings automatically resize as needed when appending characters or other strings.
- **Memory Management Options:**
  - **Heap Allocation (malloc):** Ideal for general-purpose string handling.
  - **Arena Allocation:** More efficient for managing a large number of strings with similar lifetimes.
- **String Manipulation:**
  - Appending character arrays (`char*`)
  - Appending other `String` structures
  - Determining string length
- **Memory Safety:**
  - Includes null terminator handling for correct string operations.
  - Robust error handling with `ArenaError` type for arena-related operations.

## Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/AyanamiKaine/c_string.git
   ```

2. **Include the header file:**
   ```c
   #include "c_string.h"
   ```
   When you want to use the arena allocator use https://github.com/AyanamiKaine/arena_allocator

## Usage

### Heap Allocation

```c
String *myString = new_string_malloc("Hello, ");
string_append_char_array_malloc(myString, "world!");
// myString now contains "Hello, world!"

string_free(myString); // Free the memory when done
```

### Arena Allocation

```c
Arena myArena;
arena_new(&myArena, 1024, true); // Initialize an arena

String *str1 = new_string_arena("This is ", &myArena);
String *str2 = new_string_arena("an arena string.", &myArena);
string_append_string_arena(str1, str2, &myArena);
// str1 now contains "This is an arena string."

arena_free(&myArena); // Free the entire arena (frees all strings inside)
```

### Other Functions

```c
size_t len = string_length(str1); // Get length of str1
char c = string_char_at_index(str1, 3); // Get the 4th character
```

## Error Handling

Functions using arena allocation return an `ArenaError` value:

- `ARENA_SUCCESS` - Operation completed successfully.
- `ARENA_ERROR_ALLOCATION_FAILED` - Initial arena allocation failed.
- `ARENA_ERROR_REALLOCATION_FAILED` - Arena expansion failed (when appending to a string).

Check the return value and handle errors appropriately in your application.

## Contributing

Contributions are welcome! Please feel free to open issues or pull requests.

## License

This library is licensed under the [MIT License](LICENSE).
