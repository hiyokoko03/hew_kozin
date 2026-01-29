// ==============================================================
// active_camera.h	
// アクティブカメラ制御
// 
// 制作者:村上和樹		制作日付：2025/11/20	
// 更新者:村上和樹		更新日付：2025/11/20
// ==============================================================
#ifndef ACTIVE_CAMERA_H
#define ACTIVE_CAMERA_H

#include <DirectXMath.h>

#include "camera.h"

class ActiveCamera : public Camera {
private:
	DirectX::XMFLOAT3 m_Front{};				//前方向ベクトル
	DirectX::XMFLOAT3 m_Right{};				//右方向ベクトル
	DirectX::XMFLOAT3 m_Up{ 0.0f, 1.0f, 0.0f };	//カメラの上方向
	DirectX::XMFLOAT3 m_Position{};				//カメラの目の位置
	DirectX::XMFLOAT3 m_Target{};				//カメラの注視点位置
	float m_Fov{ 0.6f };

public:
	ActiveCamera(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3 target);

	void Initialize()override;
	void Finalize()override;
	void Update(double elapsed_time) override;
	void Draw()override;

	virtual void SetMatrix() const override;

	const DirectX::XMFLOAT3& GetPos(void) const { return m_Position; }
	void SetPos(DirectX::XMFLOAT3 Pos) { m_Position = Pos; }

	const DirectX::XMFLOAT3& GetFront(void) const { return m_Front; }
	void SetFront(DirectX::XMFLOAT3 FrontPos) { m_Front = FrontPos; }

	const DirectX::XMFLOAT3& GetRight(void) const { return m_Right; }
	void SetRight(DirectX::XMFLOAT3 RightPos) { m_Right = RightPos; }

	const DirectX::XMFLOAT3& GetUp(void) const { return m_Up; }
	void SetUp(DirectX::XMFLOAT3 UpPos) { m_Up = UpPos; }

	const DirectX::XMFLOAT3& GetTarget(void) { return m_Target; }

	const float GetFov(void) const { return m_Fov; }
	void SetFov(float Fov) { m_Fov = Fov; }

};

ActiveCamera* GetActiveCamera(void);

#endif // !ACTIVE_CAMERA_H