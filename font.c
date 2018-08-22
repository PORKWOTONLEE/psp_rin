const unsigned char font[]={
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0x38,0x44,0x82,0x82,0x82,0x44,0x38,0x00,	//��
	0x82,0x44,0x28,0x10,0x28,0x44,0x82,0x00,	//�~
	0xFE,0x82,0x82,0x82,0x82,0x82,0xFE,0x00,	//��
	0x10,0x28,0x28,0x44,0x44,0x82,0xFE,0x00,	//��
	0x10,0x38,0x7C,0x10,0x10,0x10,0x10,0x00,	//��
	0x00,0x08,0x0C,0xFE,0x0C,0x08,0x00,0x00,	//��
	0x10,0x10,0x10,0x10,0x7C,0x38,0x10,0x00,	//��

	0x00,0x20,0x60,0xFE,0x60,0x20,0x00,0x00,	//��
//	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0b01111110,                                 // 9 cursor for rewind progress bar
	0b01111110,
	0b01111110,
	0b01111110,
	0b01111110,
	0b01111110,
	0b01111110,
	0b11111111,
	//0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0b00000000,                                 // 10 main builder for rewind progress bar
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11111111,
//	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0b00000011,                                 // 11 left limiter for rewind progress bar
	0b00000011,
	0b00000011,
	0b00000011,
	0b00000011,
	0b00000011,
	0b00000011,
	0b00000011,
//	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0b11000000,                                 // 12 right limiter for rewind progress bar
	0b11000000,
	0b11000000,
	0b11000000,
	0b11000000,
	0b11000000,
	0b11000000,
	0b11000000,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x18,0x18,0x18,0x18,0x00,0x18,0x00,
	0x00,0x66,0x66,0x66,0x00,0x00,0x00,0x00,
	0x00,0x66,0xff,0x66,0x66,0xff,0x66,0x00,
	0x18,0x3e,0x60,0x3c,0x06,0x7c,0x18,0x00,
	0x00,0x66,0x6c,0x18,0x30,0x66,0x46,0x00,
	0x1c,0x36,0x1c,0x38,0x6f,0x66,0x3b,0x00,
	0x00,0x18,0x18,0x18,0x00,0x00,0x00,0x00,

	0x00,0x0e,0x1c,0x18,0x18,0x1c,0x0e,0x00,
	0x00,0x70,0x38,0x18,0x18,0x38,0x70,0x00,
	0x00,0x66,0x3c,0xff,0x3c,0x66,0x00,0x00,
//	0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x00,
	0x00,0x18,0x18,0x7e,0x7e,0x18,0x18,0x00,
	0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30,
//	0x00,0x00,0x00,0x7e,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x7e,0x7e,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,
	0x00,0x06,0x0c,0x18,0x30,0x60,0x40,0x00,

	0x00,0x3c,0x66,0x6e,0x76,0x66,0x3c,0x00,
	0x00,0x18,0x38,0x18,0x18,0x18,0x7e,0x00,
	0x00,0x3c,0x66,0x0c,0x18,0x30,0x7e,0x00,
	0x00,0x7e,0x0c,0x18,0x0c,0x66,0x3c,0x00,
	0x00,0x0c,0x1c,0x3c,0x6c,0x7e,0x0c,0x00,
	0x00,0x7e,0x60,0x7c,0x06,0x66,0x3c,0x00,
	0x00,0x3c,0x60,0x7c,0x66,0x66,0x3c,0x00,
	0x00,0x7e,0x06,0x0c,0x18,0x30,0x30,0x00,

	0x00,0x3c,0x66,0x3c,0x66,0x66,0x3c,0x00,
	0x00,0x3c,0x66,0x3e,0x06,0x0c,0x38,0x00,
	0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x00,
	0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x30,
	0x06,0x0c,0x18,0x30,0x18,0x0c,0x06,0x00,
	0x00,0x00,0x7e,0x00,0x00,0x7e,0x00,0x00,
	0x60,0x30,0x18,0x0c,0x18,0x30,0x60,0x00,
	0x00,0x3c,0x66,0x0c,0x18,0x00,0x18,0x00,

	0x00,0x3c,0x66,0x6e,0x6e,0x60,0x3e,0x00,
	0x00,0x18,0x3c,0x66,0x66,0x7e,0x66,0x00,
	0x00,0x7c,0x66,0x7c,0x66,0x66,0x7c,0x00,
	0x00,0x3c,0x66,0x60,0x60,0x66,0x3c,0x00,
	0x00,0x78,0x6c,0x66,0x66,0x6c,0x78,0x00,
	0x00,0x7e,0x60,0x7c,0x60,0x60,0x7e,0x00,
	0x00,0x7e,0x60,0x7c,0x60,0x60,0x60,0x00,
	0x00,0x3e,0x60,0x60,0x6e,0x66,0x3e,0x00,

	0x00,0x66,0x66,0x7e,0x66,0x66,0x66,0x00,
	0x00,0x7e,0x18,0x18,0x18,0x18,0x7e,0x00,
	0x00,0x06,0x06,0x06,0x06,0x66,0x3c,0x00,
	0x00,0x66,0x6c,0x78,0x78,0x6c,0x66,0x00,
	0x00,0x60,0x60,0x60,0x60,0x60,0x7e,0x00,
	0x00,0x63,0x77,0x7f,0x6b,0x63,0x63,0x00,
	0x00,0x66,0x76,0x7e,0x7e,0x6e,0x66,0x00,
	0x00,0x3c,0x66,0x66,0x66,0x66,0x3c,0x00,

	0x00,0x7c,0x66,0x66,0x7c,0x60,0x60,0x00,
	0x00,0x3c,0x66,0x66,0x66,0x6c,0x36,0x00,
	0x00,0x7c,0x66,0x66,0x7c,0x6c,0x66,0x00,
	0x00,0x3c,0x60,0x3c,0x06,0x06,0x3c,0x00,
	0x00,0x7e,0x18,0x18,0x18,0x18,0x18,0x00,
	0x00,0x66,0x66,0x66,0x66,0x66,0x7e,0x00,
	0x00,0x66,0x66,0x66,0x66,0x3c,0x18,0x00,
	0x00,0x63,0x63,0x6b,0x7f,0x77,0x63,0x00,

	0x00,0x66,0x66,0x3c,0x3c,0x66,0x66,0x00,
	0x00,0x66,0x66,0x3c,0x18,0x18,0x18,0x00,
	0x00,0x7e,0x0c,0x18,0x30,0x60,0x7e,0x00,
	0x00,0x1e,0x18,0x18,0x18,0x18,0x1e,0x00,
	0x00,0x40,0x60,0x30,0x18,0x0c,0x06,0x00,
	0x00,0x78,0x18,0x18,0x18,0x18,0x78,0x00,
	0x00,0x08,0x1c,0x36,0x63,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,

	0x00,0x18,0x3c,0x7e,0x7e,0x3c,0x18,0x00,
	0x00,0x00,0x3c,0x06,0x3e,0x66,0x3e,0x00,
	0x00,0x60,0x60,0x7c,0x66,0x66,0x7c,0x00,
	0x00,0x00,0x3c,0x60,0x60,0x60,0x3c,0x00,
	0x00,0x06,0x06,0x3e,0x66,0x66,0x3e,0x00,
	0x00,0x00,0x3c,0x66,0x7e,0x60,0x3c,0x00,
	0x00,0x0e,0x18,0x3e,0x18,0x18,0x18,0x00,
	0x00,0x00,0x3e,0x66,0x66,0x3e,0x06,0x7c,

	0x00,0x60,0x60,0x7c,0x66,0x66,0x66,0x00,
	0x00,0x18,0x00,0x38,0x18,0x18,0x3c,0x00,
	0x00,0x06,0x00,0x06,0x06,0x06,0x06,0x3c,
	0x00,0x60,0x60,0x6c,0x78,0x6c,0x66,0x00,
	0x00,0x38,0x18,0x18,0x18,0x18,0x3c,0x00,
	0x00,0x00,0x66,0x7f,0x7f,0x6b,0x63,0x00,
	0x00,0x00,0x7c,0x66,0x66,0x66,0x66,0x00,
	0x00,0x00,0x3c,0x66,0x66,0x66,0x3c,0x00,

	0x00,0x00,0x7c,0x66,0x66,0x7c,0x60,0x60,
	0x00,0x00,0x3e,0x66,0x66,0x3e,0x06,0x06,
	0x00,0x00,0x7c,0x66,0x60,0x60,0x60,0x00,
	0x00,0x00,0x3e,0x60,0x3c,0x06,0x7c,0x00,
	0x00,0x18,0x7e,0x18,0x18,0x18,0x0e,0x00,
	0x00,0x00,0x66,0x66,0x66,0x66,0x3e,0x00,
	0x00,0x00,0x66,0x66,0x66,0x3c,0x18,0x00,
	0x00,0x00,0x63,0x6b,0x7f,0x3e,0x36,0x00,

	0x00,0x00,0x66,0x3c,0x18,0x3c,0x66,0x00,
	0x00,0x00,0x66,0x66,0x66,0x3e,0x0c,0x78,
	0x00,0x00,0x7e,0x0c,0x18,0x30,0x7e,0x00,
	0x00,0x18,0x3c,0x7e,0x7e,0x18,0x3c,0x00,
	0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
	0x00,0x7e,0x78,0x7c,0x6e,0x66,0x06,0x00,
	0x08,0x18,0x38,0x78,0x38,0x18,0x08,0x00,
	0x10,0x18,0x1c,0x1e,0x1c,0x18,0x10,0x00,

	0x00,0x36,0x7f,0x7f,0x3e,0x1c,0x08,0x00,
	0x18,0x18,0x18,0x1f,0x1f,0x18,0x18,0x18,
	0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
	0x18,0x18,0x18,0xf8,0xf8,0x00,0x00,0x00,
	0x18,0x18,0x18,0xf8,0xf8,0x18,0x18,0x18,
	0x00,0x00,0x00,0xf8,0xf8,0x18,0x18,0x18,
	0x03,0x07,0x0e,0x1c,0x38,0x70,0xe0,0xc0,
	0xc0,0xe0,0x70,0x38,0x1c,0x0e,0x07,0x03,

	0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff,
	0x00,0x00,0x00,0x00,0x0f,0x0f,0x0f,0x0f,
	0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff,
	0x0f,0x0f,0x0f,0x0f,0x00,0x00,0x00,0x00,
	0xf0,0xf0,0xf0,0xf0,0x00,0x00,0x00,0x00,
	0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
	0x00,0x00,0x00,0x00,0xf0,0xf0,0xf0,0xf0,

	0x00,0x1c,0x1c,0x77,0x77,0x08,0x1c,0x00,
	0x00,0x00,0x00,0x1f,0x1f,0x18,0x18,0x18,
	0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,
	0x18,0x18,0x18,0xff,0xff,0x18,0x18,0x18,
	0x00,0x00,0x3c,0x7e,0x7e,0x7e,0x3c,0x00,
	0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
	0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
	0x00,0x00,0x00,0xff,0xff,0x18,0x18,0x18,

	0x18,0x18,0x18,0xff,0xff,0x00,0x00,0x00,
	0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
	0x18,0x18,0x18,0x1f,0x1f,0x00,0x00,0x00,
	0x78,0x60,0x78,0x60,0x7e,0x18,0x1e,0x00,
	0x00,0x18,0x3c,0x7e,0x18,0x18,0x18,0x00,
	0x00,0x18,0x18,0x18,0x7e,0x3c,0x18,0x00,
	0x00,0x18,0x30,0x7e,0x30,0x18,0x00,0x00,
	0x00,0x18,0x0c,0x7e,0x0c,0x18,0x00,0x00,

	//7b
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,

	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
	0xCC,0xCC,0x33,0x33,0xCC,0xCC,0x33,0x33,
};


