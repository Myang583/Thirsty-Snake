#include <string>
#include <windows.h>
using namespace std;
#pragma once

// CLASSES
class TextFrameManager {
private:
	HDC  hdc;
	RECT rect;
	char output[8];


public:
	TextFrameManager(HDC);
	void setOutput(char*);
	char* getOutputP();
	void setTextColor(COLORREF);
	void  setRect(RECT);
	RECT* getRectP();
	void drawText(UINT);

};


