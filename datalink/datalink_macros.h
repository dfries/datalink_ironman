/*
  Macros for sending data to the datalink watch.
*/

#define START1 70
#define START2 255
#define SPACE 15

#define WRITE_BYTE1(sb, byte) { \
	int pos = START1; \
	register int mask = 0x1; \
	if (sb) \
		vga_drawscanline(pos, white); \
	else \
		vga_drawscanline(pos, black); \
	pos += SPACE; \
	while (mask < 0xff) { \
		if (mask&byte) \
			vga_drawscanline(pos, black); \
		else \
			vga_drawscanline(pos, white); \
		pos += SPACE; \
		mask = mask << 1; \
	} \
}

#define WRITE_BYTE2(sb, byte) { \
	int pos = START2; \
	register int mask = 1; \
	if (sb) \
		vga_drawscanline(pos, white); \
	else \
		vga_drawscanline(pos, black); \
	pos += SPACE; \
	while (mask < 0xff) { \
		if (mask&byte) \
			vga_drawscanline(pos, black); \
		else \
			vga_drawscanline(pos, white); \
		pos += SPACE; \
		mask = mask << 1; \
	} \
}

#define END_PACKET { \
	printf(" Sync "); \
	vga_waitretrace(); \
	WRITE_BYTE1(0, 0xff) \
	WRITE_BYTE2(0, 0xff) \
	{int i; for (i = 0; i < (10+((type==MODEL_IRONMAN)?2:0)); i++) \
	{	printf(" Sync "); \
		vga_waitretrace();}} \
}

#define SYNC { \
	register int i; \
	for (i = 0; i < 200; i++) { \
		vga_waitretrace(); \
		WRITE_BYTE1(1, 0x55) \
		WRITE_BYTE2(1, 0x55) \
	} \
	END_PACKET \
}

#define DATASTART { \
	register int i; \
	for (i = 0; i < 50; i++) { \
		vga_waitretrace(); \
		WRITE_BYTE1(1, 0xAA) \
	} \
	END_PACKET \
}