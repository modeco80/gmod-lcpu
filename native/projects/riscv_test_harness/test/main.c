// a simple test program
#include <stdint.h>

uint32_t strlen(const char* str) {
	if(!str)
		return 0;
	const char* c = str;
	while(*c++)
		;
	return c - str;
}

#define SYSCON *(volatile uint32_t*)0x11100000

#define UART_BASE 0x10000000
#define UART_DATA *(volatile uint32_t*)UART_BASE
#define UART_STATUS UART_DATA

void putc(char c) {
	UART_DATA = (uint32_t)c;
}

__attribute__((noinline)) void puts(const char* str) {
	const uint32_t length = strlen(str);
	for(uint32_t i = 0; i < length; ++i)
		putc(str[i]);
}

static uint32_t value = 0;
static uint16_t shortvalue = 0;
static uint8_t bytevalue = 0;

#define COUNTER_TEST(var, max)                    \
	for(int i = 0; i < max; ++i) {                \
		puts(#var " is (before modification): "); \
		putc("0123456789"[var]);                  \
		putc('\n');                               \
                                                  \
		var = i;                                  \
		puts(#var " is (after  modification): "); \
		putc("0123456789"[var]);                  \
		putc('\n');                               \
	}

void main() {
	puts("hello world I guess\n");

#if 1
	COUNTER_TEST(value, 9);
	COUNTER_TEST(shortvalue, 9);
	COUNTER_TEST(bytevalue, 9);
#endif

	// Shut down the test harness once we're done testing.
	puts("Tests done, shutting down test harness...\n");
	//SYSCON = 0x5555;

	// loop forever
	for(;;);
}
