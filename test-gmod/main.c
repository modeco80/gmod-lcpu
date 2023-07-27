// a simple test program - this version would talk to a device
// written in GLua
#include <stdarg.h>
#include <stdint.h>

uint32_t strlen(const char* str) {
	if(!str)
		return 0;
	const char* c = str;
	while(*c++)
		;
	return c - str;
}

#define GLUA_DEVICE_BASE 0x11300000									   // base address of the lua test device
#define GLUA_DEVICE_WORLDTIME *(volatile uint32_t*)GLUA_DEVICE_BASE	   // world time register (read only)
#define GLUA_DEVICE_LUAREG *(volatile uint32_t*)(GLUA_DEVICE_BASE + 4) // lua register (read/write)

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
	puts("fuck you garry I win");

	for(int i = 0; i < 8; ++i)
		printf("GLUA_DEVICE_WORLDTIME reading says -> %d\n", GLUA_DEVICE_WORLDTIME);

	// try writing to it
	GLUA_DEVICE_LUAREG = 0x1234;

	for(int i = 0; i < 8; ++i)
		printf("GLUA_DEVICE_LUAREG reading says -> %d\n", GLUA_DEVICE_LUAREG);

	SYSCON = 0x5555;
}
