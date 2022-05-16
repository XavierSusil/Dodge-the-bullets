#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <fstream>
#include <thread>

#include <Windows.h>

#define test 0

using std::vector;
using std::map;


int width = 120;
int height = 40;
wchar_t* gameScreen = new wchar_t[width * height];
int objCount = 0;
static vector<int> rockPos;
static vector<int> bulletPos;
static vector<int> shipFrontView;

//scoring 
int currentScore = -1;
int maximumScore = -1;
std::string currentPlayer;

map<std::string, int> playerScores;

// reads  the  values  stored in FILE into  playerScores 
void  readPlayerScores() {

	std::ifstream fin("ScoreBoard.txt", std::ios_base::in);
	if (fin.is_open()) {
		std::string s;
		int n = 0; 
		while (fin >> s) {
			fin >> n;
			playerScores[s] = n;
		}
	}
	fin.close();
}

//  after  finishing the game  write  all the players and their scores from playerScores to FILE
void  writePlayerScores() {
	std::ofstream fout("ScoreBoard.txt", std::ios_base::out);

	for (auto& [x, y] : playerScores) {
		fout << x << " " << y << "\n";
	}
}

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
			currentScore++; // score increased by one as that rock has been dodged
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
		while (GetAsyncKeyState(VK_LBUTTON));
		int reference = currentPosition  - 2 * width + 1;
		if ( reference <= 2 * width - 3) return;
		gameScreen[reference] = 0x25CF;
		bulletPos.push_back(reference);
	}
	if (GetAsyncKeyState(VK_RBUTTON)) {
		while (GetAsyncKeyState(VK_RBUTTON));
		int reference = currentPosition - 2 * width + 2;
		if (reference <= 2 * width - 3) return;
		gameScreen[reference] = 0x25CF;
		bulletPos.push_back(reference);
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
 
// Returns  whether the ship has been moved or not
// also  moves  the ship  from old position to new position if possible
// used in moveSpaceShip
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
void bulletRockCollision() {

	for (int i = 0; i < bulletPos.size(); i++) {
		for (int j = 0; j < rockPos.size(); j++) {
			if (bulletPos[i] == rockPos[j]) {
				objCount--;
				gameScreen[bulletPos[i]] = ' ';
				bulletPos.erase(bulletPos.begin() + i);
				rockPos.erase(rockPos.begin() + j);
				currentScore += 5;
				return;
			}
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

void printInteger(int startingIndex, wchar_t screen[],int num) {

	int temp = num;
	int length = 0;
	if (!temp) screen[startingIndex + 1] = '0';
	int i = 1;
	temp = 0;
	while (num) {
		temp *= 10;
		temp += num % 10;
		num /= 10;
		length++;
	}
	while (temp) {
		int rem = temp % 10;
		temp /= 10;
		screen[startingIndex + i] = char(rem + 48);
		i++;
	}
	while (i <= length) {
		screen[startingIndex + i] = char(48);
		i++;
	}
}
void initMenuScreen() {

	initScreen(menuScreen);
	char gameName[] = "DODGE THE BULLETS UNICODE VERSION!!";
	char playerName[] =       "NAME        :";
	char scoreDisplay[] =     "SCORE       :";
	char highScoreDisplay[] = "HIGH SCORE  :";
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

	i = 0;
	if (currentScore != -1) {
		while (scoreDisplay[i] != '\0') {
			menuScreen[reference + 4 * width + i + 5] = scoreDisplay[i];
			i++;
		}
		printInteger(reference + 4 * width + i + 6, menuScreen, currentScore);
		i = 0;
		while (highScoreDisplay[i] != '\0') {
			menuScreen[reference + 6 * width + i + 5] = highScoreDisplay[i];
			i++;
		}
		printInteger(reference + 6 * width + i + 6, menuScreen, maximumScore);
	}
	menuScreen[width * height - 1] = '\0';
}


// A utility function for   userInput()
void pressCharacter(char ch, int& index, int reference) {
	if (index > 30) return;
	if (GetAsyncKeyState((unsigned short)ch)) {
		while (GetAsyncKeyState((unsigned short)ch));
		menuScreen[reference + index] = ch;
		currentPlayer += ch;
		index++;
	}
}
// index --> from where to start write (Ralative position )
void userInput(int& index) {
	// reference is  based on the reference from initMenuScreen fn
	// and the constant '19' in the second line is based on the length of the string "Name       :"
	// the constant should be changed if the length of the string is altered 
	int reference = 8 * width + width / 5 + 18;
	reference += 2 * width + 19;
	for (int i = 'A'; i <= 'Z'; i++) {
		pressCharacter((char)i, index, reference);
	}
	// detecting any back spaces pressed or not
	if (GetAsyncKeyState(VK_BACK)) {
		while (GetAsyncKeyState(VK_BACK));
		if (currentPlayer.size()) currentPlayer.erase(currentPlayer.size() - 1);
		if (index > 0) index--;
		menuScreen[reference + index] = ' ';
	}
	return;
}


// mention scoring in the bottom right corner of the game screen

void printScore() {
	char score[] = "SCORE : ";
	int i = 0;
	while (score[i] != '\0') {
		gameScreen[width * height - 20 + i] = score[i];
		i++;
	}

	int j = i;
	while (width * height - 20 + i < width * height - 1) {
		gameScreen[width * height - 20 + i] = ' '; i++;
	}
	
	printInteger(width * height - 20 + j, gameScreen, currentScore);
	
}
bool runGameScreen(HANDLE& hGameScreen,
	DWORD& dwBytesWrittenGame,
	int& slowBullets,
	int& slowRocks,
	int& currentPositionOfShip
) {
	if (slowBullets == 12) {
		shootBullets(currentPositionOfShip);
		slowBullets = 0;
	}slowBullets++;
	if (slowRocks == 30) {
		rockDrops(12);
		slowRocks = 0;
	}slowRocks++;
	moveSpaceShip(currentPositionOfShip);
	pauseAndPlay();
	bulletRockCollision();
	printScore();
	WriteConsoleOutputCharacter(hGameScreen, gameScreen, width * height, { 0,0 }, &dwBytesWrittenGame);
	uDelay(45);
	if (shipRockCollision(currentPositionOfShip)) return false;
	return true;
}

bool runMenuScreen(HANDLE& hMenuScreen, DWORD& dwBytesWrittenMenu, int& menuCursor) {

	userInput(menuCursor);
	if (GetAsyncKeyState(VK_RETURN)) {
		while (GetAsyncKeyState(VK_RETURN));
		//if  player name is  not  entered  do not leave the menu screen
		if (!currentPlayer.size()) return true;
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


static bool escPressed = false;

//this  function runs parallely   used for  finishing  game loop
void checkEsc() {
	while (!GetAsyncKeyState(VK_ESCAPE));
	while (GetAsyncKeyState(VK_ESCAPE));
	escPressed = true;
}
// main function
int main() {

#if test
	underTest();
	return 0;
#endif // test

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

	readPlayerScores();

	std::thread checkEscKeyPress(checkEsc);
	
	                      
	/* 0->initMenuScreen, 1->initGameScreen
     2 -> runMenuScreen, 3 -> runGameScreen*/
	while (!escPressed) {
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
			getExcess();
			currentPlayer = "";
			while (!escPressed and runMenuScreen(hMenuScreen, dwBytesWrittenMenu, menuCursor));
			screenNumber = 1;
				
			break;
		case 3:
			currentScore = 0;
			// score will be mentioned  in the bottom right corner

			while (!escPressed and runGameScreen(hGameScreen, dwBytesWrittenGame, slowBullets, slowrocks, currentPostionOfShip));
			screenNumber = 0;
			maximumScore = (maximumScore > currentScore) ? maximumScore : currentScore;
			
			if (playerScores.find("maximumScore") != playerScores.end()) {
				maximumScore = (maximumScore > playerScores["maximumScore"]) ? maximumScore : playerScores["maximumScore"];
				playerScores["maximumScore"] = maximumScore;
			}
			else {
				playerScores["maximumScore"] = maximumScore;
			}
			if (playerScores.find(currentPlayer) != playerScores.end()) {
				playerScores[currentPlayer] = (playerScores[currentPlayer] > currentScore) ? playerScores[currentPlayer] : currentScore;
			}
			else {
				playerScores[currentPlayer] = currentScore;
			}
			break;
		}

	}

	checkEscKeyPress.join();

	writePlayerScores();

	delete[] gameScreen;
	delete[] menuScreen;
	return 0;
}

void underTest() {

	return;
}