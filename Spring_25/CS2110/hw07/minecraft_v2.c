/**
 * CS 2110 - Spring 2025 - Homework 7
 *
 * @author Nicholas Stone
 *
 * minecraft_v2.c
 */

#include "minecraft_v2.h"

World *create_world(void) {
    World *world_new = (World *) malloc(sizeof(World));
    if (world_new == NULL) {
        return NULL;
    }
    
    world_new->players = NULL;
    world_new->num_players = 0;
    
    world_new->monsters.head = NULL;
    world_new->monsters.size = 0;
    
    return world_new;
}

int create_player(World *world, const char *name, int level) {
    if (world == NULL || name == NULL) {
        return FAILURE;
    }
    
    if (level <= 0) {
        return FAILURE;
    }
    
    for (int i = 0; i < world->num_players; i++) {
        if (strcmp(world->players[i]->name, name) == 0) {
            return FAILURE;
        }
    }
    
    Player *player_new = (Player *) malloc(sizeof(Player));
    if (player_new == NULL) {
        return FAILURE;
    }
    
    player_new->name = (char *) malloc(strlen(name) + 1);
    if (player_new->name == NULL) {
        free(player_new);
        return FAILURE;
    }
    
    strcpy(player_new->name, name);
    player_new->level = level;
    
    Player **players_new = (Player **) realloc(world->players, (world->num_players + 1) * sizeof(Player *));

    if (players_new == NULL) {
        free(player_new->name);
        free(player_new);
        return FAILURE;
    }
    
    world->players = players_new;
    world->players[world->num_players] = player_new;
    world->num_players++;
    return SUCCESS;
}

int create_monster(World *world, enum Mob mob, int level) {
    if (world == NULL) {
        return FAILURE;
    }
    
    if (level <= 0) {
        return FAILURE;
    }
    
    Monster *monster_new = (Monster *) malloc(sizeof(Monster));

    if (monster_new == NULL) {
        return FAILURE;
    }
    
    monster_new->mob = mob;
    monster_new->level = level;
    
    Node *node_new = (Node *) malloc(sizeof(Node));
    if (node_new == NULL) {
        free(monster_new);
        return FAILURE;
    }
    
    node_new->monster = monster_new;
    node_new->next = NULL;
    
    if (world->monsters.head == NULL) {
        world->monsters.head = node_new;
    } else {
        Node *curr = world->monsters.head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = node_new;
    }
    
    world->monsters.size++;
    return SUCCESS;
}

Player *find_player(World *world, char *name) {
    if (world == NULL || name == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < world->num_players; i++) {
        if (strcmp(world->players[i]->name, name) == 0) {
            return world->players[i];
        }
    }
    
    return NULL;
}

int fight_monster(World *world, char *name) {
    if (world == NULL || name == NULL) {
        return FAILURE;
    }
    
    if (world->monsters.head == NULL) {
        return FAILURE;
    }
    
    Player *player = find_player(world, name);
    if (player == NULL) {
        return FAILURE;
    }
    
    Node *monster_node = world->monsters.head;
    Monster *monster = monster_node->monster;
    
    if (player->level >= monster->level) {
        world->monsters.head = monster_node->next;
        free(monster);
        free(monster_node);
        world->monsters.size--;
        player->level++;
    } else {
        monster->level++;
        if (monster->mob == CREEPER) {
            player->level = 1;
        } else {
            delete_player(world, name);
        }
    }

    return SUCCESS;
}

int delete_player(World *world, char *name) {
    if (world == NULL || name == NULL) {
        return FAILURE;
    }
    
    int player_idx = -1;
    for (int i = 0; i < world->num_players; i++) {
        if (strcmp(world->players[i]->name, name) == 0) {
            player_idx = i;
            break;
        }
    }
    
    if (player_idx == -1) {
        return FAILURE;
    }
    
    free(world->players[player_idx]->name);
    free(world->players[player_idx]);
    
    for (int i = player_idx; i < world->num_players - 1; i++) {
        world->players[i] = world->players[i + 1];
    }
    
    world->num_players--;
    
    if (world->num_players > 0) {
        Player **players_new = (Player **) realloc(world->players, world->num_players * sizeof(Player *));
        if (players_new != NULL) {
            world->players = players_new;
        }
    } else {
        free(world->players);
        world->players = NULL;
    }
    
    return SUCCESS;
}

int delete_world(World *world) {
    if (world == NULL) {
        return FAILURE;
    }
    
    for (int i = 0; i < world->num_players; i++) {
        free(world->players[i]->name);
        free(world->players[i]);
    }
    
    free(world->players);
    
    Node *curr = world->monsters.head;
    while (curr != NULL) {
        Node *next = curr->next;
        free(curr->monster);
        free(curr);
        curr = next;
    }
    
    free(world);
    return SUCCESS;
}