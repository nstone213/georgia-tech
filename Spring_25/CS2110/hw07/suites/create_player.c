#include "../minecraft_v2.h"
#include "ag_utils.h"
#include "fakemalloc.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_create_player, .timeout = UNREASONABLY_LONG);

// 1. Incorrect arg cases

// Null World
Test(test_create_player, null_world) {
    cr_assert(
        eq(type(Result), create_player(NULL, "Steve", 10), FAILURE),
        "Should return FAILURE when NULL world passed"
    );
}

// Null Name
Test(test_create_player, null_name) {
    World *new_world = h_create_world();
    cr_assert(
        eq(type(Result), create_player(new_world, NULL, 0), FAILURE),
        "Should return FAILURE when NULL name is passed"
    );
    h_free_world(new_world);
}

// level == 0
Test(test_create_player, zero_level) {
    World *new_world = h_create_world();
    cr_assert(
        eq(type(Result), create_player(new_world, "Henry", 0), FAILURE),
        "Should return FAILURE when zero level is passed"
    );
    h_free_world(new_world);
}

// level < 0
Test(test_create_player, negative_level) {
    World *new_world = h_create_world();
    cr_assert(
        eq(type(Result), create_player(new_world, "Owen", -100), FAILURE),
        "Should return FAILURE when negative level is passed"
    );
    h_free_world(new_world);
}

// duplicate name
Test(test_create_player, duplicate_name) {
    World *new_world = h_create_world();

    h_create_player(new_world, "Steve", 10);
    h_create_player(new_world, "Alex", 10);

    cr_assert(eq(type(Result), create_player(new_world, "Steven", 12), SUCCESS), "Expected SUCCESS when adding player");
    cr_assert(eq(int, new_world->num_players, 3), "Expected number of players to increment after success");

    cr_assert(eq(type(Result), create_player(new_world, "Steve", 20), FAILURE), "Expected FAILURE when adding duplicate player");
    cr_assert(eq(str, new_world->players[0]->name, "Steve"), "Expected backing array to be correct after failure");
    cr_assert(eq(int, new_world->players[0]->level, 10), "Expected backing array to be correct after failure");
    cr_assert(eq(str, new_world->players[1]->name, "Alex"), "Expected backing array to be correct after failure");
    cr_assert(eq(int, new_world->players[1]->level, 10), "Expected backing array to be correct after failure");
    cr_assert(eq(str, new_world->players[2]->name, "Steven"), "Expected backing array to be correct after failure");
    cr_assert(eq(int, new_world->players[2]->level, 12), "Expected backing array to be correct after failure");
    cr_assert(eq(int, new_world->num_players, 3), "Expected number of players to be correct after failure");

    h_free_world(new_world);
}

// 2. Malloc failure
Test(test_create_player, malloc_failure_single_player) {
    World *new_world = h_create_world();

    bytes_until_fail = sizeof(Player) + 4;
    cr_assert(eq(type(Result), create_player(new_world, "Alex", 100), FAILURE), "malloc failing should result in FAILURE");
    cr_assert(eq(i32, new_world->num_players, 0), "Expected number of players should not change after malloc failure");
    h_free_world(new_world);
}

// 3. Resize failure
Test(test_create_player, realloc_failure) {
    World *new_world = h_create_world();
    h_create_player(new_world, "Henry", 100);
    h_create_player(new_world, "Bianca", 30);
    bytes_until_fail = (sizeof(Player)) + 4;
    cr_assert(eq(type(Result), create_player(new_world, "Owen", 45), FAILURE), "malloc failing should result in FAILURE");
    cr_assert(eq(i32, new_world->num_players, 2), "Expected number of players to remain after malloc failure");
    h_free_world(new_world);
}

// 4. SUCCESS cases
Test(test_create_player, basic) {
    World *new_world = h_create_world();

    // Test `create_player`:
    cr_assert(
        eq(type(Result), create_player(new_world, "Steve", 30), SUCCESS),
        "Expected creation of player 'Steve' to succeed"
    );
    cr_assert(eq(i32, new_world->num_players, 1), "Expected world player count to be correct");
    cr_assert(eq(str, new_world->players[0]->name, "Steve"), "Expected created player's name to be correct");
    cr_assert(eq(int, new_world->players[0]->level, 30), "Expected player level to be correct");

    h_free_world(new_world);
}

Test(test_create_player, success_multiple_players) {
    World *new_world = h_create_world();
    create_player(new_world, "Steve", 10);
    cr_assert(eq(i32, new_world->num_players, 1), "Expected world player count to be correct");
    cr_assert(eq(str, new_world->players[0]->name, "Steve"), "Expected created player's name to be correct");
    cr_assert(eq(int, new_world->players[0]->level, 10), "Expected player level to be correct");

    create_player(new_world, "Villager", 17);
    cr_assert(eq(i32, new_world->num_players, 2), "Expected world player count to be correct");
    cr_assert(eq(str, new_world->players[1]->name, "Villager"), "Expected created player's name to be correct");
    cr_assert(eq(int, new_world->players[1]->level, 17), "Expected player level to be correct");

    create_player(new_world, "Minecrafter", 1);
    cr_assert(eq(i32, new_world->num_players, 3), "Expected world player count to be correct");
    cr_assert(eq(str, new_world->players[2]->name, "Minecrafter"), "Expected created player's name to be correct");
    cr_assert(eq(int, new_world->players[2]->level, 1), "Expected player level to be correct");
    h_free_world(new_world);
}