// ==============================================================
// easing.h	
// イージング制御
// 
// 制作者:村上和樹		制作日付：2025/09/22	
// 更新者:村上和樹		更新日付：2025/09/22
// ==============================================================
#ifndef EASING_H
#define EASING_H

enum easing_functions
{
	EaseInSine,
	EaseOutSine,
	EaseInOutSine,
	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,
	EaseInQuart,
	EaseOutQuart,
	EaseInOutQuart,
	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,
	EaseInExpo,
	EaseOutExpo,
	EaseInOutExpo,
	EaseInCirc,
	EaseOutCirc,
	EaseInOutCirc,
	EaseInBack,
	EaseOutBack,
	EaseInOutBack,
	EaseInElastic,
	EaseOutElastic,
	EaseInOutElastic,
	EaseInBounce,
	EaseOutBounce,
	EaseInOutBounce
};

double easeInSine(double t);
double easeOutSine(double t);
double easeInOutSine(double t);
double easeInQuad(double t);
double easeOutQuad(double t);
double easeInOutQuad(double t);
double easeInCubic(double t);
double easeOutCubic(double t);
double easeInOutCubic(double t);
double easeInQuart(double t);
double easeOutQuart(double t);
double easeInOutQuart(double t);
double easeInQuint(double t);
double easeOutQuint(double t);
double easeInOutQuint(double t);
double easeInExpo(double t);
double easeOutExpo(double t);
double easeInOutExpo(double t);
double easeInCirc(double t);
double easeOutCirc(double t);
double easeInOutCirc(double t);
double easeInBack(double t);
double easeOutBack(double t);
double easeInOutBack(double t);
double easeInElastic(double t);
double easeOutElastic(double t);
double easeInOutElastic(double t);
double easeInBounce(double t);
double easeOutBounce(double t);
double easeInOutBounce(double t);

typedef double(*easingFunction)(double);

easingFunction getEasingFunction( easing_functions function );

#endif