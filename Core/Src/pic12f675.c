#include "pic12f675.h"
#include <stdio.h> 


void cpu_init(CPU_t *cpu) {
	cpu->pc = 0;
	cpu->wreg = 0;
	cpu->data_mem[STATUS_BANK0] = 0x18;	// STATUS 0b00011xxx
	//cpu->data_mem[5] = 0x00; // GPIO 0b--xxxxxx 
	// cpu->TMR0 = &cpu->data_mem[0x01];
	// cpu->PCL = &cpu->data_mem[0x02];
	// cpu->STATUS = &cpu->data_mem[0x03];
	// cpu->FSR = &cpu->data_mem[0x04];
	// cpu->GPIO = &cpu->data_mem[0x05];
	// cpu->PCLATH = &cpu->data_mem[0x0A];
}


uint16_t fetch(CPU_t *cpu) {
	uint16_t opcode = cpu->program_mem[cpu->pc];
	cpu->pc++;
	return opcode;
}


void decode(CPU_t *cpu, uint16_t opcode) {
	switch (opcode & 0x3000) {
		case 0x0000:
			switch (opcode & 0x0F00) {
				case 0x0000:
					movwf(cpu, opcode);
					break;
				case 0x0700:
					addwf(cpu, opcode);
					break;
				case 0x0800:
					movf(cpu, opcode);
					break;
			}	
			break;
		case 0x3000:
			switch (opcode & 0x0C00) {
				case 0x0000:
					movlw(cpu, opcode);
					break;
			}
			break;
		case 0x2000:
				break;
		case 0x1000:
			switch (opcode & 0x0C00) {
				case 0x0000:
					bcf(cpu, opcode);
					break;
			}
			break;		
	} 
}


void movlw(CPU_t *cpu, uint16_t opcode) {
	// 11 00xx kkkk kkkk
	cpu->wreg = opcode & 0xFF;
}


void bcf(CPU_t *cpu, uint16_t opcode) {
	// 01 00bb bfff ffff
	uint8_t f = opcode & 0x007F;
	uint8_t b = (opcode & 0x0380) >> 7;
	
	test_bank(cpu, f);
	
	cpu->data_mem[f] &= ~(1 << b);

	update_multbanks_registers(cpu, f);
}


void movwf(CPU_t *cpu, uint16_t opcode) {
	// 00 0000 1fff ffff
	uint8_t f = opcode & 0x007F;
	
	test_bank(cpu, f);
	
	cpu->data_mem[f] = cpu->wreg;
	
	update_multbanks_registers(cpu, f);
}

void movf(CPU_t *cpu, uint16_t opcode) {
	// 00 1000 dfff ffff
	uint8_t f = opcode & 0x007F;
	uint8_t d = (opcode & (1 << 7)) >> 7;
	
	test_bank(cpu, f);
	
	if (d == 0) cpu->wreg = cpu->data_mem[f];
	else cpu->data_mem[f] = cpu->data_mem[f];
	
	if (cpu->data_mem[f] == 0) {
		// Set STATUS zero flag (bit 2)
		cpu->data_mem[STATUS_BANK0] |= (1 << 2);
	}
	else {
		cpu->data_mem[STATUS_BANK0] &= ~(1 << 2);
	}
	update_multbanks_registers(cpu, STATUS_BANK0);
}


void addwf(CPU_t *cpu, uint16_t opcode) {
	// 00 0111 dfff ffff
	uint8_t f = opcode & 0x007F;
	uint8_t d = (opcode & (1 << 7)) >> 7;
	uint8_t sum2;
	test_bank(cpu, f);
	uint16_t num1 = cpu->wreg;
	uint16_t num2 = cpu->data_mem[f];
	uint16_t result = num1 + num2;
	if (d == 1) {
		cpu->data_mem[f] = result;
		update_multbanks_registers(cpu, f);
	}
	else cpu->wreg = result;

	// Set STATUS zero flag (bit 2)
	if ((result & 0xFF) == 0) cpu->data_mem[STATUS_BANK0] |= (1 << 2);
	else cpu->data_mem[STATUS_BANK0] &= ~(1 << 2);

	// Set STATUS digital carry (bit 1)
	sum2 = (num1 & 0x000F) + (num2 & 0x000F);
	if ((sum2 & 0x10) == 0x10) cpu->data_mem[STATUS_BANK0] |= (1 << 1);
	else cpu->data_mem[STATUS_BANK0] &= ~(1 << 0);

	// Set STATUS carry flag (bit 0)
	if ((result & 0x100) == 0x100) cpu->data_mem[STATUS_BANK0] |= (1 << 0);
	else cpu->data_mem[STATUS_BANK0] &= ~(1 << 0);
	
	update_multbanks_registers(cpu, STATUS_BANK0);
}


void update_multbanks_registers(CPU_t *cpu, uint8_t f) {
	if (f==PCL_BANK0 || f==STATUS_BANK0 || f==FSR_BANK0 || f==PCLATH_BANK0 || f==INTCON_BANK0) {
		cpu->data_mem[f + 0x80] = cpu->data_mem[f];
	}
	else if (f==PCL_BANK1 || f==STATUS_BANK1 || f==FSR_BANK1 || f==PCLATH_BANK1 || f==INTCON_BANK1) {
		cpu->data_mem[f - 0x80] = cpu->data_mem[f];
	}
}


void test_bank(CPU_t *cpu, uint8_t f) {
	// Get actual bank: 0=bank0, 1=bank1
	uint8_t actual_bank = (cpu->data_mem[STATUS_BANK0] & (1 << 5)) >> 5;
	if ((actual_bank==0 && f>=0x80) || (actual_bank==1 && f < 0x80))  {
		printf("Trying to access wrong bank");
		exit(1);
	}
}
