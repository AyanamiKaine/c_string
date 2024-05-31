#include "c_string.h"
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <stdalign.h>
#include <stdarg.h> // Needed for va_list (variable argument lists)
#include <stdio.h> // Needed for vsnprintf

String *new_string_malloc(const char *initial_str)
{
    String *str = (String *)malloc(sizeof(String));

    if (str == NULL){
        return NULL;
    }

    // Here we get the inital length of our provided inital string, 
    // if we didnt provide one and the pointer is set to null ,
    // we set the length to 0 
    size_t length_of_initial_str = initial_str ? strlen(initial_str) : 0;
    str->data = (char *)malloc(length_of_initial_str + 1);
    str->length = length_of_initial_str;
    str->capacity = length_of_initial_str + 1; // Capacity is one more for null terminator
    
    if (str->data == NULL){
        free(str);
        return NULL;
    }

    strcpy_s(str->data, str->capacity, initial_str ? initial_str : "");

    return str;
}

String *new_string_arena(const char *initial_str, Arena *arena) {
    String *str = arena_allocate(arena, sizeof(String), alignof(String)); // Use arena_alloc
    if (!str) return NULL;  

    size_t length_of_initial_str = initial_str ? strlen(initial_str) : 0;
    str->data = arena_allocate(arena, length_of_initial_str + 1, alignof(char)); // Use arena_alloc for string data
    if (!str->data) {
        // Handle allocation failure for str->data (potentially reset arena and return NULL)
        arena_reset(arena); // This would undo the allocation of the String struct itself
        return NULL;
    }

    str->length = length_of_initial_str;
    str->capacity = length_of_initial_str + 1; 

    strcpy_s(str->data, str->capacity, initial_str ? initial_str : "");
    return str;
}

char string_char_at_index(const String *string, size_t index)
{
    if(index > string->length){
        return '\0';
    }

    return string->data[index];
}

ArenaError string_append_char_array_arena(String *dest, const char *src, Arena *arena) {
    
    size_t src_len = strlen(src);
    size_t new_length = dest->length + src_len;

    // Check if we need to grow the arena (only if using arena allocation)
    if (arena && new_length + 1 > dest->capacity) { // +1 for null terminator
        ArenaError grow_result = arena_grow(arena, new_length + 1 - dest->capacity);
        if (grow_result != ARENA_SUCCESS) {
            // Handle the error (e.g., report to the user)
            return grow_result;
        }

        // Update capacity after successful growth
        dest->capacity = arena->size - (dest->data - arena->start); // Calculate new capacity within the arena
    }

    // If the string is malloc'ed and needs to grow
    if (!arena && new_length + 1 > dest->capacity) {
        size_t new_capacity = dest->capacity * 2; 
        if (new_capacity < new_length + 1) {
            new_capacity = new_length + 1;
        }

        char *new_data = (char*)realloc(dest->data, new_capacity);
        if (!new_data) {
            // Handle allocation failure (e.g., return an error code)
            return ARENA_ERROR_REALLOCATION_FAILED;
        }
        dest->data = new_data;
        dest->capacity = new_capacity;
    }
    
    // Append source
    memcpy(dest->data + dest->length, src, src_len); // Use memcpy for efficiency
    dest->length = new_length;

    // Add null terminator
    dest->data[dest->length] = '\0';
    return ARENA_SUCCESS;
}

// Function to append a char array to a string struct
void string_append_char_array_malloc(String *dest, const char *src) {
    size_t src_len = strlen(src);

    // Ensure sufficient capacity
    if (dest->length + src_len + 1 > dest->capacity) { // +1 for the null terminator
        // Double the capacity (or increase it by some amount)
        size_t new_capacity = dest->capacity * 2;
        if (new_capacity < dest->length + src_len + 1) {
            new_capacity = dest->length + src_len + 1; 
        }
        
        // Reallocate memory
        char *new_data = (char*)realloc(dest->data, new_capacity);
        if (new_data == NULL) {
            // Memory allocation failed. Handle the error appropriately.
            // (e.g., print an error message, return an error code, etc.)
            return; 
        }
        dest->data = new_data;
        dest->capacity = new_capacity;
    }

    // Copy the source string
    strcpy_s(dest->data + dest->length, dest->capacity - dest->length, src);
    dest->length += src_len;

    // Add null terminator
    dest->data[dest->length] = '\0';
}

String* string_format(String *dest, Arena *arena, const char* format, ...) {
    va_list args;
    va_start(args, format);

    // Calculate the size of the formatted string (with some buffer)
    int size = vsnprintf(NULL, 0, format, args);
    va_end(args);
    if (size < 0) {
        return NULL; // Error occurred
    }

    // Handle String allocation depending on whether we are in arena mode or not
    String *result = dest;
    if(result == NULL){
        result = arena ? new_string_arena(NULL, arena) : new_string_malloc(NULL);
        if(!result) return NULL;
    }
    
    va_start(args, format);

    result->data = arena ? arena_allocate(arena, size + 1, alignof(char)) : (char*)realloc(result->data, size + 1);
    if (!result->data) {
        // Handle allocation failure (potentially reset arena and return NULL if in arena mode)
        va_end(args);
        if(arena){ 
            arena_reset(arena); 
        }
        else{
            free(result);
        }
        return NULL; 
    }

    result->capacity = size + 1;
    result->length = vsnprintf(result->data, result->capacity, format, args);

    va_end(args);
    return result;
}

void string_append_string_malloc(String *dest, const String *src)
{
    string_append_char_array_malloc(dest, src->data);
}

size_t string_length(String *string)
{
    return string->length;
}

void string_free(String *string)
{
    free(string->data);
    free(string);
}