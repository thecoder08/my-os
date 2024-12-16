global fontFile
global cursorImage
global fingerImage
global smallFont
section .data
fontFile:
incbin "font.data"
cursorImage:
incbin "cursor.data"
fingerImage:
incbin "finger.data"
smallFont:
incbin "smallfont.data"