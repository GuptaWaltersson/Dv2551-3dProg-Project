#include "camera.hpp"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_cameraPosition = { x,y,z };
}

void Camera::move(float x, float y, float z)
{
	m_cameraPosition.x += x;
	m_cameraPosition.y += y;
	m_cameraPosition.z += z;
}

void Camera::rotate(float pitch, float yaw)
{
	m_pitch += pitch;
	m_yaw += yaw;
}

DirectX::XMMATRIX Camera::getViewMatrix() const
{
	

	DirectX::XMVECTOR forward = DirectX::XMVectorSet(
		cos(m_pitch) * sin(m_yaw),
		sin(m_pitch),
		cos(m_pitch) * cos(m_yaw),
		0
	);

	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_cameraPosition);

	DirectX::XMVECTOR target = DirectX::XMVectorAdd(pos, forward);
	DirectX::XMVECTOR upDir = { 0,1,0,0 };

	return DirectX::XMMatrixLookAtLH(pos, target, upDir);
}

DirectX::XMMATRIX Camera::getProjectionMatrix(float aspectRatio) const
{
	return DirectX::XMMatrixPerspectiveFovLH(m_fov,aspectRatio,m_nearPlane,m_farPlane);
}
