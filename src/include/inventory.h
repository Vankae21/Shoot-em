#ifndef INVENTORY_H
#define INVENTORY_H

#include <raylib.h>
#include "weapon.h"

#define SLOT_SIZE 96

typedef struct {
	int index;
	bool is_empty;
	bool is_selected;
	Weapon* weapon;
} Slot;

typedef struct {
	int SLOT_COUNT;
	Slot** slots;
	int cur_slot_i;
} Inventory;


// SLOT
Slot* init_slot(int index);
void draw_slot(Slot* slot, Vector2 pos, Texture2D weapon_spsheet);

// INVENTORY
Inventory* init_inventory(int SLOT_COUNT);
void draw_inventory(Inventory* inventory, Vector2 pos, Texture2D weapon_spsheet);

#endif