#pragma once
#include "vector.h"
#include "matrix.h"
typedef struct {
	vec3_t position;
	vec3_t direction;
	vec3_t forward_velocity;
	float yaw;
	float pitch;
} camera_t;

float get_camera_yaw();
float get_camera_pitch();
vec3_t get_camera_position();
vec3_t get_camera_direction();
vec3_t get_camera_lookAt_target();

void set_camera_yaw(float amount);
void set_camera_pitch(float amount);
void set_camera_position(float speed);
void set_camera_positionY(float speed);
void set_camera_direction(vec3_t direction);