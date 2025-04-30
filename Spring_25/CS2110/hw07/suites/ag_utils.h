#ifndef AG_UTILS_H
#define AG_UTILS_H

#include "../minecraft_v2.h"
#include "../my_malloc.h"
#include <stdbool.h>

// The Makefile script + Criterion bugs are causing weird interactions.
// `make run-tests` cannot be ^C'd (it has to be ^Z'd).
//
// As such, we need to impose a timeout so that tests will eventually stop
// in cases of infinite loops.
//
// However, we do not want to impose a timeout when debugging with GDB
// (since that wouldn't be a fun debugging experience).
//
// One way of doing this is defining ./tests --timeout N in make run-tests;
// however, this requires specifying a non-default timeout (because Criterion bug).
// You can't set +INFINITY because +INFINITY (also due to a bug) acts as a 0s timeout.
#define UNREASONABLY_LONG 86400

// These are helper functions for the autograder.
// You may not use these for your solution.
typedef int Result;
char *cr_user_Result_tostr(Result *value);
int cr_user_Result_eq(Result *a, Result *b);

typedef enum Mob mob_t;
char *cr_user_mob_t_tostr(mob_t *value);
int cr_user_mob_t_eq(mob_t *a, mob_t *b);

World *h_create_world(void);
Player *h_create_player(World *world, const char *username, int level);
Monster *h_create_monster(World *world, enum Mob mob, int level);
void h_free_player(Player *player);
void h_free_world(World *world);

// MALLOC IMPL:
#define HEAP_SIZE 0x2000

/* Used to access the address and size freelists */
extern metadata_t *address_list;
// extern metadata_t *size_list;

/*These variables are explained in malloc_suite*/
extern metadata_t *A, *B, *C;
extern bool my_sbrk_called;
extern bool my_sbrk_call_expected;
extern uint8_t *my_sbrk_fake_heap;
extern int my_sbrk_imaginary_prev_calls;

#define A_START_OFF 512
#define A_START_SIZE 256
#define B_START_OFF 928
#define B_START_SIZE 64
#define C_START_OFF 1216
#define C_START_SIZE (SBRK_SIZE - C_START_OFF - TOTAL_METADATA_SIZE)
#define D_START_OFF 32
#define D_START_SIZE 128
#define E_START_SIZE 64
#define E_START_OFF (SBRK_SIZE - E_START_SIZE - TOTAL_METADATA_SIZE)

typedef enum my_malloc_err my_malloc_err_t;
char *cr_user_my_malloc_err_t_tostr(my_malloc_err_t *value);
int cr_user_my_malloc_err_t_eq(my_malloc_err_t *a, my_malloc_err_t *b);

/*These functions are defined/explained in malloc_suite*/
void setup_heap(void);
void teardown_heap(void);
void *my_sbrk(int increment);
void init_malloc_test(int prev_sbrk_calls, bool should_sbrk);
void create_situation(metadata_t **Aout, metadata_t **Bout, metadata_t **Cout);
#endif