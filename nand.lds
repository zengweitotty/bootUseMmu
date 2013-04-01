SECTIONS{
	first 0x00000000 : AT(0) {Start.o init.o}	
	second 0x30004000 : AT(2048) {led.o} 
}
