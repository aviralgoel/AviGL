#pragma once

#include<stdint.h>

typedef struct {
	float u;
	float v;
} tex2_t;

extern int texture_width;
extern int texture_height;

// raw HARD CODED texture data in 8bit format
extern const uint8_t REDBRICK_TEXTURE[];
// raw texture data in 32bit format
extern uint32_t* mesh_texture;
