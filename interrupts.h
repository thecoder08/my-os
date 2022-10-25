typedef struct {
    unsigned short lowOffset;
    unsigned short selector;
    unsigned char always0;
    unsigned char flags;
    unsigned short highOffset;
} IdtEntry;

typedef struct {
    unsigned short size;
    IdtEntry* idtAddress;
} IdtDescriptor;

void initializeIdt();

#define low_16(address) (unsigned short)((address) & 0xFFFF)
#define high_16(address) (unsigned short)(((address) >> 16) & 0xFFFF)