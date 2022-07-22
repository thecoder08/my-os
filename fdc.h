void lba_to_chs(int lba, int *head, int *track, int *sector);
void send_command(char command, char* params, char* data, int inorout, char* results);
void read_blocks(int blocks, int drive, int lba, char* buffer);
void write_blocks(int blocks, int drive, int lba, char* buffer);
#define SECTORS_PER_TRACK 18