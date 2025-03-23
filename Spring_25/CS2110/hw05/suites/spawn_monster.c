#include "../minecraft.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

// Basic Monster Addition
Test(test_spawn_monster, basic) {
    int result = spawn_monster("Enderman", 5);
    cr_assert(eq(type(Result), result, SUCCESS), "Expected monster to be spawned successfully");
    cr_assert(eq(int, world.num_monsters, 1), "Expected number of monsters in world to be correct");
    cr_assert(eq(str, world.monsters[0].mob, "Enderman"), "Expected correct mob");
    cr_assert(eq(int, world.monsters[0].level, 5), "Expected correct level");
}

// Adding Multiple Monsters Should Be Contiguous
Test(test_spawn_monster, multiple) {
    cr_assert(eq(type(Result), spawn_monster("Enderman", 3), SUCCESS), "Expected spawning of 'Enderman' to succeed");
    cr_assert(eq(type(Result), spawn_monster("Zombie", 7), SUCCESS), "Expected spawning of 'Zombie' to succeed");
    cr_assert(eq(type(Result), spawn_monster("Skeleton", 2), SUCCESS), "Expected spawning of 'Skeleton' to succeed");

    // Check world was created correctly:
    cr_assert(eq(int, world.num_monsters, 3), "Expected number of monsters in world to be correct");

    cr_assert(eq(str, world.monsters[0].mob, "Enderman"), "Expected correct mob at index 0");
    cr_assert(eq(int, world.monsters[0].level, 3), "Expected correct level at index 0");

    cr_assert(eq(str, world.monsters[1].mob, "Zombie"), "Expected correct mob at index 1");
    cr_assert(eq(int, world.monsters[1].level, 7), "Expected correct level at index 1");

    cr_assert(eq(str, world.monsters[2].mob, "Skeleton"), "Expected correct mob at index 2");
    cr_assert(eq(int, world.monsters[2].level, 2), "Expected correct level at index 2");
}

// We should not allow adding a monster when world is at capacity
Test(test_spawn_monster, world_full) {
    // Initialize world:
    for (int i = 0; i < MAX_MONSTERS - 1; i++) {
        h_spawn_monster("Enderman", 3);
    }

    // Test `spawn_monster`:
    cr_assert(eq(type(Result), spawn_monster("Enderman", 3), SUCCESS), "Expected SUCCESS while world is almost full");
    cr_assert(eq(int, world.num_monsters, MAX_MONSTERS), "Expected number of monsters in world to increment after success");

    cr_assert(eq(type(Result), spawn_monster("Enderman", 3), FAILURE), "Expected spawning when world is full to fail");
    cr_assert(eq(int, world.num_monsters, MAX_MONSTERS), "Expected number of monsters in world to stay unchanged after failure");
}

// Invalid Monster Name
Test(test_spawn_monster, invalid_name) {
    // Initialize world:
    h_spawn_monster("Enderman", 3);

    // Test `spawn_monster`:
    int result = spawn_monster("Creeper", 5);
    cr_assert(eq(type(Result), result, FAILURE), "Expected spawning of monster with invalid name to fail");
    cr_assert(
        eq(int, world.num_monsters, 1),
        "Expected number of monsters in world to stay unchanged after failure"
    );
}

// Null Name Failure
Test(test_spawn_monster, null_name) {
    int result = spawn_monster(NULL, 5);
    cr_assert(eq(type(Result), result, FAILURE), "Expected spawning of monster with NULL name to fail");
    cr_assert(
        eq(int, world.num_monsters, 0),
        "Expected number of monsters in world to stay unchanged after failure"
    );
}

// Name is Too Big
Test(test_spawn_monster, name_too_long) {
    // Initialize world:
    h_spawn_monster("Enderman", 3);
    h_spawn_monster("Zombie", 1);

    // Create name:
    char long_name[MAX_NAME_LENGTH + 1] = {0};
    memset(long_name, 'Z', sizeof(long_name) - 1);
    // Test `spawn_monster`:
    int result = spawn_monster(long_name, 5);
    cr_assert(eq(type(Result), result, FAILURE), "Expected spawning of monster with overly long name to fail", result);
    cr_assert(
        eq(int, world.num_monsters, 2),
        "Expected number of monsters in world to stay unchanged after failure"
    );
}

// Level Less Than 1
Test(test_spawn_monster, invalid_level) {
    cr_assert(
        eq(type(Result), spawn_monster("Zombie", 0), FAILURE),
        "Expected spawning of monster with level 0 to fail"
    );
    cr_assert(
        eq(type(Result), spawn_monster("Skeleton", -5), FAILURE),
        "Expected spawning of monster with negative level to fail"
    );

    cr_assert(
        eq(int, world.num_monsters, 0),
        "Expected number of monsters in world to stay unchanged after failure"
    );
}