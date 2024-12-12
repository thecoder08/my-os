global fontFile
global cursorImage
global fingerImage
section .data
fontFile:
incbin "font.data"
cursorImage:
incbin "cursor.data"
fingerImage:
incbin "finger.data"