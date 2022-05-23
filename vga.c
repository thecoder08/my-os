#include "io.h"
#include "mem.h"

void setChar(char character, char color, int x, int y) {
    int offset = 0x000b8000 + ((y * 80 + x) * 2);
    char* cell = (char*) offset;
    *cell = character;
    *(cell + 1) = color;
}

void setCursor(int x, int y) {
	short pos = y * 80 + x;
	out(0x3D4, 0x0F);
	out(0x3D5, (char) (pos & 0xFF));
	out(0x3D4, 0x0E);
	out(0x3D5, (char) ((pos >> 8) & 0xFF));
}

void enableCursor(char cursor_start, char cursor_end) {
	out(0x3D4, 0x0A);
	out(0x3D5, (in(0x3D5) & 0xC0) | cursor_start);
 
	out(0x3D4, 0x0B);
	out(0x3D5, (in(0x3D5) & 0xE0) | cursor_end);
}

void disableCursor() {
	out(0x3D4, 0x0A);
	out(0x3D5, 0x20);
}

void clear(char character, char color) {
  for (int i = 0; i < 25; i++){
	  for (int j = 0; j < 80; j++){
		  setChar(character, color, j, i);
	  }
  }
}

int currentX = 0;
int currentY = 0;
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
		currentY = 0;
	}
	setCursor(currentX, currentY);
}

void vgaPrint(char* string) {
	for (int i = 0; string[i] != 0; i++) {
		writeChar(string[i]);
	}
}