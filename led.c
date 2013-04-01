/*
    File Name:      led.c
    Author:         zengweitotty
    Version:        V1.0
    Data:           2013/03/30
    Email:          zengweitotty@gmail.com
    Description     test TQ2440 board LED
*/
/*LED related register*/

#define GPBCON	(*(volatile unsigned long *)0xA0000010)	//used virtual address
#define GPBDAT	(*(volatile unsigned long *)0xA0000014)
#define GPBUP	(*(volatile unsigned long *)0xA0000018)

/*Key related register*/

#define GPFCON	(*(volatile unsigned long *)0xA0000050)
#define GPFDAT	(*(volatile unsigned long *)0xA0000054)
#define GPFUP	(*(volatile unsigned long *)0xA0000058)

/*LED related register*/
/*
#define GPBCON	(*(volatile unsigned long *)0x56000010)	//used virtual address
#define GPBDAT	(*(volatile unsigned long *)0x56000014)
#define GPBUP	(*(volatile unsigned long *)0x56000018)
*/
/*Key related register*/
/*
#define GPFCON	(*(volatile unsigned long *)0x56000050)
#define GPFDAT	(*(volatile unsigned long *)0x56000054)
#define GPFUP	(*(volatile unsigned long *)0x56000058)
*/
int main(void){
    int index  = 0;
	int temp = 0;
	/*setup GPB5 GPB6 GPB7 GPB8 as output*/
	GPBCON &= ~(0x11 << 10 | 
				0x11 << 12 |
				0x11 << 14 |
				0x11 << 16
				);
	GPBCON |= (0x01 << 10 |
			  0x01 << 12 |
			  0x01 << 14 |
			  0x01 << 16
			  );
	/*setup GPB5 GPB6 GPB7 GPB8 as pull up */
	//GPBUP &= ~(0x0 << 5 |
	//           0x0 << 6 |
	//		   0x0 << 7 |
	//		   0x0 << 8 
	//		  );
	//GPBUP |= (0x1 << 5 |
	//          0x1 << 6 |
	//		  0x1 << 7 |
	//		  0x1 << 8
	//		 );
	/*setup GPF0,GPF1,GPF2,GPF4*/
	GPFCON &= ~(0x11 << 0 |
				0x11 << 2 |
				0x11 << 4 |
				0x11 << 8 
				);
	GPFUP &= ~(0x1 << 0 |
			   0x1 << 1 |
			   0x1 << 2 |
			   0x1 << 4
			  );
	/*trigger led on or off*/
	while(1){
		temp = GPFDAT;
		if(temp & 0x1){//trigger led_4 light on
			GPBDAT &= ~(0x1 << 8);
		}else{
			GPBDAT |= (0x1 << 8);
		}
		if(temp & (0x1 << 1)){//trigger led_1 light on
			GPBDAT &= ~(0x1 << 5);
		}else{
			GPBDAT |= (0x1 << 5);
		}
		if(temp & (0x1 << 2)){//trigger led_3 light on
			GPBDAT &= ~(0x1 << 7);
		}else{
			GPBDAT |= (0x1 << 7);
		}
		if(temp & (0x1 << 4)){//trigger led_2 light on
			GPBDAT &= ~(0x1 << 6);
		}else{
			GPBDAT |= (0x1 << 6);
		}

	}
	return 0;		
}

