// Test program for lcpu
#include <stdarg.h>
#include <stdint.h>

// MMIO types

/// This structure is at WIRE_BASE
typedef struct {
	uint32_t nrInputs;
	uint32_t nrOutputs;
} WireDevice_Header;

#define WIRE_BASE 0x11310000
#define WIRE_HEADER ((volatile WireDevice_Header*)WIRE_BASE)
#define WIRE_IO_BASE WIRE_BASE + sizeof(WireDevice_Header)
#define WIRE_OUTPUT_BASE WIRE_IO_BASE + (WIRE_HEADER->nrInputs * sizeof(uint32_t)) 
#define WIRE_INPUT(i) *(volatile const uint32_t*)(WIRE_IO_BASE + (i * sizeof(uint32_t)))
#define WIRE_OUTPUT(i) *(volatile uint32_t*)(WIRE_OUTPUT_BASE + (i * sizeof(uint32_t)))

#define SYSCON *(volatile uint32_t*)0x11100000

#define UART_BASE 0x10000000
#define UART_DATA *(volatile uint32_t*)UART_BASE
#define UART_STATUS UART_DATA

uint32_t strlen(const char* str) {
	if(!str)
		return 0;
	const char* c = str;
	while(*c++)
		;
	return c - str;
}


void putc(char c) {
	UART_DATA = (uint32_t)c;
}

__attribute__((noinline)) void puts(const char* str) {
	const uint32_t length = strlen(str);
	for(uint32_t i = 0; i < length; ++i)
		putc(str[i]);
}

int itoa(int value, char* sp, int radix) {
	char tmp[16];
	char* tp = tmp;
	int i;
	unsigned v;

	int sign = (radix == 10 && value < 0);
	if(sign)
		v = -value;
	else
		v = (unsigned)value;

	while(v || tp == tmp) {
		i = v % radix;
		v /= radix;
		if(i < 10)
			*tp++ = i + '0';
		else
			*tp++ = i + 'a' - 10;
	}

	int len = tp - tmp;

	if(sign) {
		*sp++ = '-';
		len++;
	}

	while(tp > tmp)
		*sp++ = *--tp;

	*sp = '\0';
	return len;
}

void vprintf(const char* format, va_list val) {
	const int fl = strlen(format);
	for(int i = 0; i < fl; ++i) {
		switch(format[i]) {
			case '%':
				if(format[i + 1] == '%')
					putc('%');
				switch(format[i + 1]) {
					case 'i':
					case 'd': {
						char a[32];
						itoa(va_arg(val, uint32_t), &a[0], 10);

						const int al = strlen(a);
						for(int j = 0; j < al; ++j)
							putc(a[j]);
						i++;
					} break;

					case 's': {
						char* p = va_arg(val, char*);
						if(!p)
							puts("(null)");
						else
							puts(p);
						i++;
					};

					default: putc(' '); break;
				}
				break;
			case '\0': // band-aid fix.
				return;
			default: putc(format[i]); break;
		}
	}
}

void printf(const char* format, ...) {
	va_list val;
	va_start(val, format);
	vprintf(format, val);
	va_end(val);
}

void main() {
	printf("Wire interface: %d inputs, %d outputs\n", WIRE_HEADER->nrInputs, WIRE_HEADER->nrOutputs);

	for(uint32_t i = 0; i < WIRE_HEADER->nrInputs; ++i)
		printf("Wire input %d: value %d\n", i, WIRE_INPUT(i));

	for(uint32_t i = 0; i < WIRE_HEADER->nrOutputs; ++i) {
		printf("Setting wire output %d\n", i);
		WIRE_OUTPUT(i) = 10 + i;
	}

#if 1
	for(;;)
		;
	__builtin_unreachable();
#else
	SYSCON = 0x5555;
#endif
}
