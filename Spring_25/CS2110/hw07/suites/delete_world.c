#include "../minecraft_v2.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_delete_world, .timeout = UNREASONABLY_LONG);

Test(test_delete_world, null_world_pointer_returns_failure) {
    cr_assert(eq(type(Result), delete_world(NULL), FAILURE), "Expected FAILURE when deleting NULL world");
}

Test(test_delete_world, empty_world_returns_success) {
    World *world = h_create_world();
    cr_assert(eq(type(Result), delete_world(world), SUCCESS), "Expected SUCCESS when deleting empty world");
}

Test(test_delete_world, world_with_players_only) {
    World *world = h_create_world();
    h_create_player(world, "Alex", 10);
    h_create_player(world, "Steve", 7);
    cr_assert(eq(type(Result), delete_world(world), SUCCESS), "Expected SUCCESS when deleting world with players only");
}

Test(test_delete_world, world_with_monsters_only) {
    World *world = h_create_world();
    h_create_monster(world, ZOMBIE, 5);
    h_create_monster(world, SKELETON, 9);
    cr_assert(eq(type(Result), delete_world(world), SUCCESS), "Expected SUCCESS when deleting world with monsters only");
}

Test(test_delete_world, world_with_players_and_monsters) {
    World *world = h_create_world();
    h_create_player(world, "Alex", 10);
    h_create_monster(world, CREEPER, 4);
    cr_assert(eq(type(Result), delete_world(world), SUCCESS), "Expected SUCCESS when deleting world with both players and monsters");
}