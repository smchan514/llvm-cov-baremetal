BIN = test.bin
ELF = test.elf
OBJS_AS = startup.o
OBJS_C = \
  entry.o \
  test.o \
  llvm-cov.o
LIBS =
INCLUDES =
CFLAGS = --coverage
LDFLAGS = -Map=cov.map

all: $(BIN)

clean:
	rm -f $(OBJS_AS) $(OBJS_C)
	rm -f $(ELF) $(BIN)
	rm -f *.gcno
	rm -f cov.map

%.o: %.s
	arm-none-eabi-as -mcpu=arm926ej-s -o $@ $<

%.o: %.c
	clang --target=arm-non-eabi $(CFLAGS) -c -o $@ $<

$(ELF): $(OBJS_AS) $(OBJS_C)
	arm-none-eabi-ld $(LDFLAGS) -T test.ld $(OBJS_AS) $(OBJS_C) -o $(ELF)

$(BIN): $(ELF)
	arm-none-eabi-objcopy -O binary $(ELF) $(BIN)