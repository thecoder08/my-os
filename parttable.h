typedef struct {
    unsigned char status;
    unsigned char chsStart[3];
    unsigned char type;
    unsigned char chsEnd[3];
    unsigned int lba;
    unsigned int length;
} __attribute__((packed)) PartTableEntry;

void readPartitionTable(unsigned char controller, unsigned char driveNumber, PartTableEntry* entries);