#include <stdint.h>

/* Base Addresses */
#define UARTA       (0x03100000L)
#define PADCTL_A    (0x02430000L)

/* UART Registers */
#define UART_THR_DLAB_0_0       (*(volatile uint32_t *)(UARTA))
#define UART_IER_DLAB_0_0       (*(volatile uint32_t *)(UARTA + 0x04))
#define UART_FCR_0              (*(volatile uint32_t *)(UARTA + 0x08))
#define UART_LCR_0              (*(volatile uint32_t *)(UARTA + 0x0C))
#define UART_LSR_0              (*(volatile uint32_t *)(UARTA + 0x14))
#define PADCTL_G3_UART1_TX_0    (*(volatile uint32_t *)(PADCTL_A + 0xA8))

/* UART Constants */
#define DLAB                    (1 << 7)
#define STOP_BITS               (0 << 2)    /* 1 stop bit default */
#define WORD_LENGTH             (3 << 0)
#define LCR_REG_CONFIG          (DLAB | STOP_BITS | WORD_LENGTH)
#define BAUD_RATE_INTEGER       (13 << 0)   /* 115200 baud */
#define TX_RX_FIFO_ENABLE       (1 << 0)
#define TX_REG_EMPTY            (1 << 5)
#define TRISTATE_MASK           (1 << 4)

void uartSetup(void)
{
    /* TX Pin Initialization */
    PADCTL_G3_UART1_TX_0 &= ~TRISTATE_MASK; /* Sets TX pin to tristate passthrough 
                                             *
                                             *   passthrough: active-pass state
                                             *   tristate: high-impedance disconnected state */

    /* UART Configuration */
    UART_LCR_0 = LCR_REG_CONFIG;            /* Enables DLAB, Stop Bits = 0, Word Length = 8 */
    UART_THR_DLAB_0_0 = BAUD_RATE_INTEGER;  /* Sets baud rate integer */
    UART_LCR_0 ^= DLAB;                     /* Disables DLAB */
    UART_IER_DLAB_0_0 = 0;                  /* Ensures all interupts are disabled */

    /*
    FIFO ALREADY ENABLED ON RESET

    UART_FCR_0 = TX_RX_FIFO_ENABLE; // Enables TX and RX FIFO
    */
}

void uartTransmit(char *data)
{
    while (*data != '\0')
    {
        while (!(UART_LSR_0 & TX_REG_EMPTY));   /* Waits for Transmit FIFO register to be empty -> ready to send */
        UART_THR_DLAB_0_0 = *data;              /* Sends char */
        data++;
    }
}

int main(void)
{
    uartSetup();
    volatile uint64_t delay;

    while (1)
    {
        uartTransmit("Nikhil\n");
        for (delay = 0; delay < 1000000; delay++);
    }
    return 0;
}