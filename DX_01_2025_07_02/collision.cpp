// ==============================================================
// collision.cpp	
// 当たり判定制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/05	
// 更新者:Kazuki Murakami		更新日付：2025/11/11
// ==============================================================
#include "collision.h"

#include <cmath>
#include <DirectXMath.h>

#include "texture.h"
#include "sprite.h"
#include "player.h"

using namespace DirectX;

// ===============================================
// ボックス同士の当たり判定処理
// bool CheckBoxCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, XMFLOAT2 SizeA, XMFLOAT2 SizeB)
// 
// 型：bool
// 返り値：当たっていればtrue	当たっていなければfalseを返す
// 引数１：PosA		XMFLOAT2型		矩形Ａの座標
// 引数２：PosB		XMFLOAT2型		矩形Ｂの座標
// 引数３：SizeA	XMFLOAT2型		矩形Ａのサイズ
// 引数４：SizeB	XMFLOAT2型		矩形Ｂのサイズ
// 戻り値	true：当たっている		false：当たっていない
// ===============================================
bool CheckBoxCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, XMFLOAT2 SizeA, XMFLOAT2 SizeB)
{
	float ATop = PosA.y - SizeA.y / 2;		// Aの上端
	float ABottom = PosA.y + SizeA.y / 2;	// Aの下端
	float ARight = PosA.x + SizeA.x / 2;	// Aの右端
	float ALeft = PosA.x - SizeA.x / 2;		// Aの左端

	float BTop = PosB.y - SizeB.y / 2;		// Bの上端
	float BBottom = PosB.y + SizeB.y / 2;	// Bの下端
	float BRight = PosB.x + SizeB.x / 2;	// Bの右端
	float BLeft = PosB.x - SizeB.x / 2;		// Bの左端

	if ((ARight >= BLeft) &&		// Aの右端 > Bの左端
		(ALeft <= BRight) &&		// Aの左端 < Bの右端
		(ABottom >= BTop) &&		// Aの下端 > Bの上端
		(ATop <= BBottom))			// Aの上端 < Bの下端
	{
		// 当たっている
		return true;
	}

	// 当たっていない
	return false;
}
// ===============================================
// 円同士の当たり判定処理
// bool CheckCircleCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, float rA, float rB)
// 
// 型：bool
// 返り値：当たっていればtrue	当たっていなければfalseを返す
// 引数１：PosA		XMFLOAT2型		円Ａの座標
// 引数２：PosB		XMFLOAT2型		円Ｂの座標
// 引数３：rA		float型			円Ａの半径
// 引数４：rB		float型			円Ｂの半径
// 戻り値	true：当たっている		false：当たっていない
// ===============================================
bool CheckCircleCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, float rA, float rB)
{
	// (円Aの中心座標X - 円Bの中心座標X)の2乗 + (円Aの中心座標Y - 円Bの中心座標Y)の2乗
	float distance = (PosA.x - PosB.x) * (PosA.x - PosB.x) + (PosA.y - PosB.y) * (PosA.y - PosB.y);
	
	// (円1の半径+円2の半径)の2乗
	float rSum = (rA + rB) * (rA + rB);

	// 2つの円の距離が半径の合計を下回った時
	if (distance <= rSum)
	{
		// 当たっている
		return true;
	}

	// 当たっていない
	return false;
}

// ===================================================
// 円と長方形の当たり判定
// bool CheckCircleBoxCollider(XMFLOAT2 CirclePos, float r, XMFLOAT2 BoxPos, XMFLOAT2 BoxSize)
// 
// 型：bool
// 引数1：CirclePos		DirectX::XMFLOAT2型		円の中心座標
// 引数2：r				float型					半径
// 引数3：BoxPos		DirectX::XMFLOAT2型		ボックスの座標
// 引数4：BoxSize		DirectX::XMFLOAT2型		ボックスのサイズ
// 戻り値	true：当たっている		false：当たっていない
// ===================================================
bool CheckCircleBoxCollider(XMFLOAT2 CirclePos, float r, XMFLOAT2 BoxPos, XMFLOAT2 BoxSize)
{
	if (
		// 長方形の上下を半径r分大きくした範囲の中に円の中心点が入っているか
		((CirclePos.x >= BoxPos.x) && (CirclePos.x <= BoxPos.x + BoxSize.x) && (CirclePos.y >= BoxPos.y - r ) && (CirclePos.y <= BoxPos.y + r)) ||

		// 長方形の左右を半径r分大きくした範囲の中に円の中心点が入っているか
		((CirclePos.x >= BoxPos.x  - r) && (CirclePos.x <= BoxPos.x + BoxSize.x + r) && (CirclePos.y >= BoxPos.y) && (CirclePos.y <= BoxPos.x + BoxSize.y)) ||

		// 円の中心とボックスの各頂点との距離をチェック
		((BoxPos.x - CirclePos.x) * (BoxPos.x - CirclePos.x) + (BoxPos.y - CirclePos.y) * (BoxPos.y - CirclePos.y) < (r * r))||
		((BoxPos.x + BoxSize.x - CirclePos.x) * (BoxPos.x + BoxSize.x - CirclePos.x) + (BoxPos.y - CirclePos.y) * (BoxPos.y - CirclePos.y) < (r * r)) ||
		((BoxPos.x + BoxSize.x - CirclePos.x) * (BoxPos.x + BoxSize.x - CirclePos.x) + (BoxPos.y + BoxSize.y - CirclePos.y) * (BoxPos.y + BoxSize.y - CirclePos.y) < (r * r)) ||
		((BoxPos.x - CirclePos.x) * (BoxPos.x - CirclePos.x) + (BoxPos.y + BoxSize.y - CirclePos.y) * (BoxPos.y + BoxSize.y - CirclePos.y) < (r * r)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ===================================================
// 楕円と点の当たり判定
// bool CheckEllipsePointCollider(DirectX::XMFLOAT2 EllipsePos, DirectX::XMFLOAT2 PointPos, float rX, float rY)
// 
// 型：bool
// 引数1：PointPos			DirectX::XMFLOAT2型		ボックスの座標
// 引数2：rX				float型					楕円のX方向の半径
// 引数3：rY				float型					楕円のY方向の半径
// 戻り値	true：当たっている		false：当たっていない
// ===================================================
bool CheckEllipsePointCollider(DirectX::XMFLOAT2 PointPos, float rX, float rY)
{
	float value = (PointPos.x * PointPos.x) / (rX * rX) + (PointPos.y * PointPos.y) / (rY * rY);
	return value <= 1.0f;
}

// ===================================================
// 楕円と楕円の当たり判定
// bool CheckEllipsesCollider(DirectX::XMFLOAT2 EllipseAPos, float raX, float raY, DirectX::XMFLOAT2 EllipseBPos, float rbX, float rbY)
//
// 型：bool
// 引数1：EllipseAPos		DirectX::XMFLOAT2型		楕円Aの座標
// 引数2：raX				float型					楕円AのX方向の半径
// 引数3：raY				float型					楕円AのY方向の半径
// 引数4：EllipseBPos		DirectX::XMFLOAT2型		楕円Bの座標
// 引数5：rbX				float型					楕円BのX方向の半径
// 引数6：rbY				float型					楕円BのY方向の半径
// 戻り値	true：当たっている		false：当たっていない
// ===================================================
bool CheckEllipsesCollider(DirectX::XMFLOAT2 EllipseAPos, float raX, float raY, DirectX::XMFLOAT2 EllipseBPos, float rbX, float rbY)
{
	// 2つの楕円の中心の差
	float dx = EllipseBPos.x - EllipseAPos.x;
	float dy = EllipseBPos.y - EllipseAPos.y;

	// 両方の楕円のサイズを足し合わせた「合成半径」
	float sumA = raX + raY;
	float sumB = rbX + rbY;

	// X, Y方向のスケーリング
	float nx = dx / sumA;
	float ny = dy / sumB;

	// 楕円判定式
	return (nx * nx + ny * ny) <= 1.0f;
}

// ===================================================
// 楕円と円の当たり判定
// bool CheckEllipseCircleCollider(XMFLOAT2 EllipsePos, float rEX, float rEY, XMFLOAT2 CirclePos, float Cr)
// 
// 型：bool
// 引数1：EllipsePos		DirectX::XMFLOAT2型		楕円の座標
// 引数2：rEX				float型					楕円のX方向の半径
// 引数3：rEY				float型					楕円のY方向の半径
// 引数4：CirclePos		DirectX::XMFLOAT2型			円の座標
// 引数5：Cr				float型					円の半径
// 戻り値	true：当たっている		false：当たっていない
// ===================================================
bool CheckEllipseCircleCollider(XMFLOAT2 EllipsePos, float rEX, float rEY, XMFLOAT2 CirclePos, float Cr)
{
	// 中心差
	float dx = CirclePos.x - EllipsePos.x;
	float dy = CirclePos.y - EllipsePos.y;

	// 楕円のスケーリングに合わせて正規化
	float nx = dx / (rEX + Cr);
	float ny = dy / (rEY + Cr);

	// 正規化距離を楕円式に代入
	return (nx * nx + ny * ny) <= 1.0f;
}

// ===================================================
// 楕円と長方形の当たり判定
// bool CheckEllipseRectCollision(XMFLOAT2 EllipsePos, float rEX, float rEY, XMFLOAT2 BoxPos, XMFLOAT2 BoxSize)
//
// 型：bool
// 引数1：EllipsePos		DirectX::XMFLOAT2型		楕円の座標
// 引数2：rEX				float型					楕円のX方向の半径
// 引数3：rEY				float型					楕円のY方向の半径
// 引数4：BoxPos			DirectX::XMFLOAT2型		ボックスの座標
// 引数5：BoxSize			DirectX::XMFLOAT2型		ボックスのサイズ
// 戻り値	true：当たっている		false：当たっていない
// ===================================================
bool CheckEllipseBoxCollision(XMFLOAT2 EllipsePos, float rEX, float rEY, XMFLOAT2 BoxPos, XMFLOAT2 BoxSize)
{
	// 楕円中心と矩形中心の差
	float dx = fabs(EllipsePos.x - BoxPos.x);
	float dy = fabs(EllipsePos.y - BoxPos.y);

	float hw = BoxSize.x / 2;
	float hh = BoxSize.y / 2;

	// 楕円中心が矩形の外側にある場合は、矩形端に最も近い点を取得
	float nearestX = std::max(-hw, std::min(dx, hw));
	float nearestY = std::max(-hh, std::min(dy, hh));

	// 楕円のスケーリング空間に変換（中心間距離を楕円の半径で割る）
	float nx = (dx - nearestX) / rEX;
	float ny = (dy - nearestY) / rEY;

	// 楕円式で判定
	return (nx * nx + ny * ny) <= 1.0f;
}

