// ==============================================================
// active_camera.cpp	
// アクティブカメラ制御
// 
// 制作者:村上和樹		制作日付：2025/11/20	
// 更新者:村上和樹		更新日付：2025/11/20
// ==============================================================
#include "active_camera.h"

#include <DirectXMath.h>

#include "player.h"
#include "direct3D.h"
#include "shader3D.h"
#include "keylogger.h"

ActiveCamera g_Camera({ -9.0f,4.0f, -5.0f }, { 0.0f,0.0f,0.0f });
XMMATRIX g_ViewMatrix{};

ActiveCamera::ActiveCamera(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3 target)
	:m_Position(position), m_Up({ 0.0f, 1.0f, 0.0f })
{
	//XMFLOAT⇒XMVECTORに変換して演算
	XMVECTOR xFront = XMVector3Normalize(XMLoadFloat3(&target) - XMLoadFloat3(&position));	//XMLoadFloat3(&position)XMFLOAT型をXMVECTOR型に変換して評価する()内はアドレス

	//XMVECTOR⇒XMFLOATに変換(変換先,変換元)																	//XMVector3Normalizeベクトルの長さを１にする
	XMStoreFloat3(&m_Front, xFront);

	//右方向ベクトルを作る
	XMVECTOR xRight = XMVector3Cross({ 0.0f, 1.0f, 0.0f }, xFront);
	XMStoreFloat3(&m_Right, xRight);
}

void ActiveCamera::Initialize()
{

}

void ActiveCamera::Finalize()
{

}

void ActiveCamera::Update(double elapsed_time)
{
	Player* pPlayer = GetPlayer();

	//XMFLOAT⇒XMVECTORに変換して演算
	XMVECTOR xFront = XMVector3Normalize(XMLoadFloat3(&pPlayer->GetPos()) - XMLoadFloat3(&g_Camera.GetPos()));	

	//XMVECTOR⇒XMFLOATに変換(変換先,変換元)																	
	XMStoreFloat3(&m_Front, xFront);

	//右方向ベクトルを作る
	XMVECTOR xRight = XMVector3Cross({ 0.0f, 1.0f, 0.0f }, xFront);
	XMStoreFloat3(&m_Right, xRight);
}

void ActiveCamera::SetMatrix() const
{
	XMVECTOR xTarget = XMLoadFloat3(&m_Position) + XMLoadFloat3(&m_Front);

	//ビュー変換行列の設定
	g_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), xTarget, XMLoadFloat3(&m_Up));

	Shader3D_SetViewMatrix(g_ViewMatrix);

	//プロジェクション変換行列の設定
	//視錘台(nearとfar)
	float w = static_cast<float>(Direct3D_GetBackBufferWidth());
	float h = static_cast<float>(Direct3D_GetBackBufferHeight());
	XMMATRIX m_ProjMatrix = XMMatrixPerspectiveFovLH(m_Fov, w / h, 0.1f, 100.0f);
	Shader3D_SetProjMatrix(m_ProjMatrix);
}

void ActiveCamera::Draw()
{

}

ActiveCamera* GetActiveCamera(void)
{
	return &g_Camera;
}