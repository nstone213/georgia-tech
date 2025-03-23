/**
 * @file minecraft.c
 * @author Nicholas Stone
 *
 * @date 2025-03-xx
 */

// DO NOT MODIFY THE INCLUDE(S) LIST
#include "minecraft.h"
#include "my_string.h"

// DO NOT MODIFY THE GLOBAL VARIABLE(S) LIST
struct World world;

// ALWAYS check the validity of the passed in arguments
// (i.e. not NULL, in the correct range, not too long)

// If any of the parameters are invalid, the function should
// do nothing and return FAILURE.

/**
 * @brief Creates a player with the given name and adds it to the world.
 *
 * @param name The name of the player.
 * @return FAILURE if name NULL, too long, or alreadty existing, or if world has maximum players. SUCCESS otherwise.
 */
int create_player(const char *name) {
    // Check if name is NULL
    if (name == NULL) {
        return FAILURE;
    }

    // Check if name is too long
    if (my_strlen(name) >= MAX_NAME_LENGTH) {
        return FAILURE;
    }

    // Check if world is full
    if (world.num_players >= MAX_PLAYERS) {
        return FAILURE;
    }

    // Check if player already exists
    for (int i = 0; i < world.num_players; i++) {
        if (my_strncmp(world.players[i].name, name, MAX_NAME_LENGTH) == 0) {
            return FAILURE;
        }
    }

    // Add player to the world
    my_strncpy(world.players[world.num_players].name, name, MAX_NAME_LENGTH);
    world.players[world.num_players].inventory_size = 0;
    world.players[world.num_players].level = 1;
    world.players[world.num_players].health = PLAYER_INITIAL_HEALTH;
    world.num_players++;

    return SUCCESS;
}

/**
 * @brief Deletes a player from the world.
 *
 * @param player The name of the player to be deleted.
 * @return SUCCESS on success, FAILURE if the player passed in is NULL.
 */
int delete_player(const char *player_name) {
    // Check if player_name is NULL
    if (player_name == NULL) {
        return FAILURE;
    }

    // Find player in the world
    int player_index = -1;
    for (int i = 0; i < world.num_players; i++) {
        if (my_strncmp(world.players[i].name, player_name, MAX_NAME_LENGTH) == 0) {
            player_index = i;
            break;
        }
    }

    // Check if player exists
    if (player_index == -1) {
        return FAILURE;
    }

    // Remove player by shifting all players after it one position forward
    for (int i = player_index; i < world.num_players - 1; i++) {
        // Copy player data from the next position
        my_strncpy(world.players[i].name, world.players[i + 1].name, MAX_NAME_LENGTH);
        world.players[i].inventory_size = world.players[i + 1].inventory_size;
        world.players[i].level = world.players[i + 1].level;
        world.players[i].health = world.players[i + 1].health;
        
        // Copy inventory items
        for (int j = 0; j < world.players[i + 1].inventory_size; j++) {
            my_strncpy(world.players[i].inventory[j].weapon, world.players[i + 1].inventory[j].weapon, 12);
            world.players[i].inventory[j].level = world.players[i + 1].inventory[j].level;
        }
    }

    // Decrement number of players
    world.num_players--;

    return SUCCESS;
}

/**
 * @brief Add an item (weapon) to a player's inventory.
 *
 * @param player The player receiving the item.
 * @param weapon The type of item
 * @param level The level of the item
 * @return FAILURE if player or item is NULL, weapon is invalid, player's inventory is full, or level invalid. SUCCESS otherwise.
 */
int add_item(struct Player *player, const char *weapon, int level) {
    // Check if player or weapon is NULL
    if (player == NULL || weapon == NULL) {
        return FAILURE;
    }

    // Check if weapon is valid
    if (my_strncmp(weapon, "Sword", 12) != 0 && 
        my_strncmp(weapon, "Bow", 12) != 0 && 
        my_strncmp(weapon, "Ender Pearl", 12) != 0) {
        return FAILURE;
    }

    // Check if level is valid
    if (level < 1) {
        return FAILURE;
    }

    // Check if inventory is full
    if (player->inventory_size >= MAX_INVENTORY_SIZE) {
        return FAILURE;
    }

    // Add item to player's inventory
    my_strncpy(player->inventory[player->inventory_size].weapon, weapon, 12);
    player->inventory[player->inventory_size].level = level;
    player->inventory_size++;

    return SUCCESS;
}

/**
 * @brief Spawns a monster of a mob type in the world.
 *
 * @param name The mob type of the monster.
 * @return FAILURE if mob invalid, level invalid, or if world has maximum monsters, SUCCESS otherwise.
 */
int spawn_monster(const char *mob, int level) {
    // Check if mob is valid
    if (mob == NULL || 
        (my_strncmp(mob, "Enderman", 9) != 0 && 
         my_strncmp(mob, "Zombie", 9) != 0 && 
         my_strncmp(mob, "Skeleton", 9) != 0)) {
        return FAILURE;
    }

    // Check if level is valid
    if (level < 1) {
        return FAILURE;
    }

    // Check if world has maximum monsters
    if (world.num_monsters >= MAX_MONSTERS) {
        return FAILURE;
    }

    // Add monster to world
    my_strncpy(world.monsters[world.num_monsters].mob, mob, 9);
    world.monsters[world.num_monsters].level = level;
    world.num_monsters++;

    return SUCCESS;
}

/**
 * @brief Start a battle between a player and a monster.
 *
 * @param player The player fighting the monster.
 * @return FAILURE if player is NULL or there are no monsters in the world. 0 if the player wins, 1 if the monster wins.
 */
int fight_monster(struct Player *player) {
    // Check if player is NULL
    if (player == NULL) {
        return FAILURE;
    }

    // Check if there are no monsters
    if (world.num_monsters <= 0) {
        return FAILURE;
    }

    // Fight the last monster in the array
    struct Monster *monster = &world.monsters[world.num_monsters - 1];

    // Count weakness items
    int weakness_count = 0;
    const char *weakness_item = NULL;
    const char *drop_item = NULL;
    
    // Determine weakness and drop item based on monster type
    if (my_strncmp(monster->mob, "Skeleton", 9) == 0) {
        weakness_item = "Ender Pearl";
        drop_item = "Bow";
    } else if (my_strncmp(monster->mob, "Zombie", 9) == 0) {
        weakness_item = "Bow";
        drop_item = "Sword";
    } else if (my_strncmp(monster->mob, "Enderman", 9) == 0) {
        weakness_item = "Sword";
        drop_item = "Ender Pearl";
    }

    // Count weakness items in player's inventory
    for (int i = 0; i < player->inventory_size; i++) {
        if (my_strncmp(player->inventory[i].weapon, weakness_item, 12) == 0) {
            weakness_count++;
        }
    }

    // Determine battle outcome
    if ((weakness_count * player->level) > monster->level) {
        // Player wins
        // Remove monster from the world
        world.num_monsters--;
        
        // Player gets a drop item at the monster's level
        if (add_item(player, drop_item, monster->level) == SUCCESS) {
            // Increase player's level
            player->level++;
        }
        
        return 0; // Player wins
    } else {
        // Monster wins
        // Player loses health equal to monster's level
        player->health -= monster->level;
        
        // Check if player's health falls below or equal to 0
        if (player->health <= 0) {
            delete_player(player->name);
        }
        
        return 1; // Monster wins
    }
}

/**
 * @brief Simulates a fight between two players.
 *
 * @param challenger The challenging player.
 * @param opponent The opponent player.
 * @return FAILURE if either player is NULL or either player has an empty inventory or the challenger and player are the same, 0 if the challenger wins, 1 if opponent wins.
 */
int fight_player(struct Player *challenger, struct Player *opponent) {
    // Check if either player is NULL
    if (challenger == NULL || opponent == NULL) {
        return FAILURE;
    }

    // Check if either player has an empty inventory
    if (challenger->inventory_size == 0 || opponent->inventory_size == 0) {
        return FAILURE;
    }

    // Check if challenger and opponent are the same
    if (challenger == opponent) {
        return FAILURE;
    }

    // Find highest level item for each player
    int challenger_highest = 0;
    int opponent_highest = 0;

    for (int i = 0; i < challenger->inventory_size; i++) {
        if (challenger->inventory[i].level > challenger_highest) {
            challenger_highest = challenger->inventory[i].level;
        }
    }

    for (int i = 0; i < opponent->inventory_size; i++) {
        if (opponent->inventory[i].level > opponent_highest) {
            opponent_highest = opponent->inventory[i].level;
        }
    }

    struct Player *winner = NULL;
    struct Player *loser = NULL;
    int damage = 0;

    // Determine winner
    if (challenger_highest > opponent_highest) {
        winner = challenger;
        loser = opponent;
        damage = challenger_highest;
    } else if (opponent_highest > challenger_highest) {
        winner = opponent;
        loser = challenger;
        damage = opponent_highest;
    } else {
        // Tie in item levels, break by health
        if (challenger->health > opponent->health) {
            winner = challenger;
            loser = opponent;
            damage = challenger_highest;
        } else if (opponent->health > challenger->health) {
            winner = opponent;
            loser = challenger;
            damage = opponent_highest;
        } else {
            // Tie in health, challenger loses
            winner = opponent;
            loser = challenger;
            damage = opponent_highest;
        }
    }

    // Apply damage to loser
    loser->health -= damage;

    // Check if loser's health falls below or equal to 0
    if (loser->health <= 0) {
        delete_player(loser->name);
    }

    // Return result (0 if challenger wins, 1 if opponent wins)
    return (winner == challenger) ? 0 : 1;
}

/**
 * @brief Simulates a battle between the player and the Ender Dragon.
 *
 * @param player The player engaging in battle.
 * @return SUCCESS if the player wins, FAILURE if player is NULL, player has level < 100, or if they lose.
 */
int fight_ender_dragon(struct Player *player) {
    // Check if player is NULL
    if (player == NULL) {
        return FAILURE;
    }

    // Check if player's level is at least 100
    if (player->level < 100) {
        return FAILURE;
    }

    // Calculate sum of item levels
    int total_item_level = 0;
    for (int i = 0; i < player->inventory_size; i++) {
        total_item_level += player->inventory[i].level;
    }

    // Check if player wins
    if (total_item_level > ENDER_DRAGON_HEALTH) {
        // Player wins
        player->level += 25;
        player->health = PLAYER_INITIAL_HEALTH; // Restore to full health
        return SUCCESS;
    } else {
        // Player loses
        delete_player(player->name);
        return FAILURE;
    }
}

// ⣳⢽⡪⣗⢕⢕⢕⢕⢕⢕⢕⢕⢽⡪⡷⣝⡮⡷⣝⡮⡷⣝⡮⡷⣝⡮⡷⡽⡸⡸⡸⡨
// ⣳⢽⢝⡾⡸⡜⡜⡜⣜⢜⢜⢜⡽⣝⢽⢮⢯⣺⣕⢯⢯⡺⣮⣻⡺⡮⡯⡯⡪⡪⡪⡪
// ⢪⢪⢪⢪⢪⡪⡪⡪⡪⡪⡪⡣⡪⡪⡪⡪⡪⡲⡸⡸⣳⢽⣺⣪⢪⢪⢪⢪⢯⢯⢯⢯
// ⡱⡱⡱⡕⡕⡕⡕⡝⡜⡎⡮⡪⡺⡸⡪⡪⣪⢪⡪⡪⣞⢗⣗⢽⡸⡸⡱⡱⣝⢷⢝⣗
// ⢜⢜⢜⢜⠀⠀⠀⠀⠀⠀⠀⠀⢯⡫⡯⡯⡪⡪⡪⡪⠀⠀⠀⠀⠀⠀⠀⠀⢯⡫⣗⣗
// ⢪⢪⢣⡓⠀⠀⠀⠀⠀⠀⠀⠀⢯⡫⡯⡯⡪⡪⡪⡪⠀⠀⠀⠀⠀⠀⠀⠀⢯⢯⡺⡮
// ⡱⡱⡱⡱⠀⠀⠀⠀⠀⠀⠀⠀⢕⢕⢕⢕⢝⢜⢎⠮⠀⠀⠀⠀⠀⠀⠀⠀⢯⣳⡫⡯
// ⢜⢜⢎⠮⠀⠀⠀⠀⠀⠀⠀⠀⢕⢕⢕⢇⢗⢕⢕⢝⠀⠀⠀⠀⠀⠀⠀⠀⢯⢮⢯⢯
// ⢏⡯⡯⣻⢸⡸⡸⡸⡸⡸⡸⡸⠀⠀⠀⠀⠀⠀⠀⠀⢯⣺⢝⡾⡸⡸⡸⡸⡸⡸⡸⡸
// ⢗⡽⣝⡮⡣⡣⡣⡫⡪⡪⡪⡪⠀⠀⠀⠀⠀⠀⠀⠀⢯⣺⡳⣝⢎⢎⢮⢪⡪⡺⡸⡸
// ⢗⡽⡮⣻⢸⢸⢪⢪⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢕⢕⢕⢕⡽⡽⣝⢽
// ⢗⡽⣝⡮⡣⡣⡣⣓⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢕⢕⢕⢕⢯⢯⣺⢝
// ⢗⡽⡮⣻⢸⢪⢪⢪⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢕⢕⢵⢱⢕⢕⢎⢎
// ⢗⡽⣝⢾⢸⢜⢜⢜⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢕⢕⢕⢕⢕⢕⢕⢕
// ⡱⡱⡱⡱⣫⢯⡫⡯⠀⠀⠀⠀⡪⡪⡪⡪⡪⡪⡪⡪⠀⠀⠀⠀⢕⢕⢕⢵⢱⢕⢇⢇
// ⢜⢜⢜⢎⢷⢽⢝⡽⠀⠀⠀⠀⡪⡪⡪⡪⡪⡪⡪⡪⠀⠀⠀⠀⢕⢕⢕⢕⢕⢕⢕⢕
//
// hsssssssssssssssssssssss........