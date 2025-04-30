#include "../minecraft_v2.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_fight_monster, .timeout = UNREASONABLY_LONG);

Test(test_fight_monster, null_world_returns_failure) {
    cr_assert(eq(type(Result), fight_monster(NULL, "Steve"), FAILURE), "Expected FAILURE when world is NULL");
}

Test(test_fight_monster, null_name_returns_failure) {
    World *world = h_create_world();

    cr_assert(eq(type(Result), fight_monster(world, NULL), FAILURE), "Expected FAILURE when name is NULL");

    h_free_world(world);
}

Test(test_fight_monster, player_not_found_returns_failure) {
    World *world = h_create_world();

    cr_assert(eq(type(Result), fight_monster(world, "Bianca"), FAILURE), "Expected FAILURE when player does not exist");

    h_free_world(world);
}

Test(test_fight_monster, no_monsters_returns_failure) {
    World *world = h_create_world();

    h_create_player(world, "Steve", 3);
    cr_assert(eq(type(Result), fight_monster(world, "Steve"), FAILURE), "Expected FAILURE when there are no monsters");

    h_free_world(world);
}

Test(test_fight_monster, player_wins_against_monster) {
    World *world = h_create_world();

    h_create_player(world, "Steve", 10);
    h_create_monster(world, ZOMBIE, 5);

    cr_assert(eq(type(Result), fight_monster(world, "Steve"), SUCCESS), "Expected SUCCESS when player wins");

    cr_assert(eq(int, world->num_players, 1), "Expected number of players to stay the same");
    cr_assert(eq(str, world->players[0]->name, "Steve"), "Expected player to still exist");
    cr_assert(eq(int, world->players[0]->level, 11), "Expected player level to increase by 1");
    cr_assert(eq(int, world->monsters.size, 0), "Expected no monsters left after winning fight");

    h_free_world(world);
}

Test(test_fight_monster, monster_wins_and_player_deleted) {
    World *world = h_create_world();

    h_create_player(world, "Alex", 2);
    h_create_monster(world, ZOMBIE, 10);

    cr_assert(eq(type(Result), fight_monster(world, "Alex"), SUCCESS), "Expected SUCCESS when fight occurs");

    cr_assert(eq(int, world->num_players, 0), "Expected player to be deleted");

    cr_assert(eq(int, world->monsters.size, 1), "Expected monster to remain after winning fight");
    cr_assert(eq(type(mob_t), world->monsters.head->monster->mob, ZOMBIE), "Expected monster to remain the same type");
    cr_assert(eq(int, world->monsters.head->monster->level, 11), "Expected monster's level to increment");
    cr_assert(zero(ptr, world->monsters.head->next), "Expected node after Zombie to be NULL");

    h_free_world(world);
}

Test(test_fight_monster, monster_wins_and_creeper_resets_player_level) {
    World *world = h_create_world();

    h_create_player(world, "Henry", 6);
    h_create_monster(world, CREEPER, 10);

    cr_assert(eq(type(Result), fight_monster(world, "Henry"), SUCCESS), "Expected SUCCESS when creeper wins");

    cr_assert(eq(int, world->num_players, 1), "Expected number of players to stay the same");
    cr_assert(eq(str, world->players[0]->name, "Henry"), "Expected player to still exist");
    cr_assert(eq(int, world->players[0]->level, 1), "Expected player level to be reset to 1");

    cr_assert(eq(int, world->monsters.size, 1), "Expected monster to remain after winning fight");
    cr_assert(eq(type(mob_t), world->monsters.head->monster->mob, CREEPER), "Expected monster to remain the same type");
    cr_assert(eq(int, world->monsters.head->monster->level, 11), "Expected monster's level to increment");
    cr_assert(zero(ptr, world->monsters.head->next), "Expected node after Creeper to be NULL");

    h_free_world(world);
}

Test(test_fight_monster, player_wins_same_level) {
    World *world = h_create_world();

    h_create_player(world, "Steve", 10);
    h_create_monster(world, ZOMBIE, 10);

    cr_assert(eq(type(Result), fight_monster(world, "Steve"), SUCCESS), "Expected SUCCESS when player wins");

    cr_assert(eq(int, world->num_players, 1), "Expected number of players to stay the same");
    cr_assert(eq(str, world->players[0]->name, "Steve"), "Expected player to still exist");
    cr_assert(eq(int, world->players[0]->level, 11), "Expected player level to increase by 1");
    cr_assert(eq(int, world->monsters.size, 0), "Expected no monsters left after winning fight");

    h_free_world(world);
}
Test(test_fight_monster, multiple_monsters) {
    World *world = h_create_world();
    
    h_create_player(world, "Steve", 10);
    h_create_monster(world, ZOMBIE, 10);
    h_create_monster(world, CREEPER, 10);
    h_create_monster(world, ENDERMAN, 13);

    // Fight ZOMBIE
    cr_assert(eq(type(Result), fight_monster(world, "Steve"), SUCCESS), "Expected SUCCESS when player wins");

    cr_assert(eq(int, world->num_players, 1), "Expected number of players to stay the same");
    cr_assert(eq(str, world->players[0]->name, "Steve"), "Expected player to still exist");
    cr_assert(eq(int, world->players[0]->level, 11), "Expected player level to increase by 1");
    // Check monster linked list
    cr_assert(eq(int, world->monsters.size, 2), "Expected monster size to be decremented by 1");
    cr_assert(not(zero(ptr, world->monsters.head)), "Expected monster linked list head to be non-NULL");
    // -- Creeper, aw man
    cr_assert(eq(type(mob_t), world->monsters.head->monster->mob, CREEPER), "Expected 1st node to point to CREEPER");
    cr_assert(eq(int, world->monsters.head->monster->level, 10), "Expected Creeper's level to be correct");
    cr_assert(not(zero(ptr, world->monsters.head->next)), "Expected node after Creeper to be non-NULL");
    // -- Enderman
    cr_assert(eq(type(mob_t), world->monsters.head->next->monster->mob, ENDERMAN), "Expected 2nd node to point to ENDERMAN");
    cr_assert(eq(int, world->monsters.head->next->monster->level, 13), "Expected Enderman's level to be correct");
    cr_assert(zero(ptr, world->monsters.head->next->next), "Expected node after Enderman to be NULL");

    // Fight CREEPER
    cr_assert(eq(type(Result), fight_monster(world, "Steve"), SUCCESS), "Expected SUCCESS when player wins");

    cr_assert(eq(int, world->num_players, 1), "Expected number of players to stay the same");
    cr_assert(eq(str, world->players[0]->name, "Steve"), "Expected player to still exist");
    cr_assert(eq(int, world->players[0]->level, 12), "Expected player level to increase by 1");
    // Check monster linked list
    cr_assert(eq(int, world->monsters.size, 1), "Expected monster size to be decremented by 1");
    cr_assert(not(zero(ptr, world->monsters.head)), "Expected monster linked list head to be non-NULL");
    // -- Enderman
    cr_assert(eq(type(mob_t), world->monsters.head->monster->mob, ENDERMAN), "Expected 1st node to point to ENDERMAN");
    cr_assert(eq(int, world->monsters.head->monster->level, 13), "Expected Enderman's level to be correct");
    cr_assert(zero(ptr, world->monsters.head->next), "Expected node after Enderman to be NULL");

    // Fight ENDERMAN
    cr_assert(eq(type(Result), fight_monster(world, "Steve"), SUCCESS), "Expected SUCCESS when Enderman wins");

    cr_assert(eq(int, world->num_players, 0), "Expected player to be deleted");
    // Check monster linked list
    cr_assert(eq(int, world->monsters.size, 1), "Expected monster size to be decremented by 1");
    cr_assert(not(zero(ptr, world->monsters.head)), "Expected monster linked list head to be non-NULL");
    // -- Enderman
    cr_assert(eq(type(mob_t), world->monsters.head->monster->mob, ENDERMAN), "Expected 1st node to point to ENDERMAN");
    cr_assert(eq(int, world->monsters.head->monster->level, 14), "Expected Enderman's level to be correct");
    cr_assert(zero(ptr, world->monsters.head->next), "Expected node after Enderman to be NULL");

    h_free_world(world);
}