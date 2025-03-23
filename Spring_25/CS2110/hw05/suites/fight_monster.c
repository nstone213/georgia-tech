#include "../minecraft.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_fight_monster, .timeout = UNREASONABLY_LONG);

// NULL Player Failure
Test(test_fight_monster, null_player) {
    // Initialize world:
    h_spawn_monster("Skeleton", 7);

    // Test `fight_monster`:
    cr_assert(
        eq(type(Result), fight_monster(NULL), FAILURE),
        "Expected FAILURE when player is NULL"
    );
    cr_assert(eq(int, world.num_monsters, 1), "Expected number of monsters in the world to be correct");
}

// Having No Monsters in the World Should Fail
Test(test_fight_monster, no_monsters) {
    // Initialize world:
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 5,
        .health = PLAYER_INITIAL_HEALTH,
        .inventory = {
            {.weapon = "Sword", .level = 3}
        },
        .inventory_size = 1
    });

    // Test `fight_monster`:
    cr_assert(
        eq(type(Result), fight_monster(&world.players[0]), FAILURE),
        "Expected FAILURE when no monsters exist"
    );
    cr_assert(eq(int, world.num_players, 1), "Expected number of players in the world to be correct");
}

// Player Has Enough Weakness Items and Wins Against Skeleton and Gets Bow
Test(test_fight_monster, win_skeleton) {
    // Initialize world:
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 4,
        .health = PLAYER_INITIAL_HEALTH,
        .inventory = {
            {.weapon = "Ender Pearl", .level = 2},
            {.weapon = "Ender Pearl", .level = 2}
        },
        .inventory_size = 2
    });

    h_spawn_monster("Skeleton", 7); // 2 * 4 = 8 > 7

    // Test `fight_monster`:
    int result = fight_monster(&world.players[0]);
    cr_assert(eq(int, result, 0), "Expected player to win (0)");
    cr_assert(eq(int, world.players[0].level, 5), "Expected player's level to increase");
    cr_assert(eq(int, world.players[0].inventory_size, 3), "Expected inventory size to increase");
    cr_assert(eq(str, world.players[0].inventory[2].weapon, "Bow"), "Expected correct drop item after fighting monster");
    cr_assert(eq(int, world.players[0].inventory[2].level, 7), "Expected correct drop level after fighting monster");
    cr_assert(eq(int, world.num_players, 1), "Expected number of players in world to stay the same");
    cr_assert(eq(int, world.num_monsters, 0), "Expected number of monsters in the world to decrease");
}

// Player Has Enough Weakness Items Against Enderman and Gets Ender Pearl
Test(test_fight_monster, win_enderman) {
    // Initialize world:
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 3,
        .health = PLAYER_INITIAL_HEALTH,
        .inventory = {
            {.weapon = "Sword", .level = 4},
            {.weapon = "Sword", .level = 2},
            {.weapon = "Sword", .level = 3},
            {.weapon = "Sword", .level = 1},
        },
        .inventory_size = 4
    });

    h_spawn_monster("Enderman", 10); // 4 Swords * 3 = 12 > 10

    // Test `fight_monster`:
    int result = fight_monster(&world.players[0]);
    cr_assert(eq(int, result, 0), "Expected player to win (0)");
    cr_assert(eq(int, world.players[0].level, 4), "Expected player's level to increase");
    cr_assert(eq(int, world.players[0].inventory_size, 5), "Expected inventory size to increase");
    cr_assert(eq(str, world.players[0].inventory[4].weapon, "Ender Pearl"), "Expected correct drop item after fighting monster");
    cr_assert(eq(int, world.players[0].inventory[4].level, 10), "Expected correct drop level after fighting monster");
    cr_assert(eq(int, world.num_players, 1), "Expected number of players in world to stay the same");
    cr_assert(eq(int, world.num_monsters, 0), "Expected number of monsters in the world to decrease");
}

// Player Has Enough Weakness Items Against Zombie and Gets Sword
Test(test_fight_monster, win_zombie) {
    // Initialize world:
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 2,
        .health = PLAYER_INITIAL_HEALTH,
        .inventory = {
            {.weapon = "Bow", .level = 6},
            {.weapon = "Bow", .level = 2},
            {.weapon = "Bow", .level = 3},
            {.weapon = "Bow", .level = 1},
            {.weapon = "Bow", .level = 1},
            {.weapon = "Bow", .level = 1},
        },
        .inventory_size = 6
    });

    h_spawn_monster("Zombie", 8); // 6 Bows * 2 = 12 > 8

    // Test `fight_monster`:
    int result = fight_monster(&world.players[0]);
    cr_assert(eq(int, result, 0), "Expected player to win (0)");
    cr_assert(eq(int, world.players[0].level, 3), "Expected player's level to increase");
    cr_assert(eq(int, world.players[0].inventory_size, 7), "Expected inventory size to increase");
    cr_assert(eq(str, world.players[0].inventory[6].weapon, "Sword"), "Expected correct drop item after fighting monster");
    cr_assert(eq(int, world.players[0].inventory[6].level, 8), "Expected correct drop level after fighting monster");
    cr_assert(eq(int, world.num_players, 1), "Expected number of players in world to stay the same");
    cr_assert(eq(int, world.num_monsters, 0), "Expected number of monsters in the world to decrease");
}

// Not Enough Weakness Items and Loses to Skeleton
Test(test_fight_monster, lose_skeleton) {
    // Initialize world:
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 3,
        .health = 25,
        .inventory = {
            {.weapon = "Ender Pearl", .level = 1},
        },
        .inventory_size = 1
    });

    h_spawn_monster("Skeleton", 10); // 1 Ender Pearl * 3 = 3 < 10

    // Test `fight_monster`:
    int result = fight_monster(&world.players[0]);
    cr_assert(eq(int, result, 1), "Expected player to lose (1)");
    cr_assert(eq(int, world.players[0].health, 15), "Expected health to be correct");
    cr_assert(eq(int, world.num_players, 1), "Expected number of players in world to be correct");
    cr_assert(eq(int, world.num_monsters, 1), "Expected number of monsters in world to be correct");
}

// Not Enough Weakness Items and Loses to Skeleton
Test(test_fight_monster, lose_enderman) {
    // Initialize world:
    h_create_player("Bianca");
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 5,
        .health = 25,
        .inventory = {
            {.weapon = "Sword", .level = 5},
            {.weapon = "Sword", .level = 5},
            // This weapon is past the bounds of the array
            // and should not be accessed.
            {.weapon = "Sword", .level = 5},
        },
        .inventory_size = 2
    });
    h_create_player("Alex");

    h_spawn_monster("Enderman", 10); // 2 Sword * 5 = 10 == 10

    // Test `fight_monster`:
    int result = fight_monster(&world.players[1]);
    cr_assert(eq(int, result, 1), "Expected player to lose (1)");
    cr_assert(eq(int, world.players[1].health, 15), "Expected health to be correct");
    cr_assert(eq(int, world.num_players, 3), "Expected number of players in world to be correct");
    cr_assert(eq(int, world.num_monsters, 1), "Expected number of monsters in world to be correct");
}

// Player Dies After Losing
Test(test_fight_monster, player_dies) {
    // Initialize world:
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 1,
        .health = 5,
        .inventory = {
            {.weapon = "Bow", .level = 1},
        },
        .inventory_size = 1
    });
    h_create_player("Alex");

    h_spawn_monster("Zombie", 8); // 1 Bow * 1 = 1 < 8

    // Test `fight_monster`:
    int result = fight_monster(&world.players[0]);
    cr_assert(eq(int, result, 1), "Expected player to lose (1)");
    cr_assert(eq(int, world.num_players, 1), "Expected number of players to decrease");
    cr_assert(eq(int, world.num_monsters, 1), "Expected number of monsters in world to be correct");
}

// Player Has Incorrect Weakness Items for Fight against Skeleton so Loses
Test(test_fight_monster, no_weakness_items) {
    // Initialize world:
    h_create_player("Alex");
    h_create_player("Andy");
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 5,
        .health = 25,
        .inventory = {
            {.weapon = "Sword", .level = 3},
        },
        .inventory_size = 1
    });

    h_spawn_monster("Skeleton", 10); // 0 * 5 = 0 < 10

    // Test `fight_monster`:
    int result = fight_monster(&world.players[2]);
    cr_assert(eq(int, result, 1), "Expected player to lose (1)");
    cr_assert(eq(int, world.players[2].health, 15), "Expected health to be correct");
    cr_assert(eq(int, world.num_players, 3), "Expected number of players in world to be correct");
    cr_assert(eq(int, world.num_monsters, 1), "Expected number of monsters in world to be correct");
}

// Player Has Incorrect Weakness Items for Fight against Skeleton so Loses
Test(test_fight_monster, multiple_monsters) {
    // Initialize world:
    h_create_player("Alex");
    h_add_player_to_world((struct Player){
        .name = "Steve",
        .level = 5,
        .health = 25,
        .inventory = {
            {.weapon = "Sword", .level = 3},
            {.weapon = "Sword", .level = 4},
            {.weapon = "Sword", .level = 5},
        },
        .inventory_size = 3
    });

    h_spawn_monster("Zombie", 10);
    h_spawn_monster("Skeleton", 1);
    h_spawn_monster("Enderman", 10);

    // Test `fight_monster`:
    // Fight Enderman (win)
    cr_assert(eq(int, fight_monster(&world.players[1]), 0), "Expected player to win (0) against Enderman");
    cr_assert(eq(int, world.players[1].level, 6), "Expected player's level to increase");
    cr_assert(eq(int, world.players[1].inventory_size, 4), "Expected inventory size to increase");
    cr_assert(eq(str, world.players[1].inventory[3].weapon, "Ender Pearl"), "Expected correct drop item after fighting monster");
    cr_assert(eq(int, world.players[1].inventory[3].level, 10), "Expected correct drop level after fighting monster");
    cr_assert(eq(int, world.num_players, 2), "Expected number of players in the world to decrease");
    cr_assert(eq(int, world.num_monsters, 2), "Expected number of monsters in the world to decrease");

    // Fight Skeleton (win)
    cr_assert(eq(int, fight_monster(&world.players[1]), 0), "Expected player to win (0) against Skeleton");
    cr_assert(eq(int, world.players[1].level, 7), "Expected player's level to increase");
    cr_assert(eq(int, world.players[1].inventory_size, 5), "Expected inventory size to increase");
    cr_assert(eq(str, world.players[1].inventory[4].weapon, "Bow"), "Expected correct drop item after fighting monster");
    cr_assert(eq(int, world.players[1].inventory[4].level, 1), "Expected correct drop level after fighting monster");
    cr_assert(eq(int, world.num_players, 2), "Expected number of players in the world to decrease");
    cr_assert(eq(int, world.num_monsters, 1), "Expected number of monsters in the world to decrease");

    // Fight Zombie (lose)
    cr_assert(eq(int, fight_monster(&world.players[1]), 1), "Expected player to lose (1) against Zombie");
    cr_assert(eq(int, world.players[1].health, 15), "Expected health to be correct");
    cr_assert(eq(int, world.num_players, 2), "Expected number of players in world to be correct");
    cr_assert(eq(int, world.num_monsters, 1), "Expected number of monsters in world to be correct");
}
