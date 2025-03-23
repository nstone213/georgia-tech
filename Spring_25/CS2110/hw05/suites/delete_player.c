#include "../minecraft.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_delete_player, .timeout = UNREASONABLY_LONG);

// 1. Null checks

Test(test_delete_player, null_player_no_players) {
    world.num_players = 0;

    cr_assert(
        eq(type(Result), delete_player(NULL), FAILURE),
        "Deleting NULL player should return FAILURE."
    );
    cr_assert(
        eq(int, world.num_players, 0),
        "Deleting NULL player should keep the initial amount of players in world."
    );
}

Test(test_delete_player, null_player_with_other_players) {
    h_create_player("Henry");
    h_create_player("Andy");
    h_create_player("Owen");
    h_create_player("Willson");

    cr_assert(
        eq(type(Result), delete_player(NULL), FAILURE),
        "Deleting NULL player should return FAILURE."
    );

    cr_assert(
        eq(int, world.num_players, 4),
        "Number of players in world should remain the same after failure."
    );
}

// 2. Delete non-existent player
Test(test_delete_player, delete_nonexistent_player) {
    h_create_player("Henry");
    h_create_player("Andy");
    h_create_player("Owen");

    cr_assert(
        eq(type(Result), delete_player("Willson"), FAILURE),
        "Cannot delete player that does not exist in world"
    );

    cr_assert(
        eq(int, world.num_players, 3),
        "Number of players in world should remain the same after failure."
    );
}

Test(test_delete_player, delete_existing_player) {
    // Init players in world
    struct Player p0 = {
        .name = "Henry",
        .level = 0x59,
        .health = 0x45,
        .inventory = {
            {.weapon = "Sword", .level = 1},
            {.weapon = "Bow", .level = 6}
        },
        .inventory_size = 2
    };
    struct Player p1 = {
        .name = "Andy",
        .level = 0x45,
        .health = 0x54,
        .inventory = {
            {.weapon = "Bow", .level = 2},
            {.weapon = "Ender Pearl", .level = 7}
        },
        .inventory_size = 2
    };
    struct Player p2 = {
        .name = "Owen",
        .level = 0x21,
        .health = 0x59,
        .inventory = {
            {.weapon = "Ender Pearl", .level = 3},
            {.weapon = "Sword", .level = 8}
        },
        .inventory_size = 2
    };
    struct Player p3 = {
        .name = "Willson",
        .level = 0x4f,
        .health = 0x49,
        .inventory = {
            {.weapon = "Sword", .level = 4},
            {.weapon = "Bow", .level = 9}
        },
        .inventory_size = 2
    };
    struct Player p4 = {
        .name = "Bianca",
        .level = 0x4e,
        .health = 0x4b,
        .inventory = {
            {.weapon = "Bow", .level = 5},
            {.weapon = "Sword", .level = 10}
        },
        .inventory_size = 2
    };
    h_add_player_to_world(p0);
    h_add_player_to_world(p1);
    h_add_player_to_world(p2);
    h_add_player_to_world(p3);
    h_add_player_to_world(p4);

    // --- delete first player ---
    cr_assert(
        eq(type(Result), delete_player(p3.name), SUCCESS),
        "Player should be deleted successfully"
    );
    cr_assert(eq(int, world.num_players, 4), "Number of players in world should have been decremented");

    // Contiguous order check
    // This is very beefy.
    cr_assert(eq(str, world.players[0].name, p0.name), "Expected correct player to be in world after deletion");
    cr_assert(eq(int, world.players[0].level, p0.level), "Expected player's level to be the same after deletion");
    cr_assert(eq(int, world.players[0].health, p0.health), "Expected player's health to be the same after deletion");
    cr_assert(eq(int, world.players[0].inventory_size, p0.inventory_size), "Expected player's inventory size to be the same after deletion");
    cr_assert(eq(str, world.players[0].inventory[0].weapon, p0.inventory[0].weapon), "Expected player's item weapon at index 0 to be the same after deletion");
    cr_assert(eq(int, world.players[0].inventory[0].level, p0.inventory[0].level), "Expected player's item level at index 0 to be the same after deletion");
    cr_assert(eq(str, world.players[0].inventory[1].weapon, p0.inventory[1].weapon), "Expected player's item weapon at index 1 to be the same after deletion");
    cr_assert(eq(int, world.players[0].inventory[1].level, p0.inventory[1].level), "Expected player's item level at index 1 to be the same after deletion");

    cr_assert(eq(str, world.players[1].name, p1.name), "Expected correct player to be in world after deletion");
    cr_assert(eq(int, world.players[1].level, p1.level), "Expected player's level to be the same after deletion");
    cr_assert(eq(int, world.players[1].health, p1.health), "Expected player's health to be the same after deletion");
    cr_assert(eq(int, world.players[1].inventory_size, p1.inventory_size), "Expected player's inventory size to be the same after deletion");
    cr_assert(eq(str, world.players[1].inventory[0].weapon, p1.inventory[0].weapon), "Expected player's item weapon at index 0 to be the same after deletion");
    cr_assert(eq(int, world.players[1].inventory[0].level, p1.inventory[0].level), "Expected player's item level at index 0 to be the same after deletion");
    cr_assert(eq(str, world.players[1].inventory[1].weapon, p1.inventory[1].weapon), "Expected player's item weapon at index 1 to be the same after deletion");
    cr_assert(eq(int, world.players[1].inventory[1].level, p1.inventory[1].level), "Expected player's item level at index 1 to be the same after deletion");

    cr_assert(eq(str, world.players[2].name, p2.name), "Expected correct player to be in world after deletion");
    cr_assert(eq(int, world.players[2].level, p2.level), "Expected player's level to be the same after deletion");
    cr_assert(eq(int, world.players[2].health, p2.health), "Expected player's health to be the same after deletion");
    cr_assert(eq(int, world.players[2].inventory_size, p2.inventory_size), "Expected player's inventory size to be the same after deletion");
    cr_assert(eq(str, world.players[2].inventory[0].weapon, p2.inventory[0].weapon), "Expected player's item weapon at index 0 to be the same after deletion");
    cr_assert(eq(int, world.players[2].inventory[0].level, p2.inventory[0].level), "Expected player's item level at index 0 to be the same after deletion");
    cr_assert(eq(str, world.players[2].inventory[1].weapon, p2.inventory[1].weapon), "Expected player's item weapon at index 1 to be the same after deletion");
    cr_assert(eq(int, world.players[2].inventory[1].level, p2.inventory[1].level), "Expected player's item level at index 1 to be the same after deletion");

    cr_assert(eq(str, world.players[3].name, p4.name), "Expected correct player to be in world after deletion");
    cr_assert(eq(int, world.players[3].level, p4.level), "Expected player's level to be the same after deletion");
    cr_assert(eq(int, world.players[3].health, p4.health), "Expected player's health to be the same after deletion");
    cr_assert(eq(int, world.players[3].inventory_size, p4.inventory_size), "Expected player's inventory size to be the same after deletion");
    cr_assert(eq(str, world.players[3].inventory[0].weapon, p4.inventory[0].weapon), "Expected player's item weapon at index 0 to be the same after deletion");
    cr_assert(eq(int, world.players[3].inventory[0].level, p4.inventory[0].level), "Expected player's item level at index 0 to be the same after deletion");
    cr_assert(eq(str, world.players[3].inventory[1].weapon, p4.inventory[1].weapon), "Expected player's item weapon at index 1 to be the same after deletion");
    cr_assert(eq(int, world.players[3].inventory[1].level, p4.inventory[1].level), "Expected player's item level at index 1 to be the same after deletion");

    // --- delete second player ---
    cr_assert(
        eq(type(Result), delete_player("Owen"), SUCCESS),
        "Player should be deleted successfully"
    );
    cr_assert(eq(int, world.num_players, 3), "Number of players in world should have been decremented");

    // Contiguous order check
    cr_assert(eq(str, world.players[0].name, "Henry"), "Expected correct players to be in world after deletion");
    cr_assert(eq(str, world.players[1].name, "Andy"), "Expected correct players to be in world after deletion");
    cr_assert(eq(str, world.players[2].name, "Bianca"), "Expected correct players to be in world after deletion");

    // --- delete third player ---
    cr_assert(
        eq(type(Result), delete_player("Henry"), SUCCESS),
        "Player should be deleted successfully"
    );
    cr_assert(eq(int, world.num_players, 2), "Number of players in world should have been decremented");

    // Contiguous order check
    cr_assert(eq(str, world.players[0].name, "Andy"), "Expected correct players to be in world after deletion");
    cr_assert(eq(str, world.players[1].name, "Bianca"), "Expected correct players to be in world after deletion");

    // --- delete fourth player ---
    cr_assert(
        eq(type(Result), delete_player("Bianca"), SUCCESS),
        "Player should be deleted successfully"
    );
    cr_assert(eq(int, world.num_players, 1), "Number of players in world should have been decremented");

    // Contiguous order check
    cr_assert(eq(str, world.players[0].name, "Andy"), "Expected correct players to be in world after deletion");

    // --- delete fifth player ---
    cr_assert(
        eq(type(Result), delete_player("Andy"), SUCCESS),
        "Player should be deleted successfully"
    );

    // empty world delete check
    cr_assert(eq(int, world.num_players, 0), "Number of players in world should have been decremented");
    cr_assert(eq(type(Result), delete_player("Henry"), FAILURE), "Player cannot be deleted from an empty world");
    cr_assert(eq(int, world.num_players, 0), "Number of players in world should not change when non-existent player is deleted");
}
