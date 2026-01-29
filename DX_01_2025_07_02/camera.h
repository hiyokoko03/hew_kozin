// ==============================================================
// camera.h	
// カメラ制御
// 
// 制作者:村上和樹		制作日付：2025/10/22	
// 更新者:村上和樹		更新日付：2025/10/28
// ==============================================================
#ifndef CAMERA_H
#define CAMERA_H

class Camera{
public:
	virtual ~Camera() = default;

	virtual void Initialize() {}
	virtual void Finalize() {}
	virtual void Update(double elapsed_time) {}
	virtual void Draw() {}
	virtual void SetMatrix() const = 0;
};

#endif //CAMERA_H
