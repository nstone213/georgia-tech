#include "../minecraft.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_fight_ender_dragon, .timeout = UNREASONABLY_LONG);

// 1. Null player check
Test(test_fight_ender_dragon, null_player) {
    cr_assert(
        eq(type(Result), fight_ender_dragon(NULL), FAILURE),
        "Null player passed in, should return FAILURE\n"
    );
}

// 2. Player level check
Test(test_fight_ender_dragon, weak_player) {
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 99,
        .health = PLAYER_INITIAL_HEALTH,
        .inventory = {
            {.weapon = "Bow", .level = 1000},
            {.weapon = "Ender Pearl", .level = 200},
            {.weapon = "Sword", .level = 310},
        },
        .inventory_size = 3
    });

    cr_assert(
        eq(type(Result), fight_ender_dragon(&world.players[0]), FAILURE),
        "Player is too weak to fight the Ender Dragon, should return FAILURE"
    );
    cr_assert(eq(int, world.num_players, 1), "Player should still exist");
    cr_assert(eq(int, world.players[0].level, 99), "Player's level should have stayed the same");
    cr_assert(eq(int, world.players[0].health, PLAYER_INITIAL_HEALTH), "Player's health should have stayed the same");
}

// Win Scenario Checks
Test(test_fight_ender_dragon, player_wins) {
    // Initialize player and world:
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 200,
        .health = PLAYER_INITIAL_HEALTH,
        // Inventory total (1510) > ENDER_DRAGON_HEALTH (1500)
        .inventory = {
            {.weapon = "Bow", .level = 1000},
            {.weapon = "Ender Pearl", .level = 200},
            {.weapon = "Sword", .level = 310},
        },
        .inventory_size = 3
    });

    // Test `fight_ender_dragon`:
    cr_assert(eq(type(Result), fight_ender_dragon(&world.players[0]), SUCCESS), "Player beat Ender Dragon, should return SUCCESS");
    cr_assert(eq(int, world.num_players, 1), "Player should still exist after defeating Ender Dragon");
    cr_assert(eq(int, world.players[0].level, 225), "Player's level should have increased");
    cr_assert(eq(int, world.players[0].health, 50), "Player's health should have been restored");
}

// Loss Scenario Checks
Test(test_fight_ender_dragon, ender_dragon_wins) {
    // Initialize player and world:
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 200,
        .health = PLAYER_INITIAL_HEALTH,
        // Inventory total (1200) < ENDER_DRAGON_HEALTH (1500)
        .inventory = {
            {.weapon = "Bow", .level = 1000},
            {.weapon = "Ender Pearl", .level = 200},
            // This weapon is past the bounds of the array
            // and should not be accessed.
            {.weapon = "Sword", .level = 1500}
        },
        .inventory_size = 2
    });

    // Test `fight_ender_dragon`:
    cr_assert(eq(type(Result), fight_ender_dragon(&world.players[0]), FAILURE), "Ender Dragon beat the player, should return FAILURE");
    cr_assert(eq(int, world.num_players, 0), "Player should be deleted from the world after losing");
}

Test(test_fight_ender_dragon, ender_dragon_wins_eq) {
    // Initialize player and world:
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 200,
        .health = PLAYER_INITIAL_HEALTH,
        // Inventory total (1500) == ENDER_DRAGON_HEALTH (1500)
        .inventory = {
            {.weapon = "Bow", .level = 1000},
            {.weapon = "Ender Pearl", .level = 500},
        },
        .inventory_size = 2
    });

    // Test `fight_ender_dragon`:
    cr_assert(eq(type(Result), fight_ender_dragon(&world.players[0]), FAILURE), "Ender Dragon beat the player, should return FAILURE");
    cr_assert(eq(int, world.num_players, 0), "Player should be deleted from the world after losing");
}
