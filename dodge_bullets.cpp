#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>

#include <Windows.h>

using std::vector;
using std::map;


int width = 120;
int height = 40;
wchar_t* gameScreen = new wchar_t[width * height];
int objCount = 0;
vector<int> rockPos;
vector<int> bulletPos;
vector<int> shipFrontView;

void underTest();

// initialize the screens nd put the borders
void initScreen(wchar_t  screen[]) {
	for (int i = 0; i < width * height; i++) {
		screen[i] = ' ';
	}

	for (int i = 0; i < width; i++) {
		screen[i] = 0x2501;
		screen[width * (height - 1) + i] = 0x2501;
	}

	for (int i = 1; i < height - 1; i++) {
		screen[width * i + 1] = 0x2503;
		screen[width * i + width - 2] = 0x2503;
	}
	// corners----------------------------------
	// top left 
	screen[0] = ' ';
	screen[1] = 0x250F;

	// top right
	screen[width - 1] = ' ';
	screen[width - 2] = 0x2513;

	// bottom left
	screen[width * (height - 1)] = ' ';
	screen[width * (height - 1) + 1] = 0x2517;

	//bottom right
	screen[width * height - 2] = 0x251B;

	//last character null
	screen[width * height - 1] = '\0';
}    // 
// user Defined  delay using for loop
void uDelay(int x) {
	x *= 100000;
	for (int i = 0; i < x; i++);
}
//pause 'ENTER' play 'ENTER'
void pauseAndPlay() {
	if (GetAsyncKeyState(VK_RETURN)) {
		while (GetAsyncKeyState(VK_RETURN));
		while (true) {
			if (GetAsyncKeyState(VK_RETURN)) {
				while (GetAsyncKeyState(VK_RETURN));
				break;
			}
		}
	}
	return;
}

void initShipFrontView() {
	vector<int> temp{ 0,3,-4,7,1 - width,2 - width,-1 + width,-2 + width,-3 + width,4 + width,5 + width,6 + width };
	shipFrontView = vector(temp.begin(), temp.end());
}



// Drops  simulation
void rockDrops(int objects) {
	int temp = 0;
	if (objCount < 0) objCount = 0;
	if (objCount < objects) {
		objCount++;
		temp = rand() % (width - 4);
		temp += width + 2;
		gameScreen[temp] = 0x25A0;
		rockPos.push_back(temp);
	}
	for (auto x = rockPos.begin(); x != rockPos.end(); x++) {
		if (((*x) >= width * (height - 2) + 2) && (*x <= width * (height - 2) + width - 3)) {
			gameScreen[*x] = ' ';
			rockPos.erase(x);
			x = rockPos.begin();
			objCount--;
		}
		else {
			gameScreen[*x] = ' ';
			*x += width;
			gameScreen[*x] = 0x25A0;
		}
	}

}


// ----------------SPACE----------- SHIP-------------------------

//A utility function for spaceShip()
void printShip(int x) {

	gameScreen[x] = 0x256D;
	gameScreen[x + 3] = 0x256E;
	gameScreen[x + 1] = 0x2573;
	gameScreen[x + 2] = 0x2573;
	gameScreen[x - 4] = 0x254F;
	gameScreen[x + 7] = 0x254F;


	gameScreen[x - width + 1] = 0x2571;
	gameScreen[x - width + 2] = 0x2572;

	gameScreen[x + width] = 0x2567;
	gameScreen[x + width + 3] = 0x2567;
	gameScreen[x + width + 1] = 0x2550;
	gameScreen[x + width + 2] = 0x2550;
	gameScreen[x + width - 1] = 0x2560;
	gameScreen[x + width + 4] = 0x2563;
	gameScreen[x + width - 2] = 0x2573;
	gameScreen[x + width + 5] = 0x2573;
	gameScreen[x + width - 3] = 0x2557;
	gameScreen[x + width + 6] = 0x2554;
	gameScreen[x + width - 4] = 0x2560;
	gameScreen[x + width + 7] = 0x2563;

	for (int i = -1; i <= 4; i++) {
		gameScreen[x + 2 * width + i] = 0x2569;
	}
	gameScreen[x + 2 * width - 2] = 0x2550;
	gameScreen[x + 2 * width + 5] = 0x2550;
	gameScreen[x + 2 * width - 3] = 0x2560;
	gameScreen[x + 2 * width + 6] = 0x2563;
}

// A utility function for spaceShip()
void clearShip(int x) {

	gameScreen[x] = ' ';
	gameScreen[x + 3] = ' ';
	gameScreen[x + 1] = ' ';
	gameScreen[x + 2] = ' ';
	gameScreen[x - 4] = ' ';
	gameScreen[x + 7] = ' ';


	gameScreen[x - width + 1] = ' ';
	gameScreen[x - width + 2] = ' ';

	gameScreen[x + width] = ' ';
	gameScreen[x + width + 3] = ' ';
	gameScreen[x + width + 1] = ' ';
	gameScreen[x + width + 2] = ' ';
	gameScreen[x + width - 1] = ' ';
	gameScreen[x + width + 4] = ' ';
	gameScreen[x + width - 2] = ' ';
	gameScreen[x + width + 5] = ' ';
	gameScreen[x + width - 3] = ' ';
	gameScreen[x + width + 6] = ' ';
	gameScreen[x + width - 4] = ' ';
	gameScreen[x + width + 7] = ' ';

	for (int i = -1; i <= 4; i++) {
		gameScreen[x + 2 * width + i] = ' ';
	}
	gameScreen[x + 2 * width - 2] = ' ';
	gameScreen[x + 2 * width + 5] = ' ';
	gameScreen[x + 2 * width - 3] = ' ';
	gameScreen[x + 2 * width + 6] = ' ';
}

void shootBullets(const int& currentPosition) {

	if (GetAsyncKeyState(VK_LBUTTON)) {
		gameScreen[currentPosition - 4 - width] = 0x25CF;
		bulletPos.push_back(currentPosition - 4 - width);
	}
	if (GetAsyncKeyState(VK_RBUTTON)) {
		gameScreen[currentPosition + 7 - width] = 0x25CF;
		bulletPos.push_back(currentPosition + 7 - width);
	}
	for (auto x = bulletPos.begin(); x != bulletPos.end(); x++) {
		if ((*x) >= width + 2 && (*x) <= 2 * width - 3) {
			gameScreen[*x] = ' ';
			if (bulletPos.size() == 1) {
				bulletPos.clear();
				break;
			}
			bulletPos.erase(x);
			x = bulletPos.begin();
		}
		else
		{
			gameScreen[*x] = ' ';
			*x -= width;
			gameScreen[*x] = 0x25CF;
		}
	}


}

// tempararily done  
// Returns  whether the ship has been moved or not 
bool spaceShip(int shipPosition, int oldPosition) {
	int h;
	h = shipPosition / width;
	if (shipPosition + 7 <= h * width + width - 3 && shipPosition - 4 >= h * width + 2
		&& shipPosition - width >= width + 2 && shipPosition + 3 * width <= width * height - 3) {
		clearShip(oldPosition);
		printShip(shipPosition);
		return true;
	}
	return false;
}

void moveSpaceShip(int& currentPositionOfShip) {
	bool spaceShipFlag = true;
	if (GetAsyncKeyState((unsigned short)'A')) {
		spaceShipFlag = spaceShip(currentPositionOfShip - 1, currentPositionOfShip);
		if (spaceShipFlag) currentPositionOfShip--;
	}
	if (GetAsyncKeyState((unsigned short)'D')) {
		spaceShipFlag = spaceShip(currentPositionOfShip + 1, currentPositionOfShip);
		if (spaceShipFlag) currentPositionOfShip++;
	}
	if (GetAsyncKeyState((unsigned short)'W')) {
		spaceShipFlag = spaceShip(currentPositionOfShip - width, currentPositionOfShip);
		if (spaceShipFlag) currentPositionOfShip -= width;
	}
	if (GetAsyncKeyState((unsigned short)'S')) {
		spaceShipFlag = spaceShip(currentPositionOfShip + width, currentPositionOfShip);
		if (spaceShipFlag) currentPositionOfShip += width;
	}

	return;
}


// collision
// under construction
void bulletRockCollision() {

	for (auto x = bulletPos.begin(); x != bulletPos.end(); x++) {

		auto y = std::find(rockPos.begin(), rockPos.end(), *x);
		if (y != rockPos.end()) {
			bulletPos.erase(y);
			rockPos.erase(y);
			x = bulletPos.begin();
			objCount--;
			gameScreen[*x] = ' ';
		}

	}
}

bool shipRockCollision(int currentPositionOfShip) {

	map<int, int> cnt;
	for (auto& x : shipFrontView) {
		cnt[currentPositionOfShip + x] ++;
	}
	for (auto& x : rockPos) {
		cnt[x]++;
	}
	for (auto& x : cnt) {
		if (x.second > 1) return true;
	}
	return false;
}





//     ---------MENU  ----------------------SCREEN------------------

wchar_t* menuScreen = new wchar_t[width * height];

void initMenuScreen() {

	initScreen(menuScreen);
	char gameName[] = "CHICKEN INVADERS UNICODE VERSION!!";
	char playerName[] = "NAME  :";
	int reference = 8 * width + width / 5 + 18;
	int i = 0;
	while (gameName[i] != '\0') {
		menuScreen[reference + i] = gameName[i];
		i++;
	}
	i = 0;
	while (playerName[i] != '\0') {
		menuScreen[reference + 2 * width + i + 5] = playerName[i];
		i++;
	}
	menuScreen[width * height - 1] = '\0';
}


// A utility function for   userInput()
void pressCharacter(char ch, int& index, int reference) {
	if (index > 30) return;
	if (GetAsyncKeyState((unsigned short)ch)) {
		while (GetAsyncKeyState((unsigned short)ch));
		menuScreen[reference + index] = ch;
		index++;
	}
}
// index --> from where to start write (Ralative position )
void userInput(int& index) {
	// reference is  based on the reference from initMenuScreen fn
	// and the constant '13' in the second line is based on the length of the string "Name  :"
	// the constant should be changed if the length of the string is altered 
	int reference = 8 * width + width / 5 + 18;
	reference += 2 * width + 13;
	for (int i = 'A'; i <= 'Z'; i++) {
		pressCharacter((char)i, index, reference);
	}

	return;
}

bool runGameScreen(HANDLE& hGameScreen,
	DWORD& dwBytesWrittenGame,
	int& slowBullets,
	int& slowRocks,
	int& currentPositionOfShip
) {
	if (slowBullets == 25) {
		shootBullets(currentPositionOfShip);
		slowBullets = 0;
	}slowBullets++;
	if (slowRocks == 50) {
		rockDrops(5);
		slowRocks = 0;
	}slowRocks++;
	moveSpaceShip(currentPositionOfShip);
	pauseAndPlay();
	//	bulletRockCollision();
	WriteConsoleOutputCharacter(hGameScreen, gameScreen, width * height, { 0,0 }, &dwBytesWrittenGame);
	uDelay(45);
	if (shipRockCollision(currentPositionOfShip)) return false;
	return true;
}

bool runMenuScreen(HANDLE& hMenuScreen, DWORD& dwBytesWrittenMenu, int& menuCursor) {

	userInput(menuCursor);
	if (GetAsyncKeyState(VK_RETURN)) {
		while (GetAsyncKeyState(VK_RETURN));
		return false;
	}
	if (menuCursor == 30) {
		return false;
	}
	WriteConsoleOutputCharacter(hMenuScreen, menuScreen, width * height, { 0,0 }, &dwBytesWrittenMenu);
	return true;
}

void getExcess() {
	if (GetAsyncKeyState((unsigned int)'A')) {
		while (GetAsyncKeyState((unsigned int)'A')); return;
	}
	if (GetAsyncKeyState((unsigned int)'S')) {
		while (GetAsyncKeyState((unsigned int)'S')); return;
	}
	if (GetAsyncKeyState((unsigned int)'W')) {
		while (GetAsyncKeyState((unsigned int)'W')); return;
	}
	if (GetAsyncKeyState((unsigned int)'D')) {
		while (GetAsyncKeyState((unsigned int)'D')); return;
	}
}

// main function
int main() {
	HANDLE hMenuScreen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	HANDLE hGameScreen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	DWORD dwBytesWrittenMenu = 0;
	DWORD dwBytesWrittenGame = 0;

	initMenuScreen();
	initShipFrontView();
	// for menu  Screen
	int menuCursor;

	// for Game Screen
	int slowBullets = 0;
	int slowrocks = 0;
	int currentPostionOfShip;
	int screenNumber = 0;
	bool runGetExcessOnce = true;/* 0->initMenuScreen, 1->initGameScreen
						   2 -> runMenuScreen, 3 -> runGameScreen*/
	while (true) {
		switch (screenNumber) {
		case 0:
			SetConsoleActiveScreenBuffer(hMenuScreen);
			initMenuScreen();
			menuCursor = 0;
			screenNumber = 2; break;
		case 1:
			SetConsoleActiveScreenBuffer(hGameScreen);
			initScreen(gameScreen);
			rockPos.clear();
			bulletPos.clear();
			objCount = 0;
			currentPostionOfShip = (height - 4) * width + 10;
			printShip(currentPostionOfShip);
			screenNumber = 3; break;
		case 2:
			//After the ship collided with rock get the excess amount WASD pressed
			if (runGetExcessOnce) {
				getExcess();
				runGetExcessOnce = false;
			}
			if (!runMenuScreen(hMenuScreen, dwBytesWrittenMenu, menuCursor)) {
				screenNumber = 1;
				runGetExcessOnce = true;
			}
			break;
		case 3:

			if (!runGameScreen(hGameScreen, dwBytesWrittenGame, slowBullets, slowrocks, currentPostionOfShip)) {
				screenNumber = 0;
			}
			break;
		}

	}

	delete[] gameScreen;
	delete[] menuScreen;
	return 0;
}

// ----------------------just for fun

void underTest() {

	return;
}