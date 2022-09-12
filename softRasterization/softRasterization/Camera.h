#pragma once
#include"glm\glm.hpp"
#include"Geometry.h"
#include"Transform.h"


class Camera
{
public:
	Camera(
		glm::vec3 position = glm::vec3(-2.0f, 5.0f, -5.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3 lookat = glm::vec3(0.0f, 0.0f, 0.0f),
		float fov = 60.0f,
		int w = 800,
		int h = 600
	) :
		Position(position), WorldUp(up), Fov(glm::radians(fov)), Aspect((float)w / h), Pitch(0), Yaw(0)
	{
		Front = glm::normalize(lookat - Position);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));

		Pitch = glm::degrees(asin(Front.y)); 

		glm::vec3 XOZ = glm::normalize(glm::vec3(Front.x, 0, Front.z)); //先投影到xoz归一化计算，否则会出错
		Yaw = glm::degrees(acos(XOZ.x));
		if (XOZ.z < 0)
			Yaw = 360 -Yaw;
	}
	glm::mat4 ViewMatrix()
	{
		//return glm::lookAt(Position, Position + Front, Up);
		return  GeometryStage::getViewMatrix(Position, Front, Right, Up);
	}
	glm::mat4 PerspectiveMatrix()
	{
		//return glm::perspective(Fov, Aspect, 0.3f, 100.0f);
		return GeometryStage::getPerspectiveMatrix(Fov, Aspect, 0.3f, 100);
	}
	void UpdateFov(float fov = 60.0f) {
		Fov = glm::radians(fov);
	}
	void UpdateAspect(int w, int h) {
		Aspect = (float)w / h;
	}
	void MoveForward(float distance) {
		Position += Front * distance;
	}
	void MoveRight(float distance) {
		Position += Right * distance;
	}
	void MoveUp(float distance) {
		Position += Up * distance;
	}
	void RotatePitch(float angle) {
		Pitch += angle;
		if (Pitch > 89.0)
			Pitch = 89.0;
		if (Pitch < -89.0)
			Pitch = -89.0;
		UpdateCameraVectors();
	}
	void RotateYaw(float angle) {
		Yaw += angle;
		if (Yaw > 360)
			Yaw = 0;
		if (Yaw < 0)
			Yaw = 360;
		UpdateCameraVectors();
	}
private:
	void UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Fov;
	float Aspect;

	float Pitch;  //Yaw,Pitch 位于右手坐标系的球中
	float Yaw;
};