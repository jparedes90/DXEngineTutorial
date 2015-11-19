#ifndef _CAMERA_H
#define _CAMERA_H

#include <directxmath.h>

using namespace DirectX;

class Camera
{
public:
	Camera(void);
	~Camera(void);

	void InitializeOthoMatrix(int screenWidth, int screenHeight, float screenNear, float screenFar);
	void InitializeProjectionMatrix(float fov, float screenAspect, float screenNear, float screenFar);

	void Update();

	XMVECTOR GetPosition();
	XMVECTOR GetRotation();
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	XMFLOAT4X4 GetOrthoMatrix();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

private:
	XMVECTOR m_position;
	XMVECTOR m_rotation;
	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_projectionMatrix;
	XMFLOAT4X4 m_orthoMatrix;
};

#endif