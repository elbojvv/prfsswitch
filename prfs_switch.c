#include <stdio.h>

#include <pigpio.h>

/*

Losely based on pps.c
E.J. van Veldhuizen
GPL license

*/

#define PRFS_MODE_FILENAME  "/proc/prfs_mode"   /* PRFS mode file name for the kernel module */

#define GPIO_LED       23   /* gpio for LED */
#define GPIO_SWITCH1   24   /* gpio for switch: prfs mode */
#define GPIO_SWITCH2   25   /* gpio for switch: reversed prfs mode */
#define DELAY_MS        1   /* delay (ms) in loop */

#define PRFS_MODE_PRFS   0   /* PRFS mode: PRFS */
#define PRFS_MODE_RO     1   /* PRFS mode: read only */
#define PRFS_MODE_RPRFS  2   /* PRFS mode: reversed PRFS */


int msec = 0;
int prfsmode = PRFS_MODE_RO;
int prfsmode_old;

void write_prfsmode_file()
{
   FILE *fp;

   fp = fopen(PRFS_MODE_FILENAME,"w");

   if(fp == NULL)
   {
      printf("Error opening PRFS mode file.\n");   
      return;             
   }
   fprintf(fp, "%i\n", prfsmode);
   fclose(fp);
   
   printf("PRFS mode %i written.\n", prfsmode);
}

int get_switch() {
	int sw1, sw2;

	sw1 = gpioRead(GPIO_SWITCH1);
	sw2 = gpioRead(GPIO_SWITCH2);

	if ((sw1 == PI_BAD_GPIO) || (sw2 == PI_BAD_GPIO)) return PRFS_MODE_RO;
	if (sw1 == 0) return PRFS_MODE_PRFS;
	if (sw2 == 0) return PRFS_MODE_RPRFS;
	return PRFS_MODE_RO;
}

void alert(int gpio, int level, uint32_t tick)
{
	prfsmode = get_switch();
	if (prfsmode != prfsmode_old)
	{		
		prfsmode_old = prfsmode;
		write_prfsmode_file();
	}
}

int main(int argc, char *argv[])
{
	int msec=0;
	prfsmode_old = prfsmode;

	if (gpioInitialise()<0) return 1;

	gpioSetMode(GPIO_LED, PI_OUTPUT);
	gpioSetMode(GPIO_SWITCH1, PI_INPUT);
	gpioSetMode(GPIO_SWITCH2, PI_INPUT);
	gpioSetPullUpDown(GPIO_SWITCH1, PI_PUD_UP);
	gpioSetPullUpDown(GPIO_SWITCH2, PI_PUD_UP);

	gpioDelay(1000); // time to settle switch
	prfsmode = get_switch();
	write_prfsmode_file(); // write current status before alert

	gpioSetAlertFunc(GPIO_SWITCH1, alert);
	gpioSetAlertFunc(GPIO_SWITCH2, alert);

	while (1)
	{
		int time_on=0;
		if (prfsmode == PRFS_MODE_PRFS) time_on = 1;
		if (prfsmode == PRFS_MODE_RO) time_on = 100;
		if (prfsmode == PRFS_MODE_RPRFS) time_on = 500;
		if (msec < time_on)
		{
			gpioWrite(GPIO_LED, PI_ON);
		} else
		{
			gpioWrite(GPIO_LED, PI_OFF);
		}
		msec += DELAY_MS;
		if (msec > 1000) msec = 0;
		gpioDelay(DELAY_MS * 1000);
	}
	
	gpioTerminate();
}
