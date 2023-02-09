#include "camera.h"

static camera_t camera;
camera_t camera = {
	.position = {0, 0, 0},
	.direction = {0, 0, 1},
	.forward_velocity = {0.0,0.0,0.0},
	.yaw = 0.0f,
	.pitch = 0.0f
};

float get_camera_yaw()
{
	return camera.yaw;
}

float get_camera_pitch()
{
	return camera.pitch;
}

vec3_t get_camera_position()
{
	return camera.position;
}

vec3_t get_camera_direction()
{
	return camera.direction;
}

vec3_t get_camera_lookAt_target()
{
	vec3_t target = { 0,0,1.0 };
	// rotate camera based on yaw value (based on user input)
	mat4_t camera_yaw_rotation = mat4_make_rotation_y(get_camera_yaw());
	mat4_t camera_pitch_rotation = mat4_make_rotation_x(get_camera_pitch());

	mat4_t camera_rotation = mat4_make_identity();
	camera_rotation = mat4_multiply_mat4(camera_yaw_rotation, camera_rotation);
	camera_rotation = mat4_multiply_mat4(camera_pitch_rotation, camera_rotation);

	set_camera_direction(mat4_multiply_vec3(camera_rotation, target));
	target = vec3_add(get_camera_position(), get_camera_direction());
	
	return target;
}

void set_camera_yaw(float amount)
{
	camera.yaw += amount;
}
void set_camera_pitch(float amount)
{
	camera.pitch += amount;
}

void set_camera_position(float speed)
{
	camera.forward_velocity = vec3_multiply(camera.direction, speed);
	camera.position = vec3_add(camera.position, camera.forward_velocity);
}

void set_camera_positionY(float amount)
{
	camera.position.y += amount;
}

void set_camera_direction(vec3_t direction)
{
	camera.direction = direction;
}
