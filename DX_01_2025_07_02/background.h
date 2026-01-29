// ==============================================================
// background.h	
// Direct3D設定全般
// 
// 制作者:石沢山		        制作日付：2025/10/29	
// 更新者:Kazuki Murakami		更新日付：2025/11/05
// ==============================================================
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <DirectXMath.h>

#include "object.h"

// ===============================================
// マクロ定義
// ===============================================
static constexpr int BACKGROUND_MAX = 1;

// ===============================================
// クラス宣言
// ===============================================
class Background : public GameObject {
private:
    // ===========================================
    // メンバ変数
    // ===========================================
    int m_TextureID = -1;

public:
    // ===========================================
    // プロトタイプ宣言
    // ===========================================
    void Initialize(void)override;
    void Finalize(void)override;
    void Update(double elpased_time)override;
    void Draw(void)override;
    
    Background* GetBackground(void);
};



#endif//BACKGROUND_H