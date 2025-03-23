#include "../minecraft.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_add_item, .timeout = UNREASONABLY_LONG);

// Null Player Check
Test(test_add_item, null_player) {
    cr_assert(
        eq(type(Result), add_item(NULL, "Sword", 5), FAILURE),
        "Expected FAILURE when player is NULL"
    );
}

// Full Inventory Check
Test(test_add_item, inventory_full) {
    // Initialize player:
    struct Player player = {
        .name = "Alex",
        .level = 3,
        .health = PLAYER_INITIAL_HEALTH
    };
    for (int i = 0; i < MAX_INVENTORY_SIZE; i++) {
        h_add_item(&player, "Bow", i + 1);
    }

    // Test `add_item`:
    cr_assert(
        eq(type(Result), add_item(&player, "Sword", 10), FAILURE),
        "Expected FAILURE when inventory is full"
    );
}

// Null Weapon Name Check
Test(test_add_item, null_weapon) {
    // Initialize player:
    struct Player player = {
        .name = "Steve",
        .level = 2,
        .health = PLAYER_INITIAL_HEALTH
    };

    // Test `add_item`:
    cr_assert(
        eq(type(Result), add_item(&player, NULL, 5), FAILURE),
        "Expected FAILURE when weapon name is NULL"
    );
}

// Adding Invalid Weapon Name Check
Test(test_add_item, invalid_weapon) {
    // Initialize player:
    struct Player player = {
        .name = "Steve",
        .level = 2,
        .health = PLAYER_INITIAL_HEALTH
    };

    // Test `add_item`:
    cr_assert(
        eq(type(Result), add_item(&player, "Excalibur", 5), FAILURE),
        "Expected FAILURE when weapon name is invalid"
    );
}

// Invalid Weapon level Check
Test(test_add_item, invalid_level) {
    // Initialize player:
    struct Player player = {
        .name = "Steve",
        .level = 2,
        .health = PLAYER_INITIAL_HEALTH
    };

    // Test `add_item`:
    cr_assert(
        eq(type(Result), add_item(&player, "Bow", 0), FAILURE),
        "Expected FAILURE when weapon level < 1"
    );
}

// Add Sword
Test(test_add_item, sword_success) {
    // Initialize player:
    struct Player player = {
        .name = "Steve",
        .level = 2,
        .health = PLAYER_INITIAL_HEALTH
    };

    // Test `add_item`:
    int result = add_item(&player, "Sword", 3);
    cr_assert(eq(type(Result), result, SUCCESS), "Adding 'Sword' to player's inventory should have succeeded");
    cr_assert(eq(int, player.inventory_size, 1), "Expected inventory size to be correct");
    cr_assert(eq(str, player.inventory[0].weapon, "Sword"), "Expected weapon to be correct");
    cr_assert(eq(int, player.inventory[0].level, 3), "Expected level to be correct");
}

// Add Bow
Test(test_add_item, bow_success) {
    // Initialize player:
    struct Player player = {
        .name = "Steve",
        .level = 2,
        .health = PLAYER_INITIAL_HEALTH
    };

    // Test `add_item`:
    int result = add_item(&player, "Bow", 10);
    cr_assert(eq(type(Result), result, SUCCESS), "Adding 'Bow' to player's inventory should have succeeded");
    cr_assert(eq(int, player.inventory_size, 1), "Expected inventory size to be correct");
    cr_assert(eq(str, player.inventory[0].weapon, "Bow"), "Expected weapon to be correct");
    cr_assert(eq(int, player.inventory[0].level, 10), "Expected level to be correct");
}

// Add Ender Pearl
Test(test_add_item, ender_pearl_success) {
    // Initialize player:
    struct Player player = {
        .name = "Steve",
        .level = 2,
        .health = PLAYER_INITIAL_HEALTH
    };

    // Test `add_item`:
    int result = add_item(&player, "Ender Pearl", 10);
    cr_assert(eq(type(Result), result, SUCCESS), "Adding 'Ender Pearl' to player's inventory should have succeeded");
    cr_assert(eq(int, player.inventory_size, 1), "Expected inventory size to be correct");
    cr_assert(eq(str, player.inventory[0].weapon, "Ender Pearl"), "Expected weapon to be correct");
    cr_assert(eq(int, player.inventory[0].level, 10), "Expected level to be correct");
}

// Add Multiple Items
Test(test_add_item, multiple_items) {
    // Initialize player:
    struct Player player = {
        .name = "Alex",
        .level = 2,
        .health = PLAYER_INITIAL_HEALTH
    };

    cr_assert(eq(type(Result), add_item(&player, "Sword", 4), SUCCESS), "Adding 'Sword' to player's inventory should have succeeded");
    cr_assert(eq(type(Result), add_item(&player, "Bow", 7), SUCCESS), "Adding 'Bow' to player's inventory should have succeeded");
    cr_assert(eq(type(Result), add_item(&player, "Ender Pearl", 5), SUCCESS), "Adding 'Ender Pearl' to player's inventory should have succeeded");
    cr_assert(eq(int, player.inventory_size, 3), "Expected inventory size to be correct");
    cr_assert(eq(str, player.inventory[0].weapon, "Sword"), "Expected weapon at index 0 to be correct");
    cr_assert(eq(int, player.inventory[0].level, 4), "Expected item level at index 0 to be correct");
    cr_assert(eq(str, player.inventory[1].weapon, "Bow"), "Expected weapon at index 1 to be correct");
    cr_assert(eq(int, player.inventory[1].level, 7), "Expected item level at index 1 to be correct");
    cr_assert(eq(str, player.inventory[2].weapon, "Ender Pearl"), "Expected weapon at index 2 to be correct");
    cr_assert(eq(int, player.inventory[2].level, 5), "Expected item level at index 2 to be correct");
}

// Add Duplicate Items
Test(test_add_item, duplicate_items) {
    // Initialize player:
    struct Player player = {
        .name = "Alex",
        .level = 2,
        .health = PLAYER_INITIAL_HEALTH
    };

    cr_assert(eq(type(Result), add_item(&player, "Sword", 4), SUCCESS), "Adding to player's inventory should have succeeded");
    cr_assert(eq(type(Result), add_item(&player, "Sword", 7), SUCCESS), "Adding to player's inventory should have succeeded");
    cr_assert(eq(type(Result), add_item(&player, "Sword", 5), SUCCESS), "Adding to player's inventory should have succeeded");
    cr_assert(eq(int, player.inventory_size, 3), "Expected inventory size to be correct");
    cr_assert(eq(str, player.inventory[0].weapon, "Sword"), "Expected weapon at index 0 to be correct");
    cr_assert(eq(int, player.inventory[0].level, 4), "Expected item level at index 0 to be correct");
    cr_assert(eq(str, player.inventory[1].weapon, "Sword"), "Expected weapon at index 1 to be correct");
    cr_assert(eq(int, player.inventory[1].level, 7), "Expected item level at index 1 to be correct");
    cr_assert(eq(str, player.inventory[2].weapon, "Sword"), "Expected weapon at index 2 to be correct");
    cr_assert(eq(int, player.inventory[2].level, 5), "Expected item level at index 2 to be correct");
}