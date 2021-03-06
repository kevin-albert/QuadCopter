/* 
 * File:   rpi.h
 * Author: kevin
 *
 * Created on April 3, 2014, 2:55 PM
 */

#ifndef RPI_H
#define RPI_H

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BCM2708_PERI_BASE   0x20000000
#define GPIO_BASE           (BCM2708_PERI_BASE + 0x200000)  // GPIO controller
#define BLOCK_SIZE          (4*1024)

// IO Access
struct bcm2835_peripheral {
    unsigned long addr_p;
    int mem_fd;
    void *map;
    volatile unsigned int *addr;
};

extern struct bcm2835_peripheral gpio;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x)
#define INP_GPIO(g)         *(gpio.addr + ((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)         *(gpio.addr + ((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a)   *(gpio.addr + (((g)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_SET            *(gpio.addr + 7)  //   sets bits which are 1 ignores bits which are 0
#define GPIO_CLR            *(gpio.addr + 10) // clears bits which are 1 ignores bits which are 0
#define GPIO_READ(g)        *(gpio.addr + 13) &= (1<<(g))

int map_peripheral(struct bcm2835_peripheral *p);
void unmap_peripheral(struct bcm2835_peripheral *p);

//I2C Controller
#define BSC1_BASE       (BCM2708_PERI_BASE + 0x804000)

extern struct bcm2835_peripheral bsc1;

// I2C macros
// #define BSC1_C          *(bsc1.addr + 0x00)
// #define BSC1_S          *(bsc1.addr + 0x01)
// #define BSC1_DLEN       *(bsc1.addr + 0x02)
// #define BSC1_A          *(bsc1.addr + 0x03)
// #define BSC1_FIFO       *(bsc1.addr + 0x04)
#define BSC1_C          *(bsc1.addr)
#define BSC1_S          *(bsc1.addr + 0x4)
#define BSC1_DLEN       *(bsc1.addr + 0x8)
#define BSC1_A          *(bsc1.addr + 0xc)
#define BSC1_FIFO       *(bsc1.addr + 0x10)

#define BSC_C_I2CEN     (1 << 15)
#define BSC_C_INTR      (1 << 10)
#define BSC_C_INTT      (1 << 9)
#define BSC_C_INTD      (1 << 8)
#define BSC_C_ST        (1 << 7)
#define BSC_C_CLEAR     (1 << 4)
#define BSC_C_READ      1

#define START_READ      BSC_C_I2CEN|BSC_C_ST|BSC_C_CLEAR|BSC_C_READ
#define START_WRITE     BSC_C_I2CEN|BSC_C_ST

#define BSC_S_CLKT      (1 << 9)
#define BSC_S_ERR       (1 << 8)
#define BSC_S_RXF       (1 << 7)
#define BSC_S_TXE       (1 << 6)
#define BSC_S_RXD       (1 << 5)
#define BSC_S_TXD       (1 << 4)
#define BSC_S_RXR       (1 << 3)
#define BSC_S_TXW       (1 << 2)
#define BSC_S_DONE      (1 << 1)
#define BSC_S_TA        1

#define CLEAR_STATUS    BSC_S_CLKT|BSC_S_ERR|BSC_S_DONE

// I2C Function Prototypes
void i2c_Init();
void i2c_Sync();

// Utility
long deltaT_Usecs(struct timespec*);

#endif  /* RPI_H */

