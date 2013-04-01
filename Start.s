@***********************************
@File: Start.s
@Description: first boot section
@***********************************
.text
.global _start
_start:
	ldr sp,=4096 @use C function,so need stack
	bl disable_watchdog
	bl memsetup
@bl memctl_setup
	bl copy_2th_to_ram
	bl create_page_table
	bl mmu_init
	ldr sp,=0xB4000000
	ldr pc,=0xB0004000
halt_loop:
	b halt_loop
memctl_setup:
    mov r1,#0x48000000
	adrl r2,mem_cfg_val
	add r3,r1,#52
loop2:
    ldr r4,[r2],#4
    str r4,[r1],#4
    cmp r3,r1
    bne loop2
    mov pc,lr
.align 4
mem_cfg_val:
    .long 0x22011110 @BWSCON
    .long 0x00000700 @BANKCON0
    .long 0x00000700 @BANKCON1
    .long 0x00000700 @BANKCON2
    .long 0x00001F4C @BANKCON3
    .long 0x00000700 @BANKCON4
    .long 0x00000700 @BANKCON5
    .long 0x00018005 @BANKCON6 with SDRAM
    .long 0x00018005 @BANKCON7 with SDRAM
    .long 0x008E04F4 @REFRESH
    .long 0x00000032 @BANKSIZE
    .long 0x00000030 @MRSRB6
    .long 0x00000030 @MRSRB7
