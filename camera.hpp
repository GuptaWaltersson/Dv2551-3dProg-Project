#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
class Camera
{
public:
	Camera();
	~Camera();
	
	void SetPosition(float x, float y, float z);
	void move(float x, float y, float z);
	void rotate(float pitch, float yaw);

	DirectX::XMMATRIX getViewMatrix() const;
	DirectX::XMMATRIX getProjectionMatrix(float aspectRatio) const;
private:
	DirectX::XMFLOAT3 m_cameraPosition = { 0.0f,0.0f,0.0f };
	float m_pitch = 0;
	float m_yaw = 0;
	float m_fov = DirectX::XM_PIDIV4;
	float m_nearPlane = 0.1f;
	float m_farPlane = 200.0f;

};