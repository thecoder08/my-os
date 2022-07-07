void lba_to_chs(int lba, int *head, int *track, int *sector);
void send_command(unsigned char command, unsigned char* params, unsigned char* data, int inorout, unsigned char* results);
void read_blocks(int blocks, int drive, int lba, char* buffer);
void write_blocks(int blocks, int drive, int lba, char* buffer);