// ==============================================================
// switch.h	
// スイッチ制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/12/15	
// 更新者:Kazuki Murakami		更新日付：2025/12/15
// ==============================================================
#ifndef SWITCH_H
#define SWITCH_H

#include <DirectXMath.h>
#include "object.h"

static constexpr int SWITCH_MAX = 2;

class Switch {
private:
	int m_TextureID{};
	bool m_IsUse = false;
	int m_ID = 0;
	DirectX::XMFLOAT3 m_Pos{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_Size{ 1.0f,1.0f,1.0f };
	DirectX::XMFLOAT3 m_Rotation{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_CollisionSize{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_CollisionOffset{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_CollisionPos{ 0.0f,0.0f,0.0f };

public:
	Switch(bool use, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& size, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& CollisionSize, const DirectX::XMFLOAT3& CollisonOffset)
		:m_TextureID(-1), m_IsUse(use), m_Pos(pos), m_Size(size), m_Rotation(rotation), m_CollisionSize(CollisionSize), m_CollisionOffset(CollisonOffset)
	{

	}

	void Initialize(void);
	void Finalize(void);
	void Update(double elapsed_time);
	void Draw(void)const;

	const bool& GetIsUse(void) const { return m_IsUse; }
	void SetIsUse(bool use) { m_IsUse = use; }

	const int& GetID(void) const { return m_ID; }
	void SetID(int id) { m_ID = id; }

	const DirectX::XMFLOAT3& GetPos() const { return m_Pos; }
	void SetPos(const DirectX::XMFLOAT3& pos) { m_Pos = pos; }

	const DirectX::XMFLOAT3& GetSize() const { return m_Size; }
	void SetSize(const DirectX::XMFLOAT3& size) { m_Size = size; }

	const DirectX::XMFLOAT3& GetRotation()const { return m_Rotation; }
	void SetRotation(DirectX::XMFLOAT3 Rotation) { m_Rotation = Rotation; }

	const DirectX::XMFLOAT3& GetCollisionSize() const { return m_CollisionSize; }
	void SetCollisionSize(const DirectX::XMFLOAT3& CollisionSize) { m_CollisionSize = CollisionSize; }

	const DirectX::XMFLOAT3& GetCollisionOffset() const { return m_CollisionOffset; }
	void SetCollisionOffset(const DirectX::XMFLOAT3& CollisionOffset) { m_CollisionOffset = CollisionOffset; }

	const DirectX::XMFLOAT3& GetCollisionPos() const { return m_CollisionPos; }
	void SetCollisionPos(const DirectX::XMFLOAT3& CollisionPos) { m_CollisionPos = CollisionPos; }
};

Switch* GetSwitch();

#endif
