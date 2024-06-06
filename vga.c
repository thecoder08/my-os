#include "io.h"
#include "vga.h"
#include "mem.h"

void setChar(char character, char color, int x, int y) {
    char* cell = (char*) 0x000b8000 + ((y * 80 + x) * 2);
    *cell = character;
    *(cell + 1) = color;
}

void setVgaRegister3c0(char index, char data) {
	in(0x3da);
	out(0x3c0, index);
	out(0x3c0, data);
}
void setVgaRegister(unsigned short port, char index, char data) {
	out(port, index);
	out(port + 1, data);
}
char getVgaRegister3c0(char index) {
	in(0x3da);
	out(0x3c0, index);
	return in(0x3c1);
}
char getVgaRegister(unsigned short port, char index) {
	out(port, index);
	return in(port + 1);
}

void setCursor(int x, int y) {
	short pos = y * 80 + x;
	setVgaRegister(0x3d4, 0x0f, (char) (pos & 0xFF));
	setVgaRegister(0x3d4, 0x0e, (char) ((pos >> 8) & 0xFF));
}

void enableCursor(char cursor_start, char cursor_end) {
	setVgaRegister(0x3d4, 0x0a, (getVgaRegister(0x3d4, 0x0a) & 0xc0) | cursor_start);
	setVgaRegister(0x3d4, 0x0b, (getVgaRegister(0x3d4, 0x0b) & 0xe0) | cursor_end);
}

void disableCursor() {
	setVgaRegister(0x3d4, 0x0a, 0x20);
}

int currentX = 0;
int currentY = 0;

void clear(char character, char color) {
  for (int i = 0; i < 25; i++){
	  for (int j = 0; j < 80; j++){
		  setChar(character, color, j, i);
	  }
  }
  currentX = 0;
  currentY = 0;
  setCursor(0, 0);
}

void writeChar(char character) {
	if (character == '\r') {
		currentX = 0;
	}
	else if (character == '\n') {
		currentY++;
	}
	else if (character == '\b') {
		currentX--;
	}
	else {
		setChar(character, 0x07, currentX, currentY);
		currentX++;
	}
	if (currentX > 79) {
		currentX = 0;
		currentY++;
	}
	if (currentY > 24) {
		for (int row = 0; row < 24; row++) {
			char* thisRow = (char*) 0x000b8000 + 160*row;
			char* nextRow = (char*) 0x000b8000 + 160*(row+1);
			memcpy(nextRow, thisRow, 160);
		}
		for (int x = 0; x < 80; x++) {
			setChar(' ', 0x07, x, 24);
		}
		currentY = 24;
	}
	setCursor(currentX, currentY);
}

void vgaPrint(char* string) {
	for (int i = 0; string[i] != 0; i++) {
		writeChar(string[i]);
	}
}

// mode 1 is 320x200 256 color, mode 0 is 80x25 16 color text
void setVideoMode(int mode) {
	if (mode) {
		setVgaRegister3c0(0x10, 0x41);
		setVgaRegister3c0(0x11, 0x00);
		setVgaRegister3c0(0x12, 0x0F);
		setVgaRegister3c0(0x13, 0x00);
		setVgaRegister3c0(0x14, 0x00);
		out(0x3c2, 0x63);
		setVgaRegister(0x3c4, 0x01, 0x01);
		setVgaRegister(0x3c4, 0x03, 0x00);
		setVgaRegister(0x3c4, 0x04, 0x0e);
		setVgaRegister(0x3ce, 0x05, 0x40);
		setVgaRegister(0x3ce, 0x06, 0x05);
		
	}
	else {

	}
}