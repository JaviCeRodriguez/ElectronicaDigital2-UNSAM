/*                                                                            */
/* Author(s):                                                                 */
/*   Miguel Angel Sagreras                                                    */
/*                                                                            */
/* Copyright (C) 2020                                                         */
/*    Miguel Angel Sagreras                                                   */
/*                                                                            */
/* This source file may be used and distributed without restriction provided  */
/* that this copyright statement is not removed from the file and that any    */
/* derivative work contains  the original copyright notice and the associated */
/* disclaimer.                                                                */
/*                                                                            */
/* This source file is free software; you can redistribute it and/or modify   */
/* it under the terms of the GNU General Public License as published by the   */
/* Free Software Foundation, either version 3 of the License, or (at your     */
/* option) any later version.                                                 */
/*                                                                            */
/* This source is distributed in the hope that it will be useful, but WITHOUT */
/* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      */
/* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for   */
/* more details at http://www.gnu.org/licenses/.                              */
/*                                                                            */

#include "xil_exception.h"
#include <stdio.h>
#include <string.h>

#define MY_MAC		{0x00, 0x40, 0x00, 0x01, 0x02, 0x03}
#define MY_IPV4		{192, 168, 0, 14}

#define SERVER_LOG  0x03
#define STATE_LOG   0x04
#define DMA_LOG     0x08
#define ETH_LOG     0x10
#define INTC_LOG    0x20
#define INTH_LOG    0x40

const int loglevel = SERVER_LOG; // STATE_LOG; // | ETH_LOG; // | INTH_LOG;

/* ********************* */
/* MicroBlaze enviroment */
/* ********************* */

/* Interrupt controller register map */
/* --------------------------------- */

#define INTNUM_BUTTONS_AND_LEDS 0
#define INTNUM_PMODS            1
#define INTNUM_UART             2
#define INTNUM_DMA              3
#define INTNUM_ETHERNET         4
#define INTNUM_TIMER            5

//// Máscara que se utiliza para habilitar interrupciones del DMA y del Ethernet
#define IE_MASK	((1 << INTNUM_ETHERNET) | ((1 << INTNUM_DMA)))

struct intc {
	int isr;             // Interrupt Status Register (ISR)
	int ipr;             // Interrupt Pending Register (IPR)
	int ier;             // Interrupt Enable Register (IER)
	int iar;             // Interrupt Acknowledge Register (IAR)
	int sie;             // Set Interrupt Enables (SIE)
	int cie;             // Clear Interrupt Enables (CIE)
	int ivr;             // Interrupt Vector Register (IVR)
	int mer;             // Master Enable Register (MER)
	int imr;             // Interrupt Mode Register (IMR)
	int ilr;             // Interrupt Level Register (ILR)
	int reserved1[54];
	int ivar[32];        // Interrupt Vector Address Register (IVAR)
	int reserved2[32];
	int ivear[32];       // Interrupt Vector Extended Address Register (IVEAR)
};

/* CDMA controller register map */
/* ---------------------------- */

#define DMA_CRBIT_IRQEN		12
#define DMA_CRBIT_IRQERR	14

//// Másacara para el bit 12 del registro, representando un 1
//// CDMACR (CDMA Control - 00h), página 16
#define DMA_CRMSK_IRQEN		(1 << DMA_CRBIT_IRQEN)
//// Másacara para el bit 14 del registro, representando un 1
//// CDMACR (CDMA Control - 00h), página 15
#define DMA_CRMSK_IRQERR	(1 << DMA_CRBIT_IRQERR)

#define DMA_SRBIT_IDLE		1
//// Máscara para el bit 1 del registro, representando un 1
//// CDMASR (CDMA Status - 04h), página 22
#define DMA_SRMSK_IDLE		(1 << DMA_SRBIT_IDLE)

struct dma {
	int cdmacr;            // CDMA Control
	int cdmasr;            // CDMA Control
	int curdesc_pntr;      // Current Descriptor Pointer
	int curdesc_pntr_msb;  // Current Descriptor Pointer MSB if address space is greater than 32bits
	int taildesc_pntr;     // Tail description Pointer
	int taildesc_pntr_msb; // Tail description Pointer MSB if address space is greater than 32bits
	int sa;                // Source Address
	int sa_msb;            // Source Address MSB if address space is greater than 32bits
	int da;                // Source Address
	int da_msb;            // Source Address MSB if address space is greater than 32bits
	int btt;               // Byte to transfer
};

/* GPIO controller register map */
/* ---------------------------- */

struct gpio {
	int gpio_data;
	int gpio_tri;
	int gpio2_data;
	int gpio2_tri;
	int reserved1[67];
	int gier;
	int ip_isr;
	int reserved2[1];
	int ip_ier;
};

/* Ethenert Lite controller register map */
/* ------------------------------------- */

struct ethernet {
	int tx_ping_data[505];	 
	int mdioaddr;			
	int mdiowr;
	int mdiord;
	int mdioctrl;

	int tx_ping_length;	   // 0x07f4 Transmit length register for ping buffer
	int gie;               // 0x07f8 Global interrupt register
	int tx_ping_control;   // 0x07fc Transmit length register for ping buffer
	int tx_pong_data[509];
	int tx_pong_length;	   // 0x0ff4 Transmit length register for pong buffer
	int reserved3[1];
	int tx_pong_control;   // 0x0ffc Transmit length register for pong buffer
	int rx_ping_data[511];
	int rx_ping_control;   // 0x17fc Receive  length register for ping buffer
	int rx_pong_data[511];
	int rx_pong_control;   // 0x1ffc Receive  length register for pong buffer
};

/* Devices' memory map */
/* ------------------- */

volatile struct gpio     *const inputs_and_outputs = (struct gpio *)     0x40000000; //// GPIO
volatile struct intc     *const intp_ctlr          = (struct intc *)     0x41200000; //// Interruptor
volatile struct dma      *const dma_ctlr           = (struct dma *)      0x44a00000; //// DMA
volatile struct ethernet *const eth_ctlr           = (struct ethernet *) 0x40e00000; //// Ethernet

/* **************************************** */
/* Header requirement for Internet Protocol */
/* **************************************** */

/* Ethernet Frame Header */
/* --------------------- */

struct eth_hdr {
	char unsigned dest[6];
	char unsigned src[6];
	char unsigned type[2];
};

#define ETHTYP_IP	0x800
#define ETHTYP_ARP	0x806

/* Arp Frame Header for ipv4 */
/* ------------------------- */

struct arp4_hdr {
	char unsigned htype[2];	// Hardware type
	char unsigned ptype[2];	// Protocol type
	char unsigned hlen[1];	// Hardware address length
	char unsigned plen[1];	// Protocol address length
	char unsigned open[2];	// Operation
	char unsigned sha[6];	// Sender hardware address
	char unsigned spa[4];	// Sender protocol address
	char unsigned tha[6];	// Target hardware address
	char unsigned tpa[4];	// Target protocol address
};

/* Arp Over ethernet frame */
/* ----------------------- */

struct arp_frm {
	struct eth_hdr  eth_hdr;
	struct arp4_hdr arp4_hdr;
};

/* Internet Header version 4 */
/* ------------------------- */

struct ipv4_hdr {
	char unsigned verihl[1];	// Version and Header Length
	char unsigned tos[1];		// Type Of Service
	char unsigned len[2];		// Total Length
	char unsigned ident[2];		// Identification
	char unsigned flgsfrg[2];	// Flags and fragment
	char unsigned ttl[1];		// Time To Live
	char unsigned proto[1];		// Protocol
	char unsigned chksum[2];	// Header Checksum
	char unsigned sa[4];		// Source Address
	char unsigned da[4];		// Destination Address
};

const unsigned icmp_proto = 0x01;

/* ICMP Header version */
/* ------------------- */

struct icmp_hdr {
	char unsigned type[1];		// ICMP Type
	char unsigned code[1];		// ICMP Code
	char unsigned chksum[2];	
};

/* ICMP over Ethernet frame */
/* ------------------------ */

struct icmp_frm {
	struct eth_hdr  eth_hdr;
	struct ipv4_hdr ipv4_hdr;
	struct icmp_hdr icmp_hdr;
};

const char unsigned my_mac[]  = MY_MAC;
const char unsigned my_ipv4[] = MY_IPV4;

void print_mac(const char unsigned *mac)
{
	int i;

	for (i = 0; i < 5; i++)
		xil_printf("%02x:", mac[i]);
	xil_printf("%02x", mac[i]);
}

void print_ipv4(const char unsigned *ipv4)
{
	int i;

	for (i = 0; i < 3; i++)
		xil_printf("%d.", ipv4[i]);
	xil_printf("%d", ipv4[i]);
}


const struct arp_frm my_arp = {
	{ 
		{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},	// Destination MAC
		MY_MAC,									// Source MAC
		{0x08, 0x06} 							// Arp protocol
	},
	{ 
		{0x00, 0x01}, 
		{0x08, 0x00}, 
		{0x06},
		{0x04}, 
		{0x00, 0x02},
		MY_MAC,
		MY_IPV4,
		{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
		MY_IPV4
	}
};

#define RX_PKT	0
#define TX_PKT	1

int eth_state;

void ethctlr_handler () //// Documentación: ./documents/pg135-axi-ethernetlite.pdf
{
	//// Ingresa al Receive Control Register (Ping) y verifica si hay un paquete
	//// listo para ser procesado. Receive Control Register (0x17FC), página 22.
	//// Dirección absoluta: 0x40e017FC
	if (eth_ctlr->rx_ping_control & (1<< 0)) {
		if ((loglevel & INTH_LOG) && (loglevel & ETH_LOG)) xil_printf("recevied packet\r\n");
		eth_state |= (1 << RX_PKT);
		/* Clear the Interrupt */

		//// Ingresa al Receive Control Register (Ping) y verifica si el bit de Status
		//// pueda aceptar un nuevo paquete.
		//// Receive Control Register (0x17FC), página 22. Dirección absoluta: 0x40e017FC
		eth_ctlr->rx_ping_control &= ~(1<< 0);
	}
	//// Ingresa al Transmit Control Register (Ping) y verifica que estén habilitadas
	//// las interrupciones para transmitir. Transmit Control Register (0x07FC), página 19.
	//// Dirección absoluta: 0x40e007FC
	if (eth_ctlr->tx_ping_control & (1<< 3)) {
													
		if ((loglevel & INTH_LOG) && (loglevel & ETH_LOG)) xil_printf("transmitted packet\r\n");
		eth_state |= (1 << TX_PKT);
		/* Clear the Interrupt */
		//// Ingresa al Transmit Control Register (Ping) y deshabilita interrupciones para
		//// transmitir. Transmit Control Register (0x07FC), página 19.
		//// Dirección absoluta: 0x40e007FC
		eth_ctlr->tx_ping_control &= ~(1<< 3);
	}
}

enum dma_states { dma_busy, dma_ready };
enum dma_states dma_state;

void dmactlr_handler () //// Documentación: ./documents/pg034-axi-cdma.pdf
{
	dma_state = dma_ready;

	/* Clear the Interrupt */
	//// Ingresa al CDMASR Register y limpia (deshabilita) la interrupción para realizar
	//// transferencia en la DMA. CDMASR (CDMA Status – Offset 04h), página 19.
	//// Dirección absoluta: 0x44a00004
	dma_ctlr->cdmasr &= ~DMA_CRMSK_IRQEN;
	//// Ingresa al CDMASR Register y limpia la interrupción generada por un error.
	//// CDMASR (CDMA Status – Offset 04h), página 18.
	//// Dirección absoluta: 0x44a00004	
	dma_ctlr->cdmasr &= ~DMA_CRMSK_IRQERR;
}

void InterruptHandler(void *CallbackRef) //// Documentación: ./documents/pg099-axi-intc.pdf
{
	for (int irq = 0; irq < 32; irq++) {
		//// Ingresa al Interrupt Status Register y hace un barrido a lo largo del registro
		//// para saber si hubo una interrupción en algún periférico. Interrupt Status Register (ISR),
		//// página 15-16. Dirección absoluta: 0x41200000
		if (intp_ctlr->isr & (1 << irq)) {
			switch(irq) {
			case INTNUM_ETHERNET:
				ethctlr_handler();
				break;
			case INTNUM_TIMER:
				timer_handler();
				break;
			case INTNUM_DMA:
				dmactlr_handler();
				break;
			default:
				break;
			}
		}
		//// Ingresa al Interrupt Acknowledge Register y limpia la interrupción de cada periférico.
		//// Interrupt Acknowledge Register (IAR), página 19. Dirección absoluta: 0x4120000C
		intp_ctlr->iar = (1 << irq);
	}
}

void setup_intctlr(void);
void setup_ethctlr(void);
void eth_tx(size_t);
void dmacpy(void volatile *, const void volatile *, size_t);

char buffer[2048];

enum states { 
	idle, 
	rxpkt_mii,
	rxpkt_waitdma,
	rxpkt_dmacpied,
	txpkt_waitdma,
	txpkt_dmacpied };

enum states state = idle;

int main(void)
{
	xil_printf("\r\n\r\n");
	xil_printf("/* pico ping server */\r\n");
	xil_printf("/* **** **** ****** */\r\n");

	setup_intctlr();
	setup_ethctlr();

	for(;;) {
		unsigned chksum;
		unsigned length;
		unsigned tx_length;

		switch(state) {
		case idle:
			//// Ingresa al Receive Control Register (Ping) y chequa el valor del estado para saber si hay
			//// un paquete para recibir. Receive Control Register (0x17FC), página 21.
			//// Registro absoluto: 0x40E017FC
			if (eth_state & (1 << RX_PKT) || (eth_ctlr->rx_ping_control & (1<< 0))) {
				eth_state &= ~(1 << RX_PKT);
				//// Ingresa al Receive Control Register (Ping) e indica que el buffer está vacío.
				//// Receive Control Register (0x17FC), página 21.
				//// Registro absoluto: 0x40E017FC
				eth_ctlr->rx_ping_control &= ~(1<< 0);
				if (loglevel & STATE_LOG) xil_printf("rxpkt_mii\r\n");
				state = rxpkt_mii;
			} 
			break;
		case rxpkt_mii:
			dmacpy(buffer, (void *) eth_ctlr->rx_ping_data, sizeof(eth_ctlr->rx_ping_data));
			if (loglevel & STATE_LOG) xil_printf("rxpkt_waitdma\r\n");
			state = rxpkt_waitdma;
			break;
		case rxpkt_waitdma:
			switch(dma_state) {
			case dma_busy:
				state = rxpkt_waitdma;
				break;
			case dma_ready:
				if (loglevel & STATE_LOG) xil_printf("rxpkt_dmacpied\r\n");
				state = rxpkt_dmacpied;
				break;
			};
			break;
		case rxpkt_dmacpied:
			switch (
				(((struct eth_hdr *) buffer)->type[0] << 8) |
				(((struct eth_hdr *) buffer)->type[1] << 0)) {
			case ETHTYP_ARP:
				if ((loglevel & SERVER_LOG) > 0) {
					xil_printf("GRATUITOUS ARP reply to : MAC => ");
					print_mac(((struct arp_frm *) buffer)->eth_hdr.src);
					xil_printf(" IP => ");
					print_ipv4(((struct arp_frm *) buffer)->arp4_hdr.tpa);
					xil_printf("\r\n");
				}
				if (!memcmp(my_ipv4, ((struct arp_frm *) buffer)->arp4_hdr.tpa, sizeof(my_ipv4))) {
					tx_length = sizeof(my_arp);
					dmacpy(eth_ctlr->tx_ping_data, &my_arp, sizeof(int)*((tx_length+sizeof(int)-1)/sizeof(int)));
					if (loglevel & STATE_LOG) xil_printf("txpkg_waitdma\r\n");
					state = txpkt_waitdma;
				} else {
					if (loglevel & STATE_LOG) xil_printf("idle\r\n");
					state = idle;
				}
				break;
			case ETHTYP_IP:

				if (!memcmp(my_ipv4, ((struct icmp_frm *) buffer)->ipv4_hdr.da, sizeof(my_ipv4)) &&
					((struct icmp_frm *) buffer)->ipv4_hdr.proto[0] == icmp_proto) {

					chksum = 0;
					chksum |= ((struct icmp_frm *) buffer)->icmp_hdr.chksum[0];
					chksum <<= 8;
					chksum |= ((struct icmp_frm *) buffer)->icmp_hdr.chksum[1];

					chksum += 0x0800;
					if (chksum > 0xffff) chksum++;

					((struct icmp_frm *) buffer)->icmp_hdr.chksum[1] = (chksum & 0xff);
					chksum >>= 8;
					((struct icmp_frm *) buffer)->icmp_hdr.chksum[0] = (chksum & 0xff);

					((struct icmp_frm *) buffer)->icmp_hdr.code[0] = 0;

					length = 0;
					length |= ((struct icmp_frm *) buffer)->ipv4_hdr.len[0];
				   	length <<= 8;
					length |= ((struct icmp_frm *) buffer)->ipv4_hdr.len[1];
					length += sizeof(struct eth_hdr);

					for (int i = 0; i < 6; i++) {
						char unsigned aux;

						aux = ((struct icmp_frm *) buffer)->eth_hdr.src[i];
						((struct icmp_frm *) buffer)->eth_hdr.src[i]  = ((struct icmp_frm *) buffer)->eth_hdr.dest[i];
						((struct icmp_frm *) buffer)->eth_hdr.dest[i] = aux;
					}

					for (int i = 0; i < 4; i++) {
						char unsigned aux;

						aux = ((struct icmp_frm *) buffer)->ipv4_hdr.sa[i];
						((struct icmp_frm *) buffer)->ipv4_hdr.sa[i]  = ((struct icmp_frm *) buffer)->ipv4_hdr.da[i];
						((struct icmp_frm *) buffer)->ipv4_hdr.da[i] = aux;
					}
					((struct icmp_frm *) buffer)->icmp_hdr.type[0] = 0x00;

					if ((loglevel & SERVER_LOG) > 0) {
						xil_printf("ICMP reply to : MAC => ");
						print_mac(((struct icmp_frm *) buffer)->eth_hdr.dest);
						xil_printf(" IP => ");
						print_ipv4(((struct icmp_frm *) buffer)->ipv4_hdr.da);
						xil_printf("\r\n");
					}

					tx_length = length;
					dmacpy(eth_ctlr->tx_ping_data, buffer, sizeof(int)*((tx_length+sizeof(int)-1)/sizeof(int)));
					if (loglevel & STATE_LOG) xil_printf("txpkg_waitdma\r\n");
					state = txpkt_waitdma;
				} else {
					if (loglevel & STATE_LOG) xil_printf("idle\r\n");
					state = idle;
				}
				break;
			default:
				if (loglevel & STATE_LOG) xil_printf("idle\r\n");
				state = idle;
				break;
			};
			break;
		case txpkt_waitdma:
			switch(dma_state) {
			case dma_busy:
				state = txpkt_waitdma;
				break;
			case dma_ready:
				if (loglevel & STATE_LOG) xil_printf("txpkt_dmacpied\r\n");
				eth_tx(tx_length);
				state = txpkt_dmacpied;
				break;
			};
			break;
		case txpkt_dmacpied:
			if (loglevel & STATE_LOG) xil_printf("idle\r\n");
			state = idle;
			break;
		default:
			state = idle;
			break;
		}
	}


}

void setup_ethctlr() //// Documentación: ./documents/pg135-axi-ethernetlite.pdf
{
	//// En las siguientes dos líneas, se configura una nueva MAC Address, cargando un 0x0 y un 0x4, a los
	//// 4 bits más significativos y a los 2 bits menos significativos, respectivamente.
	//// Página 19 y 29. Dirección absoluta: 0x40e007FC
	eth_ctlr->tx_ping_data[0] = *(((int *) my_mac)+0);
	eth_ctlr->tx_ping_data[1] = *(((int *) my_mac)+1);

	//// Se realiza operación lógica OR entre dos números. El resultado de esta operación,
	//// Realiza una máscara, a modo de obtener los bits de intenerés del Transmit Control Register (Ping).
	//// Página 19. Dirección absoluta: 0x40e007FC
	eth_ctlr->tx_ping_control |= (0x2 | 0x1);

	//// Se realiza una operación lógica AND entre el resultado de la operación OR y el contenido de
	//// Transmit Control Register (Ping). El resultado puede ser true o false.
	//// Página 19. Dirección absoluta: 0x40e007FC
	while(eth_ctlr->tx_ping_control & (0x2 | 0x1));

	//// Cuando la condición del while de false, en Global Interrupt Enable Register (GIE) se setea
	//// el bit 31, para habilitar las interrupciones globales. Global Interrupt Enable Register (0x07F8),
	//// página 18. Dirección absoluta: 0x40e007F8
	eth_ctlr->gie |= (1 << 31); // Global Interrupt Enable

	//// 
	eth_ctlr->rx_ping_control |= ((1<< 3));
}

void eth_tx(size_t len) //// Documentación: ./documents/pg135-axi-ethernetlite.pdf
{
	//// Ingresa al Transmit Control Register (Ping) y obtiene mediante una máscara los valores de bits
	//// Status (bit 0) y Program (bit 1). Mientras sean iguales a 0x11, no sale de la función.
	//// Transmit Control Register (0x07FC), página 19. Dirección absoluta: 0x40e007FC
	while(eth_ctlr->tx_ping_control & (0x2 | 0x1));

	eth_ctlr->tx_ping_length  = len;

	//// Ingresa al Transmit Control Register (Ping) y aplica una máscara para obtener valores de
	//// bits Status (bit 0) e Interrupt Enable (bit 3). Transmit Control Register (0x07FC), página 19.
	//// Dirección absoluta: 0x40e007FC				
	eth_ctlr->tx_ping_control |= ((1<< 3) | (1 << 0));

	if (loglevel & ETH_LOG) xil_printf("eth_ctlr->tx_ping_control 0x%08x\n\r", eth_ctlr->tx_ping_control);
	if (loglevel & ETH_LOG) xil_printf("eth_ctlr->tx_ping_length  0x%08x\n\r", eth_ctlr->tx_ping_length);
}

void setup_intctlr(void) //// Documentación: ./documents/pg099-axi-intc.pdf
{

	if (loglevel & INTC_LOG) xil_printf("Init Interrupt Controller\r\n");
	if (loglevel & INTC_LOG) xil_printf("*************************\r\n");

	//// Deshabilita las interrupciones en Master Enable Register (MER).
	//// Página 23. Dirección absoluta 0x4120001C
	intp_ctlr->mer = 0;
	//// Deshabilita las interrupciones en Interrupt Enable Register (IER).
	//// Página 18. Dirección absoluta 0x41200008
	intp_ctlr->ier = 0;
	//// Activa las interrupciones en Interrupt Acknowledge Register (IAR).
	//// Página 19. Dirección absoluta 0x4120000C
	intp_ctlr->iar = 0xffffffff;
	//// Activa las interrupciones en Interrupt Mode Register (IMR).
	//// Página 19. Dirección absoluta 0x41200020							
	intp_ctlr->imr = 0;

	// Interrupt Vector Register Reset for Fast Interrupt
	// for (int i = 0; i < 32 ; i++) {
	//  intp_ctlr->ivar[i] = Xil_ExceptionHandler InterruptHandler;
	// }
	
	// Initialize the exception table and register the interrupt
	// controller handler with the exception table

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) InterruptHandler, (void *) 0);
	Xil_ExceptionEnable();
	//// Asignación de interrupciones de DMA y Ethernet en Interrupt Enable Register (IER).
	//// Página 18. Dirección absoluta 0x41200008
	intp_ctlr->ier |= IE_MASK;
	//// Muestra interrupciones activas de Interrupt Enable Register (IER) por consola.
	//// Página 18. Dirección absoluta 0x41200008
	if (loglevel & INTC_LOG) xil_printf("intp_ctlr->ier 0x%08x\n\r", intp_ctlr->ier);
	//// Habilita interrupciones en los bits ME (bit 0) y HIE (bit 1) en Master Enable Register (MER).
	//// Página 23. Dirección absoluta 0x4120001C
	intp_ctlr->mer |= (0x2 | 0x01);
}

void dmacpy(void volatile *dst, const void volatile *src, size_t len) //// Documentación: ./documents/pg034-axi-cdma.pdf
{
	if (loglevel & DMA_LOG) xil_printf("set DMA Controller\r\n");

	dma_state = dma_busy;

	//// Ingresa al CDMASR Register para chequear el bit 1 (Idle). Mientras que sean no
	//// sale de la función. CDMASR (CDMA Status – Offset 04h).
	//// Página 22, dirección absoluta: 0x44a00004
	while(!(dma_ctlr->cdmasr & DMA_SRMSK_IDLE));

	dma_ctlr->cdmacr |=  DMA_CRMSK_IRQEN;  // DMA Ctlr's Enable Interrupt
																					
	dma_ctlr->cdmacr &= ~DMA_CRMSK_IRQERR; // DMA Ctlr's Disable Error Interrupt

	dma_ctlr->sa  = (int) src;			// Set DMA Source Address
																					
	dma_ctlr->da  = (int) dst;			// Set DMA Destiantion Address
																					
	dma_ctlr->btt = len; 				// Set DMA Byte Transfer
}