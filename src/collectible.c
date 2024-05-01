#include "include/collectible.h"
#include <stdlib.h>

Collectible* init_collectible(unsigned int type, Vector2 pos)
{
    Collectible* collectible = calloc(1, sizeof(Collectible));

    collectible->type = type;
    collectible->cir = (Circle){ .center = pos, .radius = 16 };
    collectible->is_collected = false;
    collectible->offset = 0;
    collectible->passed_time = 0;
    collectible->drop_time = 0;
    collectible->is_floating = 0;
    collectible->velocity = 0;
    collectible->dir_x = (float)(rand() % 50) + 30; collectible->dir_x *= rand() % 100 > 50 ? 1 : -1;

    return collectible;
}

void draw_collectible(Collectible* collectible, Texture2D collectible_spsheet)
{
    if(collectible->is_floating)
    {
        collectible->offset = sinf(collectible->passed_time * 4) * 4;
        collectible->passed_time += GetFrameTime();
        if(collectible->passed_time > PI)
        {
            collectible->passed_time = 0;
        }
    }
    else
    {
        if(collectible->drop_time < 0.2f)
        {
            collectible->velocity += GetFrameTime() * -350;
        }
        else if(collectible->drop_time > 0.2f && collectible->drop_time < 0.7f)
        {
            collectible->velocity += GetFrameTime() * 450;
        }
        else
        {
            collectible->velocity = 0;
            collectible->is_floating = true;
            
        }
        collectible->cir.center.y += collectible->velocity * GetFrameTime();
        collectible->cir.center.x += collectible->dir_x * GetFrameTime();
        collectible->drop_time += GetFrameTime();
    }
    

    DrawTexturePro(collectible_spsheet, (Rectangle){ .x = 16 * collectible->type, .y = 0, .width = 16, .height = 16 },
                    (Rectangle){.x = collectible->cir.center.x - collectible->cir.radius,
                                .y = collectible->cir.center.y - collectible->cir.radius - collectible->offset,
                                .width = collectible->cir.radius * 2,
                                .height = collectible->cir.radius * 2,},
                    (Vector2){ 0 }, 0.0f, WHITE
    );
}

void rerow_collectibles(Collectible*** list, unsigned int* count)
{
    int c = 0;
    Collectible** result = malloc(sizeof(Collectible*) * (*count));
    for(int i = 0; i < *count; i++)
    {
        if((*list)[i]->is_collected)
        {
            free((*list)[i]);
            continue;
        }
        c++;
        result = realloc(result, sizeof(Collectible*) * c);
        result[c-1] = (*list)[i];
    }
    free(*list);
    *count = c;
    *list = result;
}

void add_collectible(Collectible* c, Collectible*** list, unsigned int* count)
{
    (*count)++;
    *list = realloc(*list, *count * sizeof(Collectible*));
    (*list)[*count-1] = c;
}