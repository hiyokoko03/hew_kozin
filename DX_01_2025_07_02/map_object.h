// ==============================================================
// map_object.h	
// マップオブジェクト描画
// 
// 制作者:Kazuki Murakami		制作日付：2025/11/22	
// 更新者:Kazuki Murakami		更新日付：2025/11/22
// ==============================================================
#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include <DirectXMath.h>
#include <vector>
#include <memory>

#include "collision.h"
#include "map_object_shadow.h"

class MapObjectShadow;

class MapObject {
public:
	enum Type {
		None = 0,
		Sofa,
		Chair,
		CoatStand,
		Plant,
		Clock,
		Shelf,
		TV,
		Vase,
		UmbrellaStand,
		Table,
		Stair,
		Box,
		LeftDoor,
		RightDoor,
	};

private:
	bool m_IsUse = false;
	int m_ID = 0;
	Type m_Type{ None };
	DirectX::XMFLOAT3 m_Pos{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_Size{ 1.0f,1.0f,1.0f };
	DirectX::XMFLOAT3 m_Rotation{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_CollisionSize{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_CollisionOffset{ 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_CollisionPos{ 0.0f,0.0f,0.0f };

	int m_ActiveShadowIndex = -1; // -1 = 影なし
	std::vector<std::unique_ptr<MapObjectShadow>> m_pShadows;
	std::vector<DirectX::XMFLOAT3> m_ShadowOffsets;
	std::vector<DirectX::XMFLOAT3> m_ShadowPos;

public:
	MapObject(const MapObject& other);
	MapObject& operator=(const MapObject& other);

	MapObject(MapObject&&) = default;
	MapObject& operator=(MapObject&&) = default;

	MapObject(const DirectX::XMFLOAT3& pos, Type type, const DirectX::XMFLOAT3& size, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& CollisionSize, const DirectX::XMFLOAT3& CollisonOffset);

	~MapObject();

	void Initialize(void);
	void Finalize(void);
	void Update(double elapsed_time);
	void Draw(void)const;

	// ゲッターセッター
	const bool& GetIsUse(void) const { return m_IsUse; }
	void SetIsUse(bool use) { m_IsUse = use; }
	const int& GetID(void) const { return m_ID; }
	void SetID(int id) { m_ID = id; }
	const Type& GetType() const { return m_Type; }
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

	const DirectX::XMFLOAT3 GetShadowPos() const;
	

	// Initializeより前に呼び出すこと
	void AddShadow(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& size, bool use);
	bool IsPlayerInShadow(const DirectX::XMFLOAT3& playerPos) const;
	
	bool HasShadow() const;
	
	const std::vector<MapObject>& GetMapObjects(void);
};

MapObject* GetObjectInShadow(const DirectX::XMFLOAT3& playerPos);

MapObject* GetMapObject(void);
#endif