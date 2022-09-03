#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "pic12f675.h"

#define CHAR_HEX_TO_INT(x) ((x>='0' && x<='9') ? x - '0' : \
							(x>='A' && x<='F') ? x - 'A' + 10: \
							(x>='a' && x<='f') ? x - 'a' + 10: -1)

static uint32_t file_get_size(FILE * file_p);


void open_hex(char * filename) {
	FILE * file_p;
	uint32_t file_size;
	const uint16_t line_size_max = 300;
	uint8_t number_of_data_bytes, data_offset;
	uint16_t address;
	uint8_t record_type, checksum;
	uint16_t data[128];
	
	char *line = malloc(line_size_max * sizeof(char));

	// Open hex (binary) file
	file_p = fopen(filename, "rb");
	if (file_p == NULL) {
		printf("Error opening file");
		exit(1);
	}
	file_size = file_get_size(file_p);	
	while(fgets(line, line_size_max, file_p)) {
		printf(line);
		number_of_data_bytes = CHAR_HEX_TO_INT(line[1]) * (1 << 4) + CHAR_HEX_TO_INT(line[2]);
		address = CHAR_HEX_TO_INT(line[3]) * (1 << 12) + CHAR_HEX_TO_INT(line[4]) * (1 << 8) +
				  CHAR_HEX_TO_INT(line[5]) * (1 << 4) + CHAR_HEX_TO_INT(line[6]) * ( 1<< 0);
		address = address/2;
		record_type = CHAR_HEX_TO_INT(line[7]) * (1 << 4) + CHAR_HEX_TO_INT(line[8]);
		
		for (int i = 0; i < (number_of_data_bytes >> 1); ++i) {
			data_offset = i * 4;
			data[i] = CHAR_HEX_TO_INT(line[9 + data_offset]) * (1 << 4) + CHAR_HEX_TO_INT(line[10 + data_offset]) * (1 << 0) + 
					  CHAR_HEX_TO_INT(line[11 + data_offset]) * (1 << 12) + CHAR_HEX_TO_INT(line[12 + data_offset]) * (1 << 8);
		}

		checksum = CHAR_HEX_TO_INT(line[9 + (number_of_data_bytes<<1)]) * (1 << 4) + CHAR_HEX_TO_INT(line[10 + (number_of_data_bytes<<1)]);
		
		printf("Number of bytes: %#x\n", number_of_data_bytes);
		printf("Address: %#x\n", address);
		printf("Type: %#x\n", record_type);
		printf("Data: ");
		for (int i = 0; i < (number_of_data_bytes >> 1); ++i) {
			printf("%#x ", data[i]);
		}
		printf("\n");
		printf("Checksum: %#x\n", checksum);
		printf("\n");
	}
	free(line);

	fclose(file_p);
}


int main(int argc, char *argv[]) {
	open_hex("./test_c.X.production.hex");

	// uint16_t hex_buffer[] = {0x3008, 0x1283, 0x00A1, 0x3002,
							 // 0x00A2, 0x0822, 0x0721, 0x00A0};
	// 
	// CPU_t pic;
	// for(int i = 0; i < 8; i++) {
		// pic.program_mem[i] = hex_buffer[i];
	// }
	// cpu_init(&pic);
	// 
	// for(int i = 0; i < 8; i++) {
		// uint16_t opcode = fetch(&pic);
		// printf("%#x\n", opcode);
		// decode(&pic, opcode);
	// }
// 
	// printf("%d\n", pic.data_mem[0x20]);
	// printf("%d\n", pic.wreg);
		
	return 0;
}


static uint32_t file_get_size(FILE *file_p) {
	uint32_t file_size;
	fseek(file_p, 0, SEEK_END);
	file_size = ftell(file_p);
	rewind(file_p);
	return file_size;
}
