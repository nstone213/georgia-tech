#include "../minecraft_v2.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_find_player, .timeout = UNREASONABLY_LONG);

Test(test_find_player, player_exists) {
    World *world = h_create_world();
    Player *p1 = h_create_player(world, "Steve", 10);
    Player *p2 = h_create_player(world, "Alex", 15);

    Player *result2 = find_player(world, "Alex");
    Player *result1 = find_player(world, "Steve");

    cr_assert(eq(ptr, result2, p2), "Expected to find player Alex");
    cr_assert(eq(ptr, result1, p1), "Expected to find player Steve");

    h_free_world(world);
}

Test(test_find_player, player_not_found) {
    World *world = h_create_world();
    h_create_player(world, "Steve", 10);

    Player *result = find_player(world, "Herobrine");
    cr_assert(zero(ptr, result), "Expected NULL when player does not exist");

    h_free_world(world);
}

Test(test_find_player, empty_world) {
    World *world = h_create_world();

    Player *result = find_player(world, "Steve");
    cr_assert(zero(ptr, result), "Expected NULL when world has no players");

    h_free_world(world);
}

Test(test_find_player, null_world) {
    Player *result = find_player(NULL, "Steve");
    cr_assert(zero(ptr, result), "Expected NULL when world is NULL");
}

Test(test_find_player, null_name) {
    World *world = h_create_world();
    h_create_player(world, "Steve", 10);

    Player *result = find_player(world, NULL);
    cr_assert(zero(ptr, result), "Expected NULL when name is NULL");

    h_free_world(world);
}
