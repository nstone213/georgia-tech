// DO NOT MODIFY THIS FILE
/**
 * @brief Header file for global macros, structures and fields to be used by the
 * user's program.
 */
#ifndef MINECRAFT_H
#define MINECRAFT_H
#include "my_string.h"

#define UNUSED_PARAM(x) ((void)x) // This macro is only used for turning off compiler errors initially

// Sizes for different arrays
#define MAX_NAME_LENGTH 20
#define MAX_INVENTORY_SIZE 20
#define MAX_PLAYERS 20
#define MAX_MONSTERS 15
#define PLAYER_INITIAL_HEALTH 50
#define ENDER_DRAGON_HEALTH 1500

// Success and failure codes for function return
#define SUCCESS 0
#define FAILURE -1
#define INCOMPLETE -2

// Students should add their structs here

struct Monster {
    char mob[9]; // must be "Skeleton", "Zombie", or "Enderman" (9 including null terminator)
    int level;
};

struct Item {
    char weapon[12]; // must be "Ender Pearl", "Sword", or "Bow"
    int level;
};

struct Player {
    char name[MAX_NAME_LENGTH];
    struct Item inventory[MAX_INVENTORY_SIZE];
    int inventory_size;
    int level;
    int health;
};

struct World {
    struct Player players[MAX_PLAYERS];
    int num_players;
    struct Monster monsters[MAX_MONSTERS];
    int num_monsters;
};

int create_player(const char *name);
int delete_player(const char *player_name);
int add_item(struct Player *player, const char *weapon, int level);
int spawn_monster(const char *mob, int level);
int fight_monster(struct Player *player);
int fight_player(struct Player *challenger, struct Player *opponent);
int fight_ender_dragon(struct Player *player);

#endif