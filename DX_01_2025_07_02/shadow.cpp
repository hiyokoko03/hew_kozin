// ==============================================================
// shadow.cpp
// 影クラス
// 
// 制作者:Kazuki Murakami   制作日付：2025/11/10	
// 更新者:Kazuki Murakami	更新日付：2025/11/27
// ==============================================================
#include "shadow.h"

#include <DirectXMath.h>

#include "player.h"
#include "map.h"
#include "map_object.h"
#include "map_object_shadow.h"
#include "lamp.h"
#include "active_camera.h"
#include "timer.h"
#include "easing.h"
#include "texture.h"
#include "sprite.h"
#include "keylogger.h"

using namespace DirectX;

// ===============================================
// 定数定義
// ===============================================
static constexpr float GEUGE_VALUE_SEC = 5.0f;
static constexpr float GAUGE_MAX = 100.0f;

static constexpr float MOVE_DISTANCE = 1.0f;
static constexpr double MOVE_TIME = 0.4;   // 移動にかかる時間（秒）
// ===============================================
// 内部構造体
// ===============================================
struct ShadowMove
{
	bool moving = false;
	double elapsed = 0.0;

	XMFLOAT3 ObjectStartPos{};
	XMFLOAT3 ObjectTargetPos{};
	XMFLOAT3 ShadowStartPos{};
	XMFLOAT3 ShadowTargetPos{};
	XMFLOAT3 CameraStartPos{};
	XMFLOAT3 CameraTargetPos{};
};

// ===============================================
// グローバル変数
// ===============================================
namespace
{
	int GaugeTextureID = -1;

	Timer g_SharowChangeCooldown;

	Shadow shadow;
	ShadowMove g_ShadowMove;
	
	ActiveCamera* pCam = GetActiveCamera();
	Player* pPlayer = GetPlayer();
	Lamp* pLamp = GetLamp();
}

void Shadow::Initialize(void)
{
	GaugeTextureID = Texture_Load(L"resource/texture/gauge.png");

	m_Active = false;
	m_Gauge = 100.0f;
	m_GaugeLimit = 0.0f;
}

void Shadow::Finalize(void)
{

}

void Shadow::Update(double elpased_time)
{
	g_SharowChangeCooldown.Update(elpased_time);

	// 影の中にいて、かつその影に紐づくオブジェクトがあれば取得
	MapObject* target = GetObjectInShadow(pPlayer->GetPos());

	if (target)
	{
		m_Color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	}
	else
	{
		m_Color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	if (KeyLogger_IsPressed(KK_LEFTSHIFT) && target && m_Active == false && g_SharowChangeCooldown.IsFinished()) // シフトで影化
	{
		if (m_Gauge > 0)
		{
			g_SharowChangeCooldown.Start(1.0f);
			m_Active = true;
			pPlayer->SetForms(pPlayer->FORM_SHADOW);
			
			pPlayer->SetPos(XMFLOAT3(target->GetShadowPos().x, pPlayer->GetPos().y, target->GetShadowPos().z));
			pCam->SetPos({ pPlayer->GetPos().x + 5.0f, pPlayer->GetPos().y + 4.0f,pPlayer->GetPos().z - 5.0f });
		}
	}
	if (KeyLogger_IsPressed(KK_LEFTSHIFT) && m_Active == true && g_SharowChangeCooldown.IsFinished()) // シフトで影化
	{
		g_SharowChangeCooldown.Start(1.0f);
		m_Active = false; // 影解除
		pPlayer->SetForms(pPlayer->FORM_IDLE);
	}

	if (m_Active == true)
	{
		m_Gauge -= static_cast<float>(GEUGE_VALUE_SEC * elpased_time); // 秒ごとに減少

		if (m_Gauge <= 0)
		{
			m_Gauge = 0.0;
			m_Active = false;

			pPlayer->SetForms(pPlayer->FORM_IDLE);
			pPlayer->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		}
		
		//座標をプレイヤーが影に変身している物に固定する
		if (target)
		{
			switch (target->GetType())
			{

			case MapObject::Shelf:
				pPlayer->SetForms(pPlayer->FORM_SHADOW);

				break;

			case MapObject::Stair:

				// 影オブジェクト移動開始
				if (!g_ShadowMove.moving)
				{
					XMFLOAT3 dir{ 0,0,0 };

					if (KeyLogger_IsTrigger(KK_D) && (pLamp + Lamp1)->GetIsUse() == true)
					{
						dir.x += MOVE_DISTANCE;

					}
					if (KeyLogger_IsTrigger(KK_A) && (pLamp + Lamp1)->GetIsUse() == true)
					{
						dir.x -= MOVE_DISTANCE;
					}
					if (KeyLogger_IsTrigger(KK_W) && (pLamp + Lamp2)->GetIsUse() == true)
					{
						dir.z += MOVE_DISTANCE;
					}
					if (KeyLogger_IsTrigger(KK_S) && (pLamp + Lamp2)->GetIsUse() == true)
					{
						dir.z -= MOVE_DISTANCE;
					}

					if (dir.x != 0 || dir.z != 0)
					{
						g_ShadowMove.moving = true;
						g_ShadowMove.elapsed = 0.0;
						g_ShadowMove.ObjectStartPos = target->GetPos();
						g_ShadowMove.ObjectTargetPos =
						{
							target->GetPos().x + dir.x,
							target->GetPos().y,
							target->GetPos().z + dir.z
						};

						g_ShadowMove.ShadowStartPos = target->GetPos();
						g_ShadowMove.ShadowTargetPos =
						{
							target->GetPos().x + dir.x,
							target->GetPos().y,
							target->GetPos().z + dir.z
						};

						g_ShadowMove.CameraStartPos = pCam->GetPos();
						g_ShadowMove.CameraTargetPos =
						{
							pCam->GetPos().x + dir.x,
							pCam->GetPos().y,
							pCam->GetPos().z + dir.z
						};
					}
				}

				if (g_ShadowMove.moving)
				{
					g_ShadowMove.elapsed += elpased_time;

					double t = std::min(g_ShadowMove.elapsed / MOVE_TIME, 1.0);
					float e = easeInQuart(t);

					XMFLOAT3 ObjectPos{};
					ObjectPos.x = g_ShadowMove.ObjectStartPos.x + (g_ShadowMove.ObjectTargetPos.x - g_ShadowMove.ObjectStartPos.x) * e;
					ObjectPos.y = g_ShadowMove.ObjectStartPos.y;
					ObjectPos.z = g_ShadowMove.ObjectStartPos.z + (g_ShadowMove.ObjectTargetPos.z - g_ShadowMove.ObjectStartPos.z) * e;

					XMFLOAT3 ShadowPos{};
					ShadowPos.x = g_ShadowMove.ShadowStartPos.x + (g_ShadowMove.ShadowTargetPos.x - g_ShadowMove.ShadowStartPos.x) * e;
					ShadowPos.y = g_ShadowMove.ShadowStartPos.y;
					ShadowPos.z = g_ShadowMove.ShadowStartPos.z + (g_ShadowMove.ShadowTargetPos.z - g_ShadowMove.ShadowStartPos.z) * e;

					XMFLOAT3 pos{};
					pos.x = g_ShadowMove.CameraStartPos.x + (g_ShadowMove.CameraTargetPos.x - g_ShadowMove.CameraStartPos.x) * e;
					pos.y = g_ShadowMove.CameraStartPos.y;
					pos.z = g_ShadowMove.CameraStartPos.z + (g_ShadowMove.CameraTargetPos.z - g_ShadowMove.CameraStartPos.z) * e;

					target->SetPos(ObjectPos);
					target->SetPos(ShadowPos);

					// カメラ追従
					pCam->SetPos(pos);

					if (t >= 1.0)
					{
						g_ShadowMove.moving = false;
					}
				}
				break;
			}

			pPlayer->SetPos(XMFLOAT3(target->GetShadowPos().x, pPlayer->GetPos().y, target->GetShadowPos().z));
		}
	}
	else
	{
		// ゲージ回復
		m_Gauge = m_Gauge >= GAUGE_MAX ? GAUGE_MAX : m_Gauge += static_cast<float>(5.0 * elpased_time);
	}
}

void Shadow::Draw(void)
{
	ShadowGaugeDraw();
}

void Shadow::ShadowGaugeDraw(void)
{
	Sprite_Draw(GaugeTextureID, 200.0f, 100.0f, 2.0f * GAUGE_MAX, 25.0f, { 1.0f,1.0f,1.0f,0.3f });

	for (int i = 0; i < m_Gauge; i++)
	{
		Sprite_Draw(GaugeTextureID, 100.0f + 2.0f * i, 100.0f, 2.0f, 25.0f, { 1.0f,1.0f,1.0f,1.0f });
	}
}

Shadow* GetShadow()
{
	return &shadow;
}
