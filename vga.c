#include "io.h"
#include "vga.h"
#include "mem.h"
#include "graphics.h"

extern int fontFile; // this is an int because its pointer becomes an int*; i.e. an array.
extern int textMode;
extern Framebuffer image;
int cursorX;
int cursorY;

void setChar(char character, char color, int x, int y) {
	if (textMode) {
		char* cell = (char*) 0x000b8000 + ((y * 80 + x) * 2);
    	*cell = character;
    	*(cell + 1) = color;
	}
	else {
		drawBuffer(x*9, y*16, 9, 16, &fontFile + character*16*9);
	}
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

int cmpChar(char* position, char character) {
	for (int i = 0; i < 16; i++) {
		if (!memcmp(position + i*image.width*4, (char*)(&fontFile + character*16*9 + i*9), 9*4)) {
			return 0;
		}
	}
	return 1;
}

void setCursor(int x, int y) {
	if (textMode) {
		short pos = y * 80 + x;
		setVgaRegister(0x3d4, 0x0f, (char) (pos & 0xFF));
		setVgaRegister(0x3d4, 0x0e, (char) ((pos >> 8) & 0xFF));
	}
	else {
		/*for (int i = 0; i < 25; i++) {
			for (int j = 0; i < 80; j++) {
				if (cmpChar((char*)(image.data + (i*image.width*16) + (j*9)), ' ')) {
					setChar('#', 0x07, j, i);
				}
			}
		}*/
		if (cmpChar((char*)(image.data + (cursorY*image.width*16) + (cursorX*9)), '_')) { // if the old cursor position is still shown as a cursor
			setChar(' ', 0x07, cursorX, cursorY); // overwrite it with a space
		}
		cursorX = x;
		cursorY = y;
		if (cmpChar((char*)(image.data + (cursorY*image.width*16) + (cursorX*9)), ' ')) {
			setChar('_', 0x07, cursorX, cursorY);
		}
	}
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

void clearText(char character, char color) {
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
		if (textMode) {
			for (int row = 0; row < 24; row++) {
				char* thisRow = (char*) 0x000b8000 + 160*row;
				char* nextRow = (char*) 0x000b8000 + 160*(row+1);
				memcpy(nextRow, thisRow, 160);
			}
		}
		else {
			for (int row = 0; row < 24; row++) {
				char* thisRow = (char*) image.data + image.width*16*4*row;
				char* nextRow = (char*) image.data + image.width*16*4*(row+1);
				memcpy(nextRow, thisRow, image.width*16*4);
			}
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