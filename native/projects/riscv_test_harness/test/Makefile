PROJECT = test

# where your rv32 toolchain is
TCPATH = /home/lily/bin/riscv/bin
PREFIX = $(TCPATH)/riscv32-unknown-elf

CC = $(PREFIX)-gcc
CXX = $(PREFIX)-g++

ARCHFLAGS = -ffreestanding -fno-stack-protector -fdata-sections -ffunction-sections -march=rv32ima -mabi=ilp32 
CCFLAGS = -g -Os $(ARCHFLAGS) -std=c18
CXXFLAGS = $(ARCHFLAGS) -g -Os -std=c++20 -fno-exceptions -fno-rtti
LDFLAGS = -T binary.ld -nostdlib -Wl,--gc-sections

OBJS = start.o \
	main.o

.PHONY: all test clean

all: $(PROJECT).bin $(PROJECT).debug.txt

# this assumes the lcpu project build dir you're using is 
# [lcpu repo root]/build
test: $(PROJECT).bin $(PROJECT).debug.txt
	../../../../build/projects/riscv_test_harness/rvtest $<

clean:
	rm $(PROJECT).elf $(PROJECT).bin $(PROJECT).debug.txt $(OBJS)

# Link rules

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


