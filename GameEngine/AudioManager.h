#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <Windows.h>
#pragma comment(lib, "Winmm.lib")
#include <string>

void openSound(std::wstring soundPath);
void playSound();
void pauseSound();
void resumeSound();
#endif // !AUDIOMANAGER_H
