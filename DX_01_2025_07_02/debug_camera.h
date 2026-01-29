// ==============================================================
// debug_camera.h	
// デバッグカメラ制御
// 
// 制作者:村上和樹		制作日付：2025/10/22	
// 更新者:村上和樹		更新日付：2025/10/22
// ==============================================================
#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H

#include <DirectXMath.h>

#include "camera.h"

class DebugCamera : public Camera{
private:
	DirectX::XMFLOAT3 m_Front{};				//前方向ベクトル
	DirectX::XMFLOAT3 m_Right{};				//右方向ベクトル
	DirectX::XMFLOAT3 m_Up{ 0.0f, 1.0f, 0.0f };	//カメラの上方向
	DirectX::XMFLOAT3 m_Position{};				//カメラの目の位置
	float m_Fov{ 1.0f };

	

public:
	DebugCamera(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3 target);

	void Initialize()override {}
	void Finalize()override {}
	void Update(double elapsed_time) override;
	void Draw()override {}

	virtual void SetMatrix() const override;

	const DirectX::XMFLOAT3& GetPos(void) const { return m_Position; }
	void SetPos(DirectX::XMFLOAT3 Pos) { m_Position = Pos; }

	const DirectX::XMFLOAT3& GetFront(void) const { return m_Front; }
	void SetFront(DirectX::XMFLOAT3 FrontPos) { m_Front = FrontPos; }

	const DirectX::XMFLOAT3& GetRight(void) const { return m_Right; }
	void SetRight(DirectX::XMFLOAT3 RightPos) { m_Right = RightPos; }

	const DirectX::XMFLOAT3& GetUp(void) const { return m_Up; }
	void SetUp(DirectX::XMFLOAT3 UpPos) { m_Up = UpPos; }

	const float GetFov(void) const { return m_Fov; }
	void SetFov(float Fov) { m_Fov = Fov; }

};

DebugCamera* GetDebugCamera(void);

#endif

