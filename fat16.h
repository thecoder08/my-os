typedef struct {
    char pad[3];
    char oemId[8];
    unsigned short bytesPerSector;
    unsigned char sectorsPerCluster;
    unsigned short reservedSectors;
    unsigned char numFats;
    unsigned short numRootDirEntries;
    unsigned short numTotalSectors;
    unsigned char mediaDescriptorType;
    unsigned short sectorsPerFat;
    unsigned short sectorsPerTrack;
    unsigned short numHeads;
    unsigned int numHiddenSectors;
    unsigned int largeSectorCount;
    // extended BPB - FAT16-specific
    unsigned char driveNumber;
    unsigned char ntFlags;
    unsigned char signature;
    unsigned int serial;
    char label[11];
    char fsTypeString[8];
    char padEnd[450];
} __attribute__((packed)) Bpb;

typedef struct {
    char name[8];
    char extension[3];
    unsigned char attributes;
    unsigned char reserved;
    unsigned char createTime100thSecs;
    unsigned short createTime;
    unsigned short createDate;
    unsigned short acessedDate;
    unsigned short highCluster; // unused in fat12/fat16
    unsigned short modTime;
    unsigned short modDate;
    unsigned short cluster;
    unsigned int size;
} __attribute__((packed)) DirEntry;

Bpb readbpb(unsigned char controller, unsigned char driveNumber, unsigned int lba);
void listRootDir(unsigned char controller, unsigned char driveNumber, unsigned int lba, DirEntry* entries);
unsigned int clusterToSector(unsigned int cluster, Bpb bpb, unsigned int lba);
int readFile(unsigned char controller, unsigned char driveNumber, char* name, char* extension, unsigned int lba, void* buffer);