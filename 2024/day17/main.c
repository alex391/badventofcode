#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "panic.c"
#include <stdbool.h>

#define INSTRUCTION_COUNT 16

int32_t instruction_pointer = 0;

struct uint3_t {
	uint8_t value: 3;
};

struct registers {
	int64_t a;
	int64_t b;
	int64_t c;
} registers = { 0 };

int64_t decode_combo(struct uint3_t operand)
{
	switch (operand.value) {
		case 0:
		case 1:
		case 2:
		case 3:
			return operand.value;
		case 4:
			return registers.a;
		case 5:
			return registers.b;
		case 6:
			return registers.c;
	}
	return -1; // unreachable in normal circumstances
}

int64_t int64_t_pow(int64_t base, int64_t exponent)
{
	int64_t power = 1;
	for (int64_t i = 0; i < exponent; i++) {
		power *= base;
	}
	return power;
}

int64_t dv_helper(struct uint3_t combo)
{
 	int64_t numerator = registers.a;
	int64_t denominator = int64_t_pow(2, decode_combo(combo));
	return numerator / denominator;
}

void adv(struct uint3_t combo)
{
	registers.a = dv_helper(combo);
}

void bxl(struct uint3_t literal)
{
	registers.b = registers.b ^ (int64_t)literal.value;
}

// https://stackoverflow.com/a/14997413/12203444
// the problem specifically says modulo, so I'm using modulo, even though
// remainder might work just as well
int64_t positive_modulo(int64_t i, int64_t n)
{
	return (i % n + n) % n;
}

void bst(struct uint3_t combo)
{
	registers.b = positive_modulo(decode_combo(combo), 8);
}

void jnz(struct uint3_t literal)
{
	if (!registers.a) {
		return;
	}
	instruction_pointer = literal.value;
	instruction_pointer -= 2; // compensate for the += 2 later
}

void bxc(struct uint3_t _ignored)
{
	registers.b |= registers.c;
}

void out(struct uint3_t combo)
{
	int64_t o = positive_modulo(decode_combo(combo), 8);
	printf("%" PRIi64 ",", o);
}

void bdv(struct uint3_t combo)
{
	registers.b = dv_helper(combo);
}

void cdv(struct uint3_t combo)
{
	registers.c = dv_helper(combo);
}

void run_opcode(struct uint3_t opcode, struct uint3_t operand)
{
	void (*instructions[8]) (struct uint3_t operand) = {
		adv,
		bxl,
		bst,
		jnz,
		bxc,
		out,
		bdv,
		cdv,
	};
	instructions[opcode.value](operand);
	instruction_pointer += 2;
}

int main()
{
	struct uint3_t program[INSTRUCTION_COUNT] = { 0 };

	FILE *input = panic_if_zero(fopen("input.txt", "r"));
	fscanf(input, "Register A: %" SCNi64 " Register B: %" SCNi64 " Register C: %" SCNi64 " Program: ", &registers.a, &registers.b, &registers.c);
	for(uint8_t i = 0; i < INSTRUCTION_COUNT; i++) {
		uint8_t temp = 0;
		fscanf(input,"%" SCNu8, &temp);
		program[i].value = temp;
		fscanf(input, ",");
	}
	fclose(input);
	
	while(true) {
		if (instruction_pointer >= INSTRUCTION_COUNT - 1) {
			return 0;
		}
		run_opcode(program[instruction_pointer], program[instruction_pointer + 1]);
	}
}
