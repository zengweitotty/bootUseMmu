/*
    File Name:      init.c
	Author:         zengweitotty
    Version:        V1.0
    Data:           2013/03/23
    Email:          zengweitotty@gmail.com
    Description     Initialize TQ2440 hardware and boot from nandflash
					corprocessor register can be found in ARM architecture from www.infocenter.arm.com
*/

#define WTCON	(*(volatile unsigned long *)0x53000000)
#define MEM_CTL_BASE	((volatile unsigned long *)0x48000000)
#define MMU_FULL_ACCESS	(0x3 << 11)
#define MMU_DOMAIN	(0x0 << 5)
#define MMU_SPECIAL	(0x1 << 4)
#define MMU_CACHEABLE	(0x1 << 3)
#define MMU_BUFFERABLE	(0x1 << 2)
#define MMU_SECTION	(0x2)
#define MMU_SECDESC	(MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_SECTION)
#define MMU_SECDESC_WB	(MMU_SECDESC | MMU_CACHEABLE | MMU_BUFFERABLE)

void disable_watchdog(void){
	WTCON = (unsigned long)0x00;
}

void memsetup(void){
	//copy from TQ2440 u-boot,to initialize SDRAM
	unsigned long const mem_cfg_val[] = { 0x22011110,/*BWSCON*/
											0x00000700,/*BANKCON0*/
											0x00000700,/*BANKCON1*/
											0x00000700,/*BANKCON2*/
											0x00001F4C,/*BANKCON3*/
											0x00000700,/*BANKCON4*/
											0x00000700,/*BANKCON5*/
											0x00018005,/*BANKCON6 with SDRAM*/
											0x00018005,/*BANKCON7 with SDRAM*/
											0x008E04F4,/*REFRESH*/
											0x00000032,/*BANKSIZE*/
											0x00000030,/*MRSRB6*/
											0x00000030,/*MRSRB7*/
										};
	/*
	volatile unsigned long* p = (volatile unsigned long *)MEM_CTL_BASE;
	for(index = 0;index < 13;index++){
		p[index] = (unsigned long)mem_cfg_val[index];	//initialize SDRAM
	}
	*/
	int index = 0;
	volatile unsigned long *p = (volatile unsigned long *)0x48000000;
	for(index = 0;index < 13;index++){
		p[index] = mem_cfg_val[index];	//initialize SDRAM
	}
}
void copy_2th_to_ram(void){
	int index = 0;
	volatile unsigned int *pSrc;
	volatile unsigned int *pDst;
	pSrc = (volatile unsigned int *)/*0x00000000*/2048;
	pDst = (volatile unsigned int *)/*0x30000000*/0x30004000;
	for(index = 0;pSrc < (volatile unsigned int *)4096/*2048*/;index++){
		*pDst = *pSrc;//copy from steppingstone address 0x2048 to SDRAM 0x30004000	
		pSrc++;
		pDst++;
	}
}
void create_page_table(void){
	unsigned long virtualaddr,physicaladdr;
	volatile unsigned long *mmu_ttb_base;
	mmu_ttb_base = (volatile unsigned long *)0x30000000;
	/*SECTION 0x00000000 -- 0x100000*/
	virtualaddr = 0;
	physicaladdr = 0;
	*(mmu_ttb_base + (virtualaddr >> 20)) = (physicaladdr & 0xFFF00000) | MMU_SECDESC_WB;
	/*SECTION 0x56000000 -- 0x57000000*/
	virtualaddr = 0xA0000000;
	physicaladdr = 0x56000000;
	*(mmu_ttb_base + (virtualaddr >> 20)) = (physicaladdr & 0xFFF00000) | MMU_SECDESC;	//io control not use cache.otherwise it will not work properly
	/*SECTION 0x30000000 -- 0x33FFFFFF*/
	virtualaddr = 0xB0000000;
	physicaladdr = 0x30000000;
	while(physicaladdr < 0x34000000){
		*(mmu_ttb_base + (virtualaddr >> 20)) = (physicaladdr & 0xFFF00000) | MMU_SECDESC_WB;
		virtualaddr += 0x100000;
		physicaladdr += 0x100000;
	}
}
void mmu_init(void){
	unsigned long ttb = 0x30000000;	//ttb address
	__asm__(
		"mov r0,#0\n"
		"mcr p15,0,r0,c7,c7,0\n"
		"mcr p15,0,r0,c7,c10,4\n"
		"mcr p15,0,r0,c8,c7,0\n"
		
		"mov r4,%0\n"
		"mcr p15,0,r4,c2,c0,0\n"
		
		"mov r0,#0xFFFFFFFF\n"
		"mcr p15,0,r0,c3,c0,0\n"

		"mrc p15,0,r0,c1,c0,0\n"

		"bic r0,r0,#0x3000\n"
		"bic r0,r0,#0x0300\n"
		"bic r0,r0,#0x0087\n"
		
		"orr r0,r0,#0x0002\n"
		"orr r0,r0,#0x0004\n"
		"orr r0,r0,#0x1000\n"
		"orr r0,r0,#0x0001\n"

		"mcr p15,0,r0,c1,c0,0\n"
		:
		:"r"(ttb)
	);
}
