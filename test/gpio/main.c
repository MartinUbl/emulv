/**
 * @file main.c GPIO peripherial test
 * @author Stanislav Kafara
 * @version 1
 */


#include <stdint.h>


#define GPIOA_CTL0  0x40010800
#define GPIOA_ISTAT 0x40010808
#define GPIOA_OCTL  0x4001080C
#define GPIOA_BOP   0x40010810

#define GPIOC_CTL1  0x40011004
#define GPIOC_ISTAT 0x40011008
#define GPIOC_OCTL  0x4001100C
#define GPIOC_BOP   0x40011010

#define CTL_OUTPUT  0b00
#define MD_OUTPUT   0b01

#define CTL_INPUT   0b10
#define MD_INPUT    0b00


/**
 * Sets up PA1 (phys. green LED), PC13 (phys. red LED), PA2 (phys. blue LED) as OUTPUT.
 * Sets up PA0 as INPUT.
 * Switches the LEDs ON using directly OCTL register and indirectly using BOP register.
 * Reacts on HIGH/LOW INPUT LEVEL on PA0 by switching HIGH/LOW on PA2 OUTPUT (ON/OFF phys. blue LED).
 */
int main() {
    volatile uint32_t *address;
    uint32_t value;

    // BEGIN INITIALIZATION

    // (PA0) Initialize PA0 as INPUT.
    address = (uint32_t *) GPIOA_CTL0;
    // (PA0) Set CTL0 in CTLO to INPUT (10). (default 01, bit 2,3)
    // (PA1) Set MD0 in CTL0 to INPUT (00). (default 00, bit 0,1)
    value = 0b11111111111111111111111111110000;
    *address = *address & value;
    value = 0b00000000000000000000000000001000;
    *address = *address | value;

    // (PA1) Initialize PA1 as OUTPUT.
    address = (uint32_t *) GPIOA_CTL0;
    // (PA1) Set CTL1 in CTLO to OUTPUT (00). (default 01, bit 6,7)
    // (PA1) Set MD1 in CTL0 to OUTPUT (01). (default 00, bit 4,5)
    value = 0b11111111111111111111111100001111;
    *address = *address & value;
    value = 0b00000000000000000000000000010000;
    *address = *address | value;

    // (PA2) Initialize PA2 as OUTPUT.
    address = (uint32_t *) GPIOA_CTL0;
    // (PA2) Set CTL2 in CTLO to OUTPUT (00). (default 01, bit 10,11)
    // (PA2) Set MD2 in CTL0 to OUTPUT (01). (default 00, bit 8,9)
    value = 0b11111111111111111111000011111111;
    *address = *address & value;
    value = 0b00000000000000000000000100000000;
    *address = *address | value;

    // (PC13) Initialize PC13 as OUTPUT.
    address = (uint32_t *) GPIOA_CTL0;
    // (PC13) Set CTL13 in CTL1 to OUTPUT (00). (default 01, bit 22,23)
    // (PC13) Set MD13 in CTL1 to OUTPUT (01). (default 00, bit 20,21)
    value = 0b11111111000011111111111111111111;
    *address = *address & value;
    value = 0b00000000000100000000000000000000;
    *address = *address | value;

    // END INITIALIZATION

    // BEGIN MANUAL SWITCH OUTPUT HIGH
    
    // (PA1) Set OCTL1 to HIGH using OCTL.
    address = (uint32_t *) GPIOA_OCTL;
    value = 0b00000000000000000000000000000010;
    *address = *address | value;
    
    // (PC13) Set OCTL13 to HIGH using BOP.
    address = (uint32_t *) GPIOC_BOP;
    value = 0b00000000000000000000000000000010;
    *address = *address | value;

    // END MANUAL SWITCH OUTPUT HIGH

    // BEGIN LOOP PA0 INPUT -> PA2 OUTPUT

    while (1) {
        // (PA0) Get pin INPUT LEVEL.
        address = (uint32_t *) GPIOA_ISTAT;
        // value 1 if INPUT LEVEL HIGH else 0
        value = *address & 0b00000000000000000000000000000001;
        // (PA2) Set address to GPIOA OCTL register.
        address = (uint32_t *) GPIOA_OCTL;
        // (PA0) INPUT LEVEL HIGH
        if (value > 0) {
            // (PA2) OUPTUT LEVEL HIGH
            value = 0b00000000000000000000000000000100;
            *address = *address | value;
        }
        // (PA0) INPUT LEVEL LOW
        else {
            // (PA2) OUPTUT LEVEL LOW
            value = 0b11111111111111111111111111111011;
            *address = *address & value;
        }
    }

    // END LOOP PA0 INPUT -> PA2 OUTPUT

    return 0;
}
