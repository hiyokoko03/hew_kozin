// ==============================================================
// model.h	
// モデル制御
// 
// 制作者:Kazuki Murakami		制作日付：2025/12/03	
// 更新者:Kazuki Murakami		更新日付：2025/12/03
// ==============================================================
#ifndef MODEL_H
#define MODEL_H

#include <unordered_map>
#include <DirectXMath.h>
#include <string>
#include "direct3D.h"
#include "DirectXTex.h"
#include "shader3d.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")

struct MODEL
{
	const aiScene* AiScene = nullptr;

	ID3D11Buffer** VertexBuffer{};
	ID3D11Buffer** IndexBuffer{};

	std::unordered_map<std::string, ID3D11ShaderResourceView*> Texture{};
};


MODEL* ModelLoad(const char* FileName, float scale = 1.0f);
void ModelRelease(MODEL* model);

void ModelDraw(const MODEL* model,const DirectX::XMMATRIX& mtxWorld);

#endif
