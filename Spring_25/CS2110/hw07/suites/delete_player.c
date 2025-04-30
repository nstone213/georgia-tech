#include "../minecraft_v2.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_delete_player, .timeout = UNREASONABLY_LONG);

// 1. NULL args
Test(test_delete_player, null_world) {
    cr_assert(
        eq(type(Result), delete_player(NULL, "Steve"), FAILURE),
        "Should return FAILURE when NULL world passed"
    );
}

Test(test_delete_player, null_player_name) {
    World *new_world = h_create_world();

    cr_assert(
        eq(type(Result), delete_player(new_world, NULL), FAILURE),
        "Should return FAILURE when NULL player is passed"
    );
    h_free_world(new_world);
}

Test(test_delete_player, null_player_name_multiple_players) {
    World *new_world = h_create_world();

    h_create_player(new_world, "Steve", 15);
    h_create_player(new_world, "Alex", 20);

    cr_assert(
        eq(type(Result), delete_player(new_world, NULL), FAILURE),
        "Should return FAILURE when NULL player is passed"
    );

    cr_assert(eq(int, new_world->num_players, 2), "Expected player count to not change after failed delete");

    h_free_world(new_world);
}

// 2. Player not found

Test(test_delete_player, player_not_found) {
    World *new_world = h_create_world();

    h_create_player(new_world, "Alex", 10);
    h_create_player(new_world, "Herobrine", 100);
    h_create_player(new_world, "Steve", 7);
    h_create_player(new_world, "Steven", 18);

    cr_assert(
        eq(type(Result), delete_player(new_world, "Stev"), FAILURE),
        "Should return FAILURE when player does not exist in world"
    );

    cr_assert(eq(int, new_world->num_players, 4), "Expected player count to not change after failed delete");

    h_free_world(new_world);
}

// 3. Sucess Case

Test(test_delete_player, success_delete_player) {
    World *new_world = h_create_world();

    // Created: Alex, Herobrine, Steve, Steven
    h_create_player(new_world, "Alex", 10);
    h_create_player(new_world, "Herobrine", 100);
    h_create_player(new_world, "Steve", 7);
    h_create_player(new_world, "Steven", 18);

    // After Delete: Alex, Herobrine, Steven
    cr_assert(eq(type(Result), delete_player(new_world, "Steve"), SUCCESS), "Should return SUCCESS when player is deleted from world");
    cr_assert(eq(int, new_world->num_players, 3), "Expected player count to be correct after delete");
    cr_assert(eq(str, new_world->players[0]->name, "Alex"), "Expected player backing array to be correct after delete");
    cr_assert(eq(int, new_world->players[0]->level, 10), "Expected player backing array to be correct after delete");
    cr_assert(eq(str, new_world->players[1]->name, "Herobrine"), "Expected player backing array to be correct after delete");
    cr_assert(eq(int, new_world->players[1]->level, 100), "Expected player backing array to be correct after delete");
    cr_assert(eq(str, new_world->players[2]->name, "Steven"), "Expected player backing array to be correct after delete");
    cr_assert(eq(int, new_world->players[2]->level, 18), "Expected player backing array to be correct after delete");

    // After Delete: Alex, Herobrine
    cr_assert(eq(type(Result), delete_player(new_world, "Steven"), SUCCESS), "Should return SUCCESS when player is deleted from world");
    cr_assert(eq(int, new_world->num_players, 2), "Expected player count to change after delete");
    cr_assert(eq(str, new_world->players[0]->name, "Alex"), "Expected player backing array to be correct after delete");
    cr_assert(eq(int, new_world->players[0]->level, 10), "Expected player backing array to be correct after delete");
    cr_assert(eq(str, new_world->players[1]->name, "Herobrine"), "Expected player backing array to be correct after delete");
    cr_assert(eq(int, new_world->players[1]->level, 100), "Expected player backing array to be correct after delete");

    // After Delete: Herobrine
    cr_assert(eq(type(Result), delete_player(new_world, "Alex"), SUCCESS), "Should return SUCCESS when player is deleted from world");
    cr_assert(eq(int, new_world->num_players, 1), "Expected player count to change after delete");
    cr_assert(eq(str, new_world->players[0]->name, "Herobrine"), "Expected player backing array to be correct after delete");
    cr_assert(eq(int, new_world->players[0]->level, 100), "Expected player backing array to be correct after delete");

    cr_assert(eq(type(Result), delete_player(new_world, "Herobrine"), SUCCESS), "Should return SUCCESS when player is deleted from world");
    cr_assert(eq(int, new_world->num_players, 0), "Expected player count to change after delete");

    h_free_world(new_world);
}