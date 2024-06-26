#include "include/inventory.h"
#include <stdlib.h>

// SLOT
Slot* init_slot(int index)
{
	Slot* slot = calloc(1, sizeof(Slot));

	slot->index = index;
	slot->is_empty = true;
	slot->weapon = (void*)0;

	return slot;
}

void draw_slot(Slot* slot, Vector2 pos, Texture2D weapon_spsheet)
{
	DrawRectangle(pos.x, pos.y, SLOT_SIZE, SLOT_SIZE, RAYWHITE);
	DrawRectangleLines(pos.x, pos.y, SLOT_SIZE, SLOT_SIZE, BLACK);

	if (slot->is_empty) return;

	DrawTexturePro(weapon_spsheet, (Rectangle){ .x = slot->weapon->type * 32, .y = 0, .width = 32, .height = 16 },
					(Rectangle){ pos.x, pos.y, SLOT_SIZE, SLOT_SIZE }, (Vector2){0}, .0f, WHITE);	
}

// INVENTORY
Inventory* init_inventory(int SLOT_COUNT)
{
	Inventory* inventory = calloc(1, sizeof(Inventory));

	inventory->SLOT_COUNT = SLOT_COUNT;
	inventory->slots = calloc(SLOT_COUNT, sizeof(Slot*));
	inventory->cur_slot_i = 0;
	for (int i = 0 ; i < SLOT_COUNT; i++) {
		inventory->slots[i] = init_slot(i);
	}
	return inventory;
}

void draw_inventory(Inventory* inventory, Vector2 pos, Texture2D weapon_spsheet)
{
	for (int i = 0; i < inventory->SLOT_COUNT; i++) {
		draw_slot(inventory->slots[i], (Vector2){ pos.x + i * (SLOT_SIZE + 32), pos.y }, weapon_spsheet);
	}
}