// ==============================================================
// debug_camera.h	
// デバッグカメラ制御
// 
// 制作者:村上和樹		制作日付：2025/10/22	
// 更新者:村上和樹		更新日付：2025/11/18
// ==============================================================
#include "debug_camera.h"

#include <DirectXMath.h>

#include "direct3D.h"
#include "shader3D.h"
#include "keylogger.h"

using namespace DirectX;

DebugCamera g_Camera({ 10.0f,10.0f, -10.0f }, { 0.0f,0.0f,0.0f });

DebugCamera::DebugCamera(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3 target)
	:m_Position(position), m_Up({ 0.0f, 1.0f, 0.0f })
{
	//XMFLOAT⇒XMVECTORに変換して演算
	XMVECTOR xFront = XMVector3Normalize(XMLoadFloat3(&target) - XMLoadFloat3(&position));	//XMLoadFloat3(&position)XMFLOAT型をXMVECTOR型に変換して評価する()内はアドレス

	//XMVECTOR⇒XMFLOATに変換(変換先,変換元)																	//XMVector3Normalizeベクトルの長さを１にする
	XMStoreFloat3(&m_Front, xFront);

	//右方向ベクトルを作る
	XMVECTOR xRight = XMVector3Cross( { 0.0f, 1.0f, 0.0f }, xFront);
	XMStoreFloat3(&m_Right, xRight);
}

void DebugCamera::Update(double elapsed_time)
{
	//const float MOVE_SPEED = static_cast<float>(1.0f * elapsed_time);
	//const float ROTATION_SPEED = static_cast<float>(XMConvertToRadians(60)* elapsed_time);

	//XMVECTOR xFront = XMLoadFloat3(&m_Front);
	//XMVECTOR xRight = XMLoadFloat3(&m_Right);
	//XMVECTOR xUp = XMLoadFloat3(&m_Up);
	//
	//
	//if (KeyLogger_IsPressed(KK_R))	//Upベクトルリセット
	//{
	//	XMFLOAT3 ResetUp = { 0.0f,1.0f,0.0f };
	//	xUp = XMLoadFloat3(&ResetUp);
	//	xRight = (XMVector3Cross(xUp, xFront));
	//}

	////回転
	//if (KeyLogger_IsPressed(KK_RIGHT))	//右回転
	//{
	//	XMMATRIX Rotation = XMMatrixRotationY(ROTATION_SPEED);
	//	xFront = XMVector3Normalize(XMVector3TransformNormal(xFront, Rotation));
	//	xUp = XMVector3Normalize(XMVector3TransformNormal(xUp, Rotation));
	//	xRight = XMVector3Normalize(XMVector3Cross(xUp, xFront));
	//}

	//if (KeyLogger_IsPressed(KK_LEFT))	//左回転
	//{
	//	XMMATRIX Rotation = XMMatrixRotationY(-ROTATION_SPEED);
	//	xFront = XMVector3Normalize(XMVector3TransformNormal(xFront, Rotation));
	//	xUp = XMVector3Normalize(XMVector3TransformNormal(xUp, Rotation));
	//	xRight = XMVector3Normalize(XMVector3Cross(xUp, xFront));
	//}

	//if (KeyLogger_IsPressed(KK_UP))		//上を見る
	//{
	//	XMMATRIX Rotation = XMMatrixRotationAxis(xRight, -ROTATION_SPEED);
	//	xFront = XMVector3Normalize(XMVector3TransformNormal(xFront, Rotation));
	//	xRight = XMVector3Normalize(XMVector3Cross(xUp, xFront));
	//}

	//if (KeyLogger_IsPressed(KK_DOWN))	//下を見る
	//{
	//	XMMATRIX Rotation = XMMatrixRotationAxis(xRight, ROTATION_SPEED);
	//	xFront = XMVector3Normalize(XMVector3TransformNormal(xFront, Rotation));
	//	xRight = XMVector3Normalize(XMVector3Cross(xUp, xFront));
	//}

	//if (KeyLogger_IsPressed(KK_Q))	//右回転
	//{
	//	XMMATRIX Rotation = XMMatrixRotationAxis(xFront, ROTATION_SPEED);
	//	xUp = XMVector3Normalize(XMVector3TransformNormal(xUp, Rotation));
	//	xRight = XMVector3Normalize(XMVector3Cross(xUp, xFront));
	//}

	//if (KeyLogger_IsPressed(KK_E))	//左回転
	//{
	//	XMMATRIX Rotation = XMMatrixRotationAxis(xFront, -ROTATION_SPEED);
	//	xUp = XMVector3Normalize(XMVector3TransformNormal(xUp, Rotation));
	//	xRight = XMVector3Normalize(XMVector3Cross(xUp, xFront));
	//}

	//XMStoreFloat3(&m_Front, xFront);
	//XMStoreFloat3(&m_Right, xRight);
	//XMStoreFloat3(&m_Up, xUp);

	////前後左右上下移動
	//XMVECTOR xPosition = XMLoadFloat3(&m_Position);

	//if (KeyLogger_IsPressed(KK_W))	//前進
	//{
	//	xPosition += XMLoadFloat3(&m_Front) * MOVE_SPEED;
	//}

	//if (KeyLogger_IsPressed(KK_S))	//後退
	//{
	//	xPosition -= XMLoadFloat3(&m_Front) * MOVE_SPEED;
	//}

	//if (KeyLogger_IsPressed(KK_D))
	//{
	//	xPosition += XMLoadFloat3(&m_Right) * MOVE_SPEED;
	//}

	//if (KeyLogger_IsPressed(KK_A))
	//{
	//	xPosition -= XMLoadFloat3(&m_Right) * MOVE_SPEED;
	//}

	//if (KeyLogger_IsPressed(KK_SPACE))	//上昇
	//{
	//	xPosition += XMVECTOR{ 0.0f,MOVE_SPEED,0.0f };
	//}

	//if (KeyLogger_IsPressed(KK_LEFTCONTROL))	//下降
	//{
	//	xPosition -= XMVECTOR{ 0.0f,MOVE_SPEED,0.0f };
	//}

	////画角
	//if (KeyLogger_IsPressed(KK_Z))
	//{
	//	m_Fov += 0.1f;
	//}

	//if (KeyLogger_IsPressed(KK_X))
	//{
	//	m_Fov -= 0.1f;
	//}

	//XMStoreFloat3(&m_Position, xPosition);
}

void DebugCamera::SetMatrix() const
{
	XMVECTOR xTarget = XMLoadFloat3(&m_Position) + XMLoadFloat3(&m_Front);

	//ビュー変換行列の設定
	XMMATRIX m_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), xTarget, XMLoadFloat3(&m_Up));

	Shader3D_SetViewMatrix(m_ViewMatrix);

	//プロジェクション変換行列の設定
	//視錘台(nearとfar)
	float w = static_cast<float>(Direct3D_GetBackBufferWidth());
	float h = static_cast<float>(Direct3D_GetBackBufferHeight());
	XMMATRIX m_ProjMatrix = XMMatrixPerspectiveFovLH (m_Fov, w / h, 0.1f, 100.0f);
	Shader3D_SetProjMatrix(m_ProjMatrix);
}

DebugCamera* GetDebugCamera(void)
{
	return &g_Camera;
}
