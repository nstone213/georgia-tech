// poison <dlfcn.h>, exec
#pragma GCC poison dlclose dlerror dlopen dlsym execl execle execlp execv execve execvp fexecve

// Remove function prototypes from ag_utils.h.
// This creates a compiler error if unauthorized
// access to ag_utils.h is attempted.
//
// This doesn't patch authorized access completely;
// but it causes a compile error rather than a linker error.
#define AG_UTILS_H