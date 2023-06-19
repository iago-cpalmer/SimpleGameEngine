#include "AudioManager.h"
#include <iostream>

void openSound(std::wstring soundPath) {
	std::wstring l = L"open \"res/sounds/" + soundPath + L"\" type mpegvideo alias mp3";
	mciSendString(const_cast<LPWSTR>(l.c_str()), NULL, 0, NULL);
}

void playSound() {
	mciSendString(L"play mp3 repeat", NULL, 0, NULL);
}

void resumeSound() {
	mciSendString(L"resume mp3", NULL, 0, NULL);
}
void pauseSound() {
	mciSendString(L"pause mp3 wait", NULL, 0, NULL);
}
