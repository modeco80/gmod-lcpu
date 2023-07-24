PROJECT = test

# where your rv32 toolchain is
TCPATH = /home/lily/bin/riscv/bin
PREFIX = $(TCPATH)/riscv32-unknown-elf

CC = $(PREFIX)-gcc
CXX = $(PREFIX)-g++

CCFLAGS = -ffreestanding -fno-stack-protector
CCFLAGS += -static -static-libgcc -fdata-sections -ffunction-sections
CCFLAGS += -g -Os -march=rv32ima -mabi=ilp32 

CXXFLAGS = $(CCFLAGS) -std=c++20 -fno-exceptions -fno-rtti

LDFLAGS = -T binary.ld -nostdlib -Wl,--gc-sections

OBJS := start.o main.o

.PHONY: all test clean

all: $(PROJECT).bin $(PROJECT).debug.txt

test: $(PROJECT).bin $(PROJECT).debug.txt
	../../../../build/projects/riscv_test_harness/rvtest $<

clean:
	rm $(PROJECT).elf $(PROJECT).bin $(PROJECT).debug.txt $(OBJS)

$(PROJECT).elf: $(OBJS)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $(OBJS) 

$(PROJECT).bin : $(PROJECT).elf
	$(PREFIX)-objcopy $^ -O binary $@

$(PROJECT).debug.txt : $(PROJECT).elf
	$(PREFIX)-objdump -t $^ > $@
	$(PREFIX)-objdump -S $^ >> $@

# Compile rules

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CCFLAGS) $< -o $@

%.o: %.S
	$(CC) -x assembler-with-cpp -march=rv32ima -mabi=ilp32 -c $< -o $@


