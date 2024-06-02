/**
 * @file c_string.h
 * @brief Implementation of a Dynamic String (String) Structure for C 
 *
 * This module provides a custom `String` structure and associated functions for working with 
 * dynamic strings in C. It offers memory management flexibility with both heap allocation
 * (using `malloc`) and arena allocation.
 * 
 * **Features:**
 *  - Dynamic Memory Allocation: Automatically adjusts string size as needed.
 *  - Memory Management Options: Choose between heap allocation and arena allocation for memory efficiency.
 *  - String Manipulation: Functions to append strings and character arrays.
 *  - Memory Safety: Includes null terminator handling for correct string operations.
 *
 */

#ifndef C_STRING_H // Check if the macro is not defined
#define C_STRING_H // Define the macro

#include "arena.h"

typedef struct
{
    char *data;       // Pointer to the char array (Here we store our string content)
    size_t length;    // Current Length of the String excluding the Null Terminator
    size_t capacity;  // Total allocated size of the data buffer
} String;

/**
 * @brief Creates a new `String` allocated on the heap (using `malloc`).
 *
 * @param initial_str The initial string to copy into the new `String`. Can be NULL.
 * @return A pointer to the newly created `String`, or NULL if allocation fails.
 */
String *new_string_malloc(const char *inital_str);

/**
 * @brief Creates a new `String` within a given arena.
 *
 * This function is used to allocate a `String` structure and its string data within a 
 * pre-allocated memory arena (`Arena`). This can be more efficient than heap allocation
 * (`malloc`) when managing many small strings.
 *
 * @param initial_str The initial string to copy into the new `String`. Can be NULL.
 * @param arena A pointer to the `Arena` structure where the `String` will be allocated.
 * @return A pointer to the newly created `String`, or NULL if allocation fails.
 *
 * @note  If allocation of either the `String` structure or its string data fails, the arena
 *        will be reset to its state before this function was called.
 */
String *new_string_arena(const char *inital_str, Arena *arena);

/**
 * @brief Retrieves the character at the specified index in the `String`.
 *
 * @param string The `String` to access.
 * @param index The index of the character to retrieve.
 * @return The character at the given index, or '\0' if the index is out of bounds.
 */
char string_char_at_index(const String *string, size_t index);

/**
 * @brief Appends a character array (`char *`) to a `String`.
 *
 * This function is used to append a null-terminated character array (`char *`) to the end of an existing `String`. 
 * It handles arena-allocated strings.
 *
 * @param dest The destination `String` to append to.
 * @param src The character array to append.
 * @param arena A pointer to the `Arena` if `dest` is arena-allocated. If NULL, it is assumed that `dest` is malloc-allocated.
 */
ArenaError string_append_char_array_arena(String *dest, const char *src, Arena *arena); 

/**
 * @brief Appends a character array (`char *`) to a malloc-allocated `String`.
 *
 * This function automatically reallocates memory for the `dest` string if needed to 
 * accommodate the appended data.
 *
 * @param dest The destination `String` to append to.
 * @param src The character array to append.
 */
void string_append_char_array_malloc(String *dest, const char *src);

/**
 * @brief Appends another `String` to a malloc-allocated `String`.
 *
 * This function internally calls `string_append_char_array_malloc` to append the data 
 * from the source `String`.
 *
 * @param dest The destination `String` to append to.
 * @param src The source `String` to append.
 */
void string_append_string_malloc(String *dest, const String *src);

/**
 * @brief Appends another `String` to a `String`.
 *
 * This function appends the contents of the `src` String to the end of the `dest` String. It automatically
 * handles reallocation if necessary.
 *
 * @param dest The destination `String` to append to.
 * @param src The source `String` to append.
 * @param arena A pointer to the `Arena` if both strings are arena-allocated.
 */
void string_append_string_arena(String *dest, const String *src, Arena *arena);

/**
 * @brief  Calculates the length of a `String`.
 * 
 * This function determines the number of characters in the given `String`, 
 * excluding the null terminator.
 *
 * @param string The `String` whose length you want to find.
 * 
 * @return The length of the `String` (the number of characters), or 0 if `string` is NULL.
 *
 * @note This function does not modify the `String` in any way.
 * 
 * @example
 * String* myString = new_string_malloc("Hello");
 * size_t length = string_length(myString); // length will be 5
 * 
 */
size_t string_length(String *string);

/**
 * @brief Frees the memory allocated for a `String` created with `new_string_malloc`.
 *
 * This function first frees the internal string data (`string->data`) and then frees 
 * the `String` structure itself.
 *
 * @param string The `String` to free.
 */
void string_free(String *string);
#endif // End of the conditional compilation block
