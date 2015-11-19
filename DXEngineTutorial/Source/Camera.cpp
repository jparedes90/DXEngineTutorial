#include "Camera.h"

Camera::Camera(void)
{
	m_position = XMVectorSet(0, 0, 0, 0);
	m_rotation = XMVectorSet(0, 0, 0, 0);
}

Camera::~Camera(void)
{
}

void Camera::InitializeOthoMatrix(int screenWidth, int screenHeight, float screenNear, float screenFar)
{
	XMStoreFloat4x4(&m_orthoMatrix, XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenFar));
}

void Camera::InitializeProjectionMatrix(float fov, float screenAspect, float screenNear, float screenFar)
{
	XMStoreFloat4x4(&m_projectionMatrix, XMMatrixPerspectiveFovLH(fov, screenAspect, screenNear, screenFar));
}

void Camera::Update()
{
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(m_rotation.m128_f32[0], m_rotation.m128_f32[1], m_rotation.m128_f32[2]);

	//transform the lookat and up vector by rotation matrix
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	//translate the lookat
	lookAt = m_position + lookAt;

	//create view matrix
	XMStoreFloat4x4(&m_viewMatrix, XMMatrixLookAtLH(m_position, lookAt, up));
}

XMVECTOR Camera::GetPosition()
{
	return m_position;
}

XMVECTOR Camera::GetRotation()
{
	return m_rotation;
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return m_viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

XMFLOAT4X4 Camera::GetOrthoMatrix()
{
	return m_orthoMatrix;
}

void Camera::SetPosition(float x, float y, float z)
{
	m_position = XMVectorSet(x, y, z, 0);
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotation = XMVectorSet(x * (float)XM_PI/180.0f, y * (float)XM_PI/180.0f, z * (float)XM_PI/180.0f, 0);
}
