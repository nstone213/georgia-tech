#ifndef AG_UTILS_H
#define AG_UTILS_H

#include "../minecraft.h"
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

extern struct World world;

// These are helper functions for the autograder.
// You may not use these for your solution.
typedef int Result;
char *cr_user_Result_tostr(Result *value);
int cr_user_Result_eq(Result *a, Result *b);

int h_is_numeric(const char *string);
void h_reverse_string(char *string);

void h_add_item(struct Player *player, const char *weapon, int level);
void h_create_player(const char *name);
void h_add_player_to_world(struct Player player);
void h_spawn_monster(const char *mob, int level);

#endif