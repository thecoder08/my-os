typedef struct {
    unsigned short lowOffset;
    unsigned short selector;
    unsigned char always0;
    unsigned char flags;
    unsigned short highOffset;
} __attribute__((packed)) IdtEntry;

typedef struct {
    unsigned short size;
    IdtEntry* idtAddress;
} __attribute__((packed)) IdtDescriptor;

struct interrupt_frame;

void initializeIdt();
void registerIsr(unsigned char entry, void (*isrHandler) (struct interrupt_frame* frame));
void addIrqHandler(unsigned char irq, void (*irqHandler) (struct interrupt_frame* frame));
void sendEOI(unsigned char irq);

#define low_16(address) (unsigned short)((address) & 0xFFFF)
#define high_16(address) (unsigned short)(((address) >> 16) & 0xFFFF)