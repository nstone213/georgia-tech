/**
 * @file my_string.c
 * @author Nicholas Stone
 * @brief Your implementation of the famous string.h library functions!
 *
 * NOTE: NO ARRAY NOTATION IS ALLOWED IN THIS FILE
 *
 * @date 2025-03-23
 */

#include "my_string.h"

/* Note about UNUSED_PARAM
 *
 * UNUSED_PARAM is used to avoid compiler warnings and errors regarding unused function
 * parameters prior to implementing the function. Once you begin implementing this
 * function, you can delete the UNUSED_PARAM lines.
 */

/**
 * @brief Calculate the length of a string
 *
 * @param s a constant C string
 * @return size_t the number of characters in the passed in string
 */
size_t my_strlen(const char *s) {
    const char *ptr = s;

    while (*ptr != '\0') {
        ptr++;
    }

    return ptr - s;
}

/**
 * @brief Compare two strings
 *
 * @param s1 First string to be compared
 * @param s2 Second string to be compared
 * @param n First (at most) n bytes to be compared
 * @return int representing the difference between the strings:
 *          - 0 if the strings are equal
 *          - arbitrary positive number if s1 > s2
 *          - arbitrary negative number if s1 < s2
 */
int my_strncmp(const char *s1, const char *s2, size_t n) {
    if (n == 0) {
        return 0;
    }

    while (n > 0) {
        if (*s1 != *s2) {
            return *s1 - *s2;
        }

        if (*s1 == '\0') {
            return 0;
        }
 
        s1++;
        s2++;
        n--;
    }

    return 0;
}

/**
 * @brief Copy a string
 *
 * @param dest The destination buffer
 * @param src The source to copy from
 * @param n maximum number of bytes to copy
 * @return char* a pointer same as dest
 */
char *my_strncpy(char *dest, const char *src, size_t n) {
    char *ptr = dest;

    while (n > 0) {
        if (*src != '\0') {
            *ptr = *src;
            src++;
        } else {
            *ptr = '\0';
        }

        ptr++;
        n--;
    }

    return dest;
}

/**
 * @brief Concatenates two strings and stores the result
 * in the destination string
 *
 * @param dest The destination string
 * @param src The source string
 * @param n The maximum number of bytes (or characters) from src to concatenate
 * @return char* a pointer same as dest
 */
char *my_strncat(char *dest, const char *src, size_t n) {
    char *ptr = dest;

    while (*ptr != '\0') {
        ptr++;
    }

    while (n > 0 && *src != '\0') {
        *ptr = *src;
        ptr++;
        src++;
        n--;
    }
    *ptr = '\0';
    
    return dest;
}

/**
 * @brief Copies the character c into the first n
 * bytes of memory starting at *str
 *
 * @param str The pointer to the block of memory to fill
 * @param c The character to fill in memory
 * @param n The number of bytes of memory to fill
 * @return char* a pointer same as str
 */
void *my_memset(void *str, int c, size_t n) {
    unsigned char *ptr = (unsigned char *)str;
    unsigned char val = (unsigned char)c;

    while (n > 0) {
        *ptr = val;
        ptr++;
        n--;
    }

    return str;
}

/**
 * @brief Write a function that reverses a
 * string in-place without using additional arrays.
 *
 * @param str The pointer to the string
 */
void reverse_string(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    char *start = str;
    char *end = str;
    
    while (*(end + 1) != '\0') {
        end++;
    }

    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}

/**
 * @brief Implement a function that checks
 * whether a given string consists only of
 * numeric characters (0-9).
 *
 * @param str The pointer to the string
 * @return 1 if the string is numeric, 0 otherwise
 */
int is_numeric(const char *str) {
    if (str == NULL) {
        return 0;
    }

    if (*str == '\0') {
        return 1;
    }

    while (*str != '\0') {
        if (*str < '0' || *str > '9') {
            return 0;
        }
        str++;
    }

    return 1;
}