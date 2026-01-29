// ==============================================================
// object.h	
// すべての3Dオブジェクトの基底クラス
// 
// 制作者:Kazuki Murakami		制作日付：2025/10/30	
// 更新者:Kazuki Murakami		更新日付：2025/11/18
// ==============================================================
#ifndef OBJECT_H
#define OBJECT_H

#include <DirectXMath.h>

#include "collision.h"

// ===============================================
// 列挙体宣言
// ===============================================
enum ObjectTag {
	OBJECT_NONE = 0,
	OBJECT_PLAYER,
	OBJECT_CAT,
	OBJECT_BLOCK,
	OBJECT_BACKGROUND,
	OBJECT_ENEMY,
};

// ===============================================
// クラス宣言
// ===============================================
class GameObject {
protected:
	ObjectTag m_Tag = OBJECT_NONE;
	bool m_Use = false;
	DirectX::XMFLOAT3 m_Pos = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_OldPos = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_Vel = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_Size = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,1.0f };
	Collision m_Collision;

public:
	GameObject() : m_Tag(OBJECT_NONE),m_Use(false),m_Pos({ 0.0f, 0.0f, 0.0f }),m_OldPos(m_Pos),m_Vel({ 0.0f, 0.0f, 0.0f }),m_Size({ 0.0f, 0.0f, 0.0f }) {}

	virtual void Initialize(void) {};
	virtual void Finalize(void) {};
	virtual void Update(double elpased_time) {};
	virtual void Draw(void) {};

	const ObjectTag& GetTag(void) const { return m_Tag; }
	void SetTag(ObjectTag tag) { m_Tag = tag; }

	const bool& GetIsUse(void) const { return m_Use; }
	void SetIsUse(bool use) { m_Use = use; }

	const DirectX::XMFLOAT3& GetPos(void) const { return m_Pos; }
	void SetPos(DirectX::XMFLOAT3 pos) { m_Pos = pos; }

	const DirectX::XMFLOAT3& GetOldPos(void) const { return m_OldPos; }
	void SetOldPos(DirectX::XMFLOAT3 OldPos) { m_OldPos = OldPos; }

	const DirectX::XMFLOAT3& GetVel(void) const { return m_Vel; }
	void SetVel(DirectX::XMFLOAT3 vel) { m_Vel = vel; }

	const DirectX::XMFLOAT3& GetSize(void) const { return m_Size; }
	void SetSize(DirectX::XMFLOAT3 size) { m_Size = size; }
};

#endif