#include "EndLessMode.h"
#include<algorithm>

void EndLessMode::decRockSpeed()
{
	rockSpeed--;
	rockSpeed = std::max(15, rockSpeed);
}

void EndLessMode::decbulletSpeed()
{
	bulletSpeed--;
	bulletSpeed = std::max(5, bulletSpeed);
}

void EndLessMode::incMultiplier()
{
	scoreMultiplier += 0.5;
	scoreMultiplier = std::min(scoreMultiplier, 13.0F);
}

void EndLessMode::incObjCount()
{
	objCount++;
	objCount = std::min(objCount, 24);
}

int EndLessMode::getRockSpeed()
{
	return rockSpeed;
}

int EndLessMode::getbulletSpeed()
{
	return bulletSpeed;
}

int EndLessMode::getScoreMultiplier()
{
	return (int)scoreMultiplier;
}

int EndLessMode::getObjCount()
{
	return objCount;
}

void EndLessMode::reset()
{
	rockSpeed = 35;
	bulletSpeed = 20;
	objCount = 5;
	scoreMultiplier = 1.0F;
}
