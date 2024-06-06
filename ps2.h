void write_ps2_1(unsigned char byte);
void write_ps2_2(unsigned char byte);
void init_ps2_1(void (*ps2_1_data_handler)(unsigned char byte));
void init_ps2_2(void (*ps2_2_data_handler)(unsigned char byte));