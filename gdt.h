typedef struct {
    unsigned short limitLow;
    unsigned short baseLow;
    unsigned char baseMid;
    unsigned char access;
    unsigned char flagsLimitHigh;
    unsigned char baseHigh;
} __attribute__((packed)) GdtEntry;

typedef struct {
    unsigned short size;
    GdtEntry* gdtAddress;
} __attribute__((packed)) GdtDescriptor;

void initializeGdt();