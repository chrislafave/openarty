////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	gpsdump.c
//
// Project:	OpenArty, an entirely open SoC based upon the Arty platform
//
// Purpose:	To dump the GPS UART to the auxiliary UART.
//
// Creator:	Dan Gisselquist, Ph.D.
//		Gisselquist Technology, LLC
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015-2016, Gisselquist Technology, LLC
//
// This program is free software (firmware): you can redistribute it and/or
// modify it under the terms of  the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program.  (It's in the $(ROOT)/doc directory, run make with no
// target there if the PDF file isn't present.)  If not, see
// <http://www.gnu.org/licenses/> for a copy.
//
// License:	GPL, v3, as defined and found on www.gnu.org,
//		http://www.gnu.org/licenses/gpl.html
//
//
////////////////////////////////////////////////////////////////////////////////
//
//
#include "zipcpu.h"
#include "zipsys.h"
#include "artyboard.h"

void main(int argc, char **argv) {
	/*
	// Method one: direct polling
	while(1) {
		int	ch;
		ch = sys->io_gps_rx;
		if ((ch&UART_RX_ERR)==0)
			sys->io_uart_tx = ch;
	}
	*/

	// Method two: Waiting on interrupts
	zip->z_pic = SYSINT_GPSRX;
	while(1) {
		while((zip->z_pic & SYSINT_GPSRX)==0)
			;
		sys->io_uart_tx = sys->io_gps_rx;
		zip->z_pic = SYSINT_GPSRX;
	}

	/*
	// Method three: Use the DMA
	zip->z_dma.d_ctrl = DMACLEAR;
	while(1) {
		zip->z_dma.d_rd = (int *)&sys->io_gps_rx;
		zip->z_dma.d_wr = (int *)&sys->io_uart_tx;
		zip->z_dma.d_len = 0x01000000; // More than we'll ever do ...
		zip->z_dma.d_ctrl = (DMAONEATATIME|DMA_CONSTDST|DMA_CONSTSRC|DMA_ONGPSRX);

		while(zip->z_dma.d_ctrl & DMA_BUSY) {
			zip_idle();
			if (zip->z_dma.d_ctrl & DMA_ERR)
				zip_halt();
		}
	}
	*/
}
