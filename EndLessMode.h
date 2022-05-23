#pragma once
class EndLessMode
{

	int rockSpeed;
	int bulletSpeed;
	int objCount;
	float scoreMultiplier;
public:
	EndLessMode(int _rockSpeed= 35, int _bulletSpeed= 20, float _scoreMultiplier= 1,int _objCount = 5)
		: rockSpeed(_rockSpeed),bulletSpeed(_bulletSpeed),scoreMultiplier(_scoreMultiplier),
	objCount(_objCount) {}
    
	void decRockSpeed();
	void decbulletSpeed();
	void incMultiplier();
	void incObjCount();


	int getRockSpeed();
	int getbulletSpeed();
	int getScoreMultiplier();
	int getObjCount();

	void reset();
};

