#include "../minecraft.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdio.h>
#include <string.h>

TestSuite(test_create_player, .timeout = UNREASONABLY_LONG);

// Basic Player Addition
Test(test_create_player, basic) {
    // Add garbage data to world:
    memset(&world, 0x21, sizeof(world));
    world.num_players = 0;

    // Test `create_player`:
    cr_assert(
        eq(type(Result), create_player("Steve"), SUCCESS),
        "Expected creation of player 'Steve' to succeed"
    );
    cr_assert(eq(int, world.num_players, 1), "Expected number of players to be correct");
    cr_assert(eq(str, world.players[0].name, "Steve"), "Expected name to be correct");
    cr_assert(eq(int, world.players[0].level, 1), "Expected level to be correct");
    cr_assert(eq(int, world.players[0].health, PLAYER_INITIAL_HEALTH), "Expected health to be correct");
    cr_assert(eq(int, world.players[0].inventory_size, 0), "Expected inventory size to be empty");
}

// Adding Multiple Players Should be Contiguous
Test(test_create_player, multiple) {
    // Add garbage data to world:
    memset(&world, 0x10, sizeof(world));
    world.num_players = 0;

    // Test `create_player`:
    cr_assert(eq(type(Result), create_player("Steve"), SUCCESS), "Expected creation of player 'Steve' to succeed");
    cr_assert(eq(type(Result), create_player("Alex"), SUCCESS), "Expected creation of player 'Alex' to succeed");
    cr_assert(eq(type(Result), create_player("Villager"), SUCCESS), "Expected creation of player 'Villager' to succeed");

    cr_assert(eq(int, world.num_players, 3), "Expected number of players to be correct");

    cr_assert(eq(str, world.players[0].name, "Steve"), "Expected name to be correct at index 0");
    cr_assert(eq(int, world.players[0].level, 1), "Expected level to be correct at index 0");
    cr_assert(eq(int, world.players[0].health, PLAYER_INITIAL_HEALTH), "Expected health to be correct at index 0");
    cr_assert(eq(int, world.players[0].inventory_size, 0), "Expected inventory size to be empty at index 0");

    cr_assert(eq(str, world.players[1].name, "Alex"), "Expected name to be correct at index 1");
    cr_assert(eq(int, world.players[1].level, 1), "Expected level to be correct at index 1");
    cr_assert(eq(int, world.players[1].health, PLAYER_INITIAL_HEALTH), "Expected health to be correct at index 1");
    cr_assert(eq(int, world.players[1].inventory_size, 0), "Expected inventory size to be empty at index 1");

    cr_assert(eq(str, world.players[2].name, "Villager"), "Expected name to be correct at index 2");
    cr_assert(eq(int, world.players[2].level, 1), "Expected level to be correct at index 2");
    cr_assert(eq(int, world.players[2].health, PLAYER_INITIAL_HEALTH), "Expected health to be correct at index 2");
    cr_assert(eq(int, world.players[2].inventory_size, 0), "Expected inventory size to be empty at index 2");
}

// We should not allow adding a player when world is at capacity
Test(test_create_player, world_full) {
    // Initialize world:
    for (int i = 0; i < MAX_PLAYERS - 1; i++) {
        char player_name[10];
        snprintf(player_name, sizeof(player_name), "P%d", i);
        h_create_player(player_name);
    }

    // Test `create_player`:
    cr_assert(eq(type(Result), create_player("ExtraPlayer1"), SUCCESS), "Expected SUCCESS while world is almost full");
    cr_assert(eq(int, world.num_players, MAX_PLAYERS), "Expected number of players to increment after success");

    cr_assert(eq(type(Result), create_player("ExtraPlayer2"), FAILURE), "Expected FAILURE when world is full");
    cr_assert(eq(int, world.num_players, MAX_PLAYERS), "Expected number of players to stay unchanged after failure");
}

// Name Null Failure
Test(test_create_player, null_name) {
    // Initialize world:
    h_create_player("Foo");

    // Test `create_player`:
    cr_assert(
        eq(type(Result), create_player(NULL), FAILURE),
        "Expected FAILURE when name is NULL"
    );
    cr_assert(eq(int, world.num_players, 1), "Expected number of players to stay unchanged after failure");
}

// Name is Too Big Failure
Test(test_create_player, name_too_long) {
    // Initialize world:
    h_create_player("Bar");

    // Create name:
    char long_name[MAX_NAME_LENGTH + 1] = {0};
    memset(long_name, 'Z', sizeof(long_name) - 1);

    // Test `create_player`:
    cr_assert(
        eq(type(Result), create_player(long_name), FAILURE),
        "Expected FAILURE for overly long name"
    );
    cr_assert(eq(int, world.num_players, 1), "Expected number of players to stay unchanged after failure");
}

// No Duplicate Names Allowed
Test(test_create_player, duplicate_name) {
    // Initialize world:
    h_create_player("Alex");
    h_create_player("Steve");

    // Test `create_player`:
    cr_assert(eq(type(Result), create_player("Ste"), SUCCESS), "Expected SUCCESS when adding player 'Ste'");
    cr_assert(eq(int, world.num_players, 3), "Expected number of players to increment after success");

    cr_assert(eq(type(Result), create_player("Steve"), FAILURE), "Expected FAILURE when adding duplicate player");
    cr_assert(eq(int, world.num_players, 3), "Expected number of players to stay unchanged after failure");
}

Test(test_create_player, duplicate_name_oob) {
    // Initialize world:
    h_create_player("Alex");
    h_create_player("Steve");
    h_create_player("Foo");
    world.num_players--; // Delete player "Foo"

    // Test `create_player`:
    int result = create_player("Foo");
    cr_assert(eq(type(Result), result, SUCCESS), "Expected SUCCESS when adding player with name of deleted player");
    cr_assert(eq(int, world.num_players, 3), "Expected number of players to be correct");
    cr_assert(eq(str, world.players[2].name, "Foo"), "Expected name to be correct");
    cr_assert(eq(int, world.players[2].level, 1), "Expected level to be correct");
    cr_assert(eq(int, world.players[2].health, PLAYER_INITIAL_HEALTH), "Expected health to be correct");
    cr_assert(eq(int, world.players[2].inventory_size, 0), "Expected inventory size to be empty");
}