#include "../minecraft_v2.h"
#include "ag_utils.h"
#include "fakemalloc.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_create_world, .timeout = UNREASONABLY_LONG);

Test(test_create_world, malloc_failure) {
    bytes_until_fail = sizeof(World) - 1;
    World *new_world = create_world();
    cr_assert(zero(ptr, new_world), "Expected to return NULL when malloc fails");
}

Test(test_create_world, success_world_created) {
    World *new_world = create_world();
    cr_assert(not(zero(ptr, new_world)), "Expected world to be created");

    cr_assert(eq(int, new_world->monsters.size, 0), "Expected monsters size to be 0");
    cr_assert(zero(ptr, new_world->monsters.head), "Expected monsters head to be NULL");
    cr_assert(zero(ptr, new_world->players), "Expected players array to be NULL");
    cr_assert(eq(int, new_world->num_players, 0), "Expected number of players to be 0");

    h_free_world(new_world);
}