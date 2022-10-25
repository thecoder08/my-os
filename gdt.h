typedef struct {
    unsigned short limitLow;
    unsigned short baseLow;
    unsigned char baseMid;
    unsigned char access;
    unsigned char flagsLimitHigh;
    unsigned char baseHigh;
} GdtEntry;

typedef struct {
    unsigned short size;
    GdtEntry* gdtAddress;
} GdtDescriptor;

void initializeGdt();

#define low_16(address) (unsigned short)((address) & 0xFFFF)
#define high_16(address) (unsigned short)(((address) >> 16) & 0xFFFF)