led.bin : Start.o init.o led.o
	arm-linux-ld -Tnand.lds -o led_elf $^
	arm-linux-objcopy -O binary -S led_elf $@
%.o : %.c
	arm-linux-gcc -nostdlib -c -o $@ $<
%.o : %.s
	arm-linux-gcc -nostdlib -c -o $@ $^
clean:
	rm -f *.o *.dis *_elf *.bin
