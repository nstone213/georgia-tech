// poison <string.h>, dynamic allocation, <ctype.h>, <dlfcn.h>, system/exec
#pragma GCC poison memcpy memmove memccpy memset memcmp memchr rawmemchr memrchr strcpy strncpy strcat strncat strcmp strncmp strcoll strxfrm strcoll_l strxfrm_l strdup strndup strchr strrchr strchrnul strcspn strpbrk strstr strtok __strtok_r strtok_r strcasestr memmem __mempcpy mempcpy strlen strnlen strerror __xpg_strerror_r strerror_r strerror_l explicit_bzero strsep strsignal __stpcpy stpcpy __stpncpy stpncpy strverscmp strfry memfrob basename malloc calloc realloc free valloc aligned_alloc isalnum isalpha islower isupper isdigit isxdigit iscntrl isgraph isspace isblank isprint ispunct tolower toupper dlclose dlerror dlopen dlsym system execl execle execlp execv execve execvp fexecve

// Remove function prototypes from ag_utils.h.
// This creates a compiler error if unauthorized
// access to ag_utils.h is attempted.
//
// This doesn't patch authorized access completely;
// but it causes a compile error rather than a linker error.
#define AG_UTILS_H