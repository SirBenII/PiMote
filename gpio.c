#include "main.h"
#include "gpio.h"

// IO Acces
struct bcm2835_peripheral {
    unsigned long addr_p;
    int mem_fd;
    void *map;
    volatile unsigned int *addr;
};
struct bcm2835_peripheral gpio = {GPIO_BASE};


int map_peripheral(struct bcm2835_peripheral *p)
{
   // Open /dev/mem
   if ((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("Failed to open /dev/mem, try checking permissions.\n");
      return -1;
   }
   p->map = mmap(
      NULL,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED,
      p->mem_fd,      // File descriptor to physical memory virtual file '/dev/mem'
      p->addr_p       // address in physical map that we want this memory block to expose
   );
   if (p->map == MAP_FAILED) {
        perror("mmap");
        return -1;
   }
   p->addr = (volatile unsigned int *)p->map;
   return 0;
}
 
void unmap_peripheral(struct bcm2835_peripheral *p) {
    munmap(p->map, BLOCK_SIZE);
    close(p->mem_fd);
}

void buzzer_short(int seq){
	INP_GPIO(4);
	OUT_GPIO(4);
	for (int i = 0; i < seq; ++i)
	{
		if(seq > 1){usleep(50000);}
		GPIO_SET = 1 << 4;
		usleep(50000);
		GPIO_CLR = 1 << 4;
	}
}

void gpio_init(){
  if(map_peripheral(&gpio) == -1) 
    {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
    }
}