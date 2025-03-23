#include "../minecraft.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

// TESTS LAYOUT:

// Null Checks:
//  1. Player null checks
//  2. Empty Inventory checks
//  3. challenger != opp

// Win tests
// - 4. Level Win (Based on higher level)
// - 5. Health Win (same item max, one player has higher health)
// - 6. Tie Win --> Challenger Wins

// In each win scenario, have to test health
// Case 1. Heath does not drop below 0
// Case 2. Health drops past 0

TestSuite(test_fight_player, .timeout = UNREASONABLY_LONG);

// 1. Null checks
Test(test_fight_player, null_challenger) {
    h_create_player("Opponent");
    cr_assert(
        eq(type(Result), fight_player(NULL, &world.players[0]), FAILURE),
        "Null player passed for challenger, should return FAILURE"
    );
}

Test(test_fight_player, null_opponent) {
    h_create_player("Challenger");
    cr_assert(
        eq(type(Result), fight_player(&world.players[0], NULL), FAILURE),
        "Null player passed for opponent, should return FAILURE"
    );
}

// 2. Inventory checks
Test(test_fight_player, chall_empty_inventory) {
    h_create_player("Opponent");
    h_create_player("Challenger");
    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(opponent, "Bow", 10);
    h_add_item(opponent, "Ender Pearl", 20);

    cr_assert(
        eq(type(Result), fight_player(challenger, opponent), FAILURE),
        "Challenger has empty inventory, should return FAILURE"
    );
}

Test(test_fight_player, opp_empty_inventory) {
    h_create_player("Opponent");
    h_create_player("Challenger");
    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Bow", 10);
    h_add_item(challenger, "Ender Pearl", 20);

    cr_assert(
        eq(type(Result), fight_player(challenger, opponent), FAILURE),
        "Opponent has empty inventory, should return FAILURE"
    );
}

// 3. Chall != Opp

Test(test_fight_player, chall_equals_opp) {
    h_create_player("Player");
    struct Player *player = &world.players[0];

    h_add_item(player, "Bow", 10);
    h_add_item(player, "Ender Pearl", 20);

    cr_assert(
        eq(type(Result), fight_player(player, player), FAILURE),
        "Player can not fight themselves, should return FAILURE"
    );
}

// 4.1. Level Wins - No delete
Test(test_fight_player, chall_level_win_no_delete) {
    h_create_player("Andy"); // my biggest opp
    h_create_player("Henry");
    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Bow", 5);
    h_add_item(challenger, "Ender Pearl", 10); // chall max item: 10
    h_add_item(opponent, "Ender Pearl", 7);    // opp max item: 7
    // This weapon is past the bounds of the array
    // and should not be accessed.
    opponent->inventory[1] = (struct Item){
        .weapon = "Bow", .level = 200
    };

    opponent->health = 50;
    challenger->health = 9; // wrong implementation might delete chall

    cr_assert(
        eq(int, fight_player(challenger, opponent), 0),
        "Challenger has item with the highest level, challenger should win"
    );

    cr_assert(
        eq(int, opponent->health, 40),
        "Opponent should lose health from this fight"
    );

    cr_assert(
        eq(int, world.num_players, 2),
        "No players should be deleted in this fight"
    );
    cr_assert(eq(str, world.players[0].name, "Andy"), "Opponent should have remained after fight");
    cr_assert(eq(str, world.players[1].name, "Henry"), "Challenger should have remained after fight");
}

Test(test_fight_player, opp_level_win_no_delete) {
    h_create_player("Owen");
    h_create_player("Willson");
    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Sword", 5);      // chall max item: 5
    h_add_item(opponent, "Ender Pearl", 12); // opp max item: 12
    h_add_item(opponent, "Sword", 7);
    // This weapon is past the bounds of the array
    // and should not be accessed.
    challenger->inventory[1] = (struct Item){
        .weapon = "Bow", .level = 200
    };

    opponent->health = 50;
    challenger->health = 50;

    cr_assert(
        eq(int, fight_player(challenger, opponent), 1),
        "Oppponent has item with the highest level, opponent should win"
    );

    cr_assert(
        eq(int, challenger->health, 38),
        "Challenger should lose health from this fight"
    );

    cr_assert(
        eq(int, world.num_players, 2),
        "No players should be deleted in this fight"
    );
    cr_assert(eq(str, world.players[0].name, "Owen"), "Opponent should have remained after fight");
    cr_assert(eq(str, world.players[1].name, "Willson"), "Challenger should have remained after fight");
}
// 4.2. Level Wins - With delete

Test(test_fight_player, chall_level_win_with_delete) {
    h_create_player("Henry");
    h_create_player("Bianca");
    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Bow", 5);
    h_add_item(challenger, "Ender Pearl", 10); // chall max item: 10
    h_add_item(opponent, "Ender Pearl", 7);    // opp max item: 7

    opponent->health = 9;
    challenger->health = 9;

    cr_assert(
        eq(int, fight_player(challenger, opponent), 0),
        "Challenger has item with the highest level, challenger should win"
    );

    cr_assert(
        eq(int, world.num_players, 1),
        "The opponent's health dropped to 0 or below therefore they should be deleted."
    );
    cr_assert(
        eq(str, world.players[0].name, "Bianca"),
        "Challenger should have remained after winning fight"
    );
}

Test(test_fight_player, opp_level_win_with_delete) {
    h_create_player("Alex");
    h_create_player("Steve");
    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Bow", 5);
    h_add_item(opponent, "Ender Pearl", 12); // chall max item: 10
    h_add_item(opponent, "Ender Pearl", 7);  // opp max item: 7

    opponent->health = 9;
    challenger->health = 12;

    cr_assert(
        eq(int, fight_player(challenger, opponent), 1),
        "Opponent has item with the highest level, opponent should win"
    );

    cr_assert(
        eq(int, world.num_players, 1),
        "The challenger's health dropped to 0 or below therefore they should be deleted."
    );
    cr_assert(
        eq(str, world.players[0].name, "Alex"),
        "Opponent should have remained after winning fight"
    );
}

// 5.1. Health Wins - Without delete

Test(test_fight_player, chall_health_win_without_delete) {
    h_create_player("Snake");
    h_create_player("Steve");
    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Sword", 5);     // chall max item: 5
    h_add_item(opponent, "Ender Pearl", 5); // opp max item: 5

    opponent->health = 40;
    challenger->health = 50;

    cr_assert(
        eq(int, fight_player(challenger, opponent), 0),
        "Challenger has higher health, challenger should win"
    );

    cr_assert(
        eq(int, opponent->health, 35),
        "Opponent should lose health from this fight"
    );

    cr_assert(
        eq(int, world.num_players, 2),
        "No players should be deleted in this fight"
    );
    cr_assert(eq(str, world.players[0].name, "Snake"), "Opponent should have remained after fight");
    cr_assert(eq(str, world.players[1].name, "Steve"), "Challenger should have remained after fight");
}

Test(test_fight_player, opp_health_win_without_delete) {
    h_create_player("Little Mac");
    h_create_player("Steve");
    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Sword", 15);     // chall max item: 15
    h_add_item(opponent, "Ender Pearl", 15); // opp max item: 15

    opponent->health = 40;
    challenger->health = 20;

    cr_assert(
        eq(int, fight_player(challenger, opponent), 1),
        "Opponent has higher health, opponent should win"
    );

    cr_assert(
        eq(int, challenger->health, 5),
        "Challenger should lose health from this fight"
    );

    cr_assert(
        eq(int, world.num_players, 2),
        "No players should be deleted in this fight"
    );
    cr_assert(eq(str, world.players[0].name, "Little Mac"), "Opponent should have remained after fight");
    cr_assert(eq(str, world.players[1].name, "Steve"), "Challenger should have remained after fight");
}

// 5.2. Health Wins - With delete
Test(test_fight_player, chall_health_win_with_delete) {
    h_create_player("Ph1LzA");
    h_create_player("Technoblade");

    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Sword", 5);     // chall max item: 5
    h_add_item(opponent, "Ender Pearl", 5); // opp max item: 5

    opponent->health = 4;
    challenger->health = 50;

    cr_assert(
        eq(int, fight_player(challenger, opponent), 0),
        "Challenger has higher health, challenger should win"
    );

    cr_assert(
        eq(int, world.num_players, 1),
        "The opponent's health dropped to 0 or below therefore they should be deleted."
    );
    cr_assert(
        eq(str, world.players[0].name, "Technoblade"),
        "Challenger should have remained after winning fight"
    );
}

Test(test_fight_player, opp_health_win_with_delete) {
    h_create_player("Mine");
    h_create_player("Craft");

    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Sword", 5);     // chall max item: 5
    h_add_item(opponent, "Ender Pearl", 5); // opp max item: 5

    opponent->health = 50;
    challenger->health = 1;

    cr_assert(
        eq(int, fight_player(challenger, opponent), 1),
        "Oppponent has higher health, opponent should win"
    );

    cr_assert(
        eq(int, world.num_players, 1),
        "The challenger's health dropped to 0 or below therefore they should be deleted."
    );
    cr_assert(
        eq(str, world.players[0].name, "Mine"),
        "Opponent should have remained after winning fight"
    );
}

// edge case health == 0
Test(test_fight_player, opp_health_win_with_delete_zero_health) {
    h_create_player("Mine");
    h_create_player("Craft");

    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Sword", 5);     // chall max item: 5
    h_add_item(opponent, "Ender Pearl", 5); // opp max item: 5

    opponent->health = 50;
    challenger->health = 5;

    cr_assert(
        eq(int, fight_player(challenger, opponent), 1),
        "Oppponent has higher health, opponent should win"
    );

    cr_assert(
        eq(int, world.num_players, 1),
        "The challenger's health dropped to 0 or below therefore they should be deleted."
    );
    cr_assert(
        eq(str, world.players[0].name, "Mine"),
        "Opponent should have remained after winning fight"
    );
}

// 6.1. Tie Wins - Without delete

Test(test_fight_player, tie_win_without_delete) {
    h_create_player("Ph1LzA");
    h_create_player("Baby Zombie");

    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Sword", 17);     // chall max item: 17
    h_add_item(opponent, "Ender Pearl", 17); // opp max item: 17

    opponent->health = 40;
    challenger->health = 40;

    cr_assert(
        eq(int, fight_player(challenger, opponent), 0),
        "The fight was a tie, therefore the challenger should automatically win"
    );

    cr_assert(
        eq(int, opponent->health, 23),
        "Opponent should lose health from this fight"
    );

    cr_assert(
        eq(int, world.num_players, 2),
        "No players should be deleted in this fight"
    );
    cr_assert(eq(str, world.players[0].name, "Ph1LzA"), "Opponent should have remained after fight");
    cr_assert(eq(str, world.players[1].name, "Baby Zombie"), "Challenger should have remained after fight");
}

// 6.2. Tie Wins - With delete
Test(test_fight_player, tie_win_with_delete) {
    h_create_player("You");
    h_create_player("Grass");

    struct Player *opponent = &world.players[0];
    struct Player *challenger = &world.players[1];

    h_add_item(challenger, "Sword", 5);     // chall max item: 5
    h_add_item(opponent, "Ender Pearl", 5); // opp max item: 5

    opponent->health = 3;
    challenger->health = 3;

    cr_assert(
        eq(int, fight_player(challenger, opponent), 0),
        "The fight was a tie, therefore the challenger should automatically win"
    );

    cr_assert(
        eq(int, world.num_players, 1),
        "The opponent's health dropped to 0 or below therefore they should be deleted."
    );
    cr_assert(
        eq(str, world.players[0].name, "Grass"),
        "Challenger should have remained after winning fight"
    );
}
