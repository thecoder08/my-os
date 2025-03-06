extern fn keyboardScan([*]const u8) void;
extern fn serialScan([*]const u8) void;
extern fn vgaPrint([*]const u8) void;
extern fn serialPrint([*]const u8) void;

var tty: i32 = 1;

export fn scan(buffer: [*]const u8) void {
    if (tty != 0) {
        keyboardScan(buffer);
    } else {
        serialScan(buffer);
    }
}

export fn print(string: [*]const u8) void {
    if (tty != 0) {
        vgaPrint(string);
    } else {
        serialPrint(string);
    }
}

export fn ctty(newTty: i32) void {
    tty = newTty;
}
