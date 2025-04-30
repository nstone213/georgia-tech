#ifndef MINECRAFT_V2_H
#define MINECRAFT_V2_H

#include "suites/fakemalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This is just to remove compiler warnings associated with unused variables.
// Delete calls to this as you implement functions.
#define UNUSED(x) ((void)(x))

#define SUCCESS 0
#define FAILURE -1
#define INCOMPLETE -2

enum Mob {
    SKELETON,
    ZOMBIE,
    ENDERMAN,
    CREEPER
};

typedef struct Player {
    char *name;
    int level;
} Player;

typedef struct Monster {
    enum Mob mob;
    int level;
} Monster;

typedef struct Node {
    struct Node *next;
    Monster *monster;
} Node;

typedef struct LinkedList {
    Node *head;
    int size;
} LinkedList;

typedef struct World {
    LinkedList monsters;
    Player **players;
    int num_players;
} World;

World *create_world(void);
int create_player(World *world, const char *username, int level);
int create_monster(World *world, enum Mob mob, int level);

Player *find_player(World *world, char *name);
int fight_monster(World *world, char *name);

int delete_player(World *world, char *name);
int delete_world(World *world);

#endif