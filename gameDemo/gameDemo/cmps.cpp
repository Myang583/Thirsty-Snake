#include <string>
#include <iostream>
#include "cmps.h"

using namespace std;

TextFrameManager::TextFrameManager(HDC hdc){
	this->hdc = hdc;
}
char* TextFrameManager::getOutputP() {
	return this->output;
}

void TextFrameManager::setOutput(char* output) {
	strcpy_s(this->output, output);
}

void TextFrameManager::setTextColor(COLORREF crf) {
	SetTextColor(this->hdc, crf);
}
void TextFrameManager::setRect(RECT rect) {
	this->rect = rect;
}

RECT* TextFrameManager::getRectP() {

	return &(this->rect);
}

void TextFrameManager::drawText(UINT location) {
	DrawTextA(this->hdc, getOutputP(), -1, getRectP(), location);
}


