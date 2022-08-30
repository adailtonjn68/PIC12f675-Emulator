#ifndef PIC12F675_H_
#define PIC12F675_H_

#include <stdint.h>


enum Registers {
	TMR0 = 0x01,
	PCL_BANK0 = 0x02,
	STATUS_BANK0,
	FSR_BANK0,
	GPIO,
	PCLATH_BANK0 = 0x0A,
	INTCON_BANK0,
	PIR1,
	TMR1L = 0xE,
	TMR1H,
	T1CON,
	CMCON = 0x19,
	ADRESH = 0x1E,
	ADCON0 = 0x1F,
	GPR_BANK0,
	OPTION_REG = 0x81,
	PCL_BANK1,
	STATUS_BANK1,
	FSR_BANK1,
	TRISIO,
	PCLATH_BANK1 = 0x8A,
	INTCON_BANK1,
	PIE1,
	PCON = 0x8E,
	OSCCAL = 0x90,
	WPU = 0x95,
	IOC,
	VRCON = 0x99,
	EEDATA,
	EEADR,
	EECON1,
	EECON2,
	ADRESL,
	ANSEL,
	GPR_BANK1
};


typedef struct {
	int pc:14;
	uint8_t wreg;
	//uint16_t program_mem[8192];
	// uint16_t *program_mem;
	uint16_t stack[8];
	uint16_t program_mem[8192];
	uint8_t data_mem[256];

	uint8_t *TMR0;
	uint8_t *PCL;
	uint8_t *STATUS;
	uint8_t *FSR;
	uint8_t *GPIO;
	uint8_t *PCLATH;
	uint8_t *INTCON;
	uint8_t *PIR1;
	uint8_t *TMR1L;
	uint8_t *TMR1H;
	uint8_t *T1CON;
	uint8_t *CMCON;
	uint8_t *ADRESH;
	uint8_t *ADCON0;
	uint8_t *GPReg;
	uint8_t *OPTION_REG;
	uint8_t *TRISIO;
	uint8_t *PIE1;
	uint8_t *PCON;
	uint8_t *OSCCAL;
	uint8_t *WPU;
	uint8_t *IOC;
	uint8_t *VRCON;
	uint8_t *EEDATA;
	uint8_t *EEADR;
	uint8_t *EECON1;
	uint8_t *EECON2;
	uint8_t *ADRESL;
	uint8_t *ANSEL;	
} CPU_t;


void cpu_init(CPU_t *cpu);

uint16_t fetch(CPU_t *cpu);

void decode(CPU_t *cpu, uint16_t opcode);

/*
 * BYTE-ORIENTED FILE REGISTER OPERATIONS
 */
void addwf(CPU_t *cpu, uint16_t opcode);
void andwf(CPU_t *cpu, uint16_t opcode);
void clrf(CPU_t *cpu, uint16_t opcode);
void clrw(CPU_t *cpu, uint16_t opcode);
void comf(CPU_t *cpu, uint16_t opcode);
void decf();
void incf();
void incfsz();
void iorwf();
void movf(CPU_t *cpu, uint16_t opcode);
void movwf(CPU_t *cpu, uint16_t opcode);
void nop();
void rlf();
void rrf();



/*
 * BIT-ORIENTED FILE REGISTER OPERATIONS
 */
void bcf(CPU_t *cpu, uint16_t opcode);
void bsf(CPU_t *cpu, uint16_t opcode);
void btfsc(CPU_t *cpu, uint16_t opcode);
void btfss(CPU_t *cpu, uint16_t opcode);

/*
 * LITERAL AND CONTROL OPERATIONS
 */
void addlw();
void andlw();
void call();
void clrwdt();
void goto_();
void iorlw();
void movlw(CPU_t *cpu, uint16_t opcode);
void retfie();
void retlw();
void return_();
void sleep_();
void sublw();
void xorlw();

void change_multbanks_registers(CPU_t *cpu, uint8_t f);

#endif // PIC12F675_H_
