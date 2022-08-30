#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "pic12f675.h"


FILE * file_read_binary(char *filename); 
static uint32_t file_get_size(FILE * file_p);
void open_bin() {
	FILE * file_p;
	uint32_t file_size;
	const uint16_t line_size = 300;
	char *line = malloc(line_size * sizeof(char));

	file_p = file_read_binary("./test_c.X.production.hex");
	file_size = file_get_size(file_p);	
	while(fgets(line, line_size, file_p)) printf(line);
	free(line);
}


int main(int argc, char *argv[]) {
	// open_bin();

	uint16_t hex_buffer[] = {0x3008, 0x1283, 0x00A1, 0x3002,
							 0x00A2, 0x0822, 0x0721, 0x00A0};
	
	CPU_t pic;
	for(int i = 0; i < 8; i++) {
		pic.program_mem[i] = hex_buffer[i];
	}
	cpu_init(&pic);
	
	for(int i = 0; i < 8; i++) {
		uint16_t opcode = fetch(&pic); 
		printf("%#x\n", opcode);
		decode(&pic, opcode);	
	}

	printf("%d\n", pic.data_mem[0x20]);
	printf("%d\n", pic.wreg);
		
	return 0;
}


FILE * file_read_binary(char *filename) {
	FILE * file_p;

	file_p = fopen(filename, "rb");
	if (file_p == NULL) {
		printf("Error opening file");
		exit(1);
	}
	return file_p;
}


static uint32_t file_get_size(FILE *file_p) {
	uint32_t file_size;
	fseek(file_p, 0, SEEK_END);
	file_size = ftell(file_p);
	rewind(file_p);
	return file_size;
}
