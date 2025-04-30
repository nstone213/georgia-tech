#include "../minecraft_v2.h"
#include "ag_utils.h"
#include "fakemalloc.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_create_monster, .timeout = UNREASONABLY_LONG);

// 1. Incorrect arg cases
Test(test_create_monster, null_world) {
    cr_assert(
        eq(type(Result), create_monster(NULL, ENDERMAN, 10), FAILURE),
        "Should return FAILURE when NULL world passed"
    );
}

Test(test_create_monster, zero_level) {
    World *new_world = h_create_world();
    cr_assert(
        eq(type(Result), create_monster(new_world, CREEPER, 0), FAILURE),
        "Should return FAILURE when zero level is passed"
    );
    h_free_world(new_world);
}

Test(test_create_monster, negative_level) {
    World *new_world = h_create_world();
    cr_assert(
        eq(type(Result), create_monster(new_world, ENDERMAN, -100), FAILURE),
        "Should return FAILURE when negative level is passed"
    );
    h_free_world(new_world);
}

// 2. Malloc failure cases
Test(test_create_monster, malloc_failure_single_mob) {
    World *new_world = h_create_world();
    bytes_until_fail = sizeof(Monster) + sizeof(Node) - 1;
    cr_assert(eq(type(Result), create_monster(new_world, CREEPER, 10), FAILURE), "malloc failing should result in FAILURE");
    h_free_world(new_world);
}

Test(test_create_monster, malloc_failure_multiple_mobs) {
    World *new_world = h_create_world();
    bytes_until_fail = (3 * (sizeof(Monster) + sizeof(Node))) - 1;
    h_create_monster(new_world, CREEPER, 10);
    h_create_monster(new_world, ZOMBIE, 20);
    cr_assert(eq(type(Result), create_monster(new_world, ENDERMAN, 100), FAILURE), "malloc failing should result in FAILURE");
    cr_assert(eq(int, new_world->monsters.size, 2), "Expected number of mobs to stay unchanged after failure");
    h_free_world(new_world);
}

// 3. SUCCESS cases
Test(test_create_monster, basic) {
    World *new_world = h_create_world();

    int result = create_monster(new_world, ENDERMAN, 5);
    cr_assert(eq(type(Result), result, SUCCESS), "Expected monster to be created successfully");
    cr_assert(not(zero(ptr, new_world->monsters.head)), "Expected number of mobs in list to be correct");

    cr_assert(eq(i32, new_world->monsters.size, 1), "Expected mob list size to be correct");
    cr_assert(eq(type(mob_t), new_world->monsters.head->monster->mob, ENDERMAN), "Expected mob type to be correct");
    cr_assert(eq(int, new_world->monsters.head->monster->level, 5), "Expected mob level to be correct");
    cr_assert(zero(ptr, new_world->monsters.head->next), "Expected node after Enderman to be NULL");

    h_free_world(new_world);
}

Test(test_create_monster, success_multiple_mobs) {
    World *new_world = h_create_world();

    // Add CREEPER
    int result1 = create_monster(new_world, CREEPER, 10);
    cr_assert(eq(type(Result), result1, SUCCESS), "Expect monster to be created successfully");
    cr_assert(not(zero(ptr, new_world->monsters.head)), "Expected head to be non-NULL");
    cr_assert(eq(i32, new_world->monsters.size, 1), "Expected mob list size to be correct");
    // -- Creeper
    cr_assert(eq(type(mob_t), new_world->monsters.head->monster->mob, CREEPER), "Expected mob type to be correct");
    cr_assert(eq(int, new_world->monsters.head->monster->level, 10), "Expected mob level to be correct");
    cr_assert(zero(ptr, new_world->monsters.head->next), "Expected node after Creeper to be NULL");

    // Add SKELETON
    int result2 = create_monster(new_world, SKELETON, 20);
    cr_assert(eq(type(Result), result2, SUCCESS), "Expect monster to be created successfully");
    cr_assert(eq(i32, new_world->monsters.size, 2), "Expected mob list size to be correct");
    // -- Creeper
    cr_assert(eq(type(mob_t), new_world->monsters.head->monster->mob, CREEPER), "Expected mob type to be correct");
    cr_assert(eq(int, new_world->monsters.head->monster->level, 10), "Expected mob level to be correct");
    cr_assert(not(zero(ptr, new_world->monsters.head->next)), "Expected node after Creeper to be non-NULL");
    // -- Skeleton
    cr_assert(eq(type(mob_t), new_world->monsters.head->next->monster->mob, SKELETON), "Expected mob type to be correct");
    cr_assert(eq(int, new_world->monsters.head->next->monster->level, 20), "Expected mob level to be correct");
    cr_assert(zero(ptr, new_world->monsters.head->next->next), "Expected node after Skeleton to be NULL");

    // Add ENDERMAN
    int result3 = create_monster(new_world, ENDERMAN, 4);
    cr_assert(eq(type(Result), result3, SUCCESS), "Expect monster to be created successfully");
    cr_assert(eq(i32, new_world->monsters.size, 3), "Expected mob list size to be correct");
    // -- Creeper
    cr_assert(eq(type(mob_t), new_world->monsters.head->monster->mob, CREEPER), "Expected mob type to be correct");
    cr_assert(eq(int, new_world->monsters.head->monster->level, 10), "Expected mob level to be correct");
    // -- Skeleton
    cr_assert(eq(type(mob_t), new_world->monsters.head->next->monster->mob, SKELETON), "Expected mob type to be correct");
    cr_assert(eq(int, new_world->monsters.head->next->monster->level, 20), "Expected mob level to be correct");
    cr_assert(not(zero(ptr, new_world->monsters.head->next->next)), "Expected node after Skeleton to be non-NULL");
    // -- Enderman
    cr_assert(eq(type(mob_t), new_world->monsters.head->next->next->monster->mob, ENDERMAN), "Expected mob type to be correct");
    cr_assert(eq(int, new_world->monsters.head->next->next->monster->level, 4), "Expected mob level to be correct");
    cr_assert(zero(ptr, new_world->monsters.head->next->next->next), "Expected node after Enderman to be NULL");

    h_free_world(new_world);
}