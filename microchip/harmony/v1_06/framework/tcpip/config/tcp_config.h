/*******************************************************************************
  Transmission Control Protocol (TCP) Configuration file

  Company:
    Microchip Technology Inc.
    
  File Name:
    tcp_config.h

  Summary:
    TCP configuration file
    
  Description:
    This file contains the TCP module configuration options
    
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright � 2011 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END
#ifndef _TCP_CONFIG_H_
#define _TCP_CONFIG_H_


// TCP Maximum Segment Size for TX.  The TX maximum segment size is actually 
// governed by the remote node's MSS option advertised during connection 
// establishment.  However, if the remote node specifies an unmanageably large 
// MSS (ex: > Ethernet MTU), this define sets a hard limit so that TX buffers 
// are not overflowed.  If the remote node does not advertise a MSS 
// option, all TX segments are fixed at 536 bytes maximum.
#define TCPIP_TCP_MAX_SEG_SIZE_TX			(1460u)

// TCP Maximum Segment Size for RX (MSS).
// This value is advertised during TCP connection establishment
// and the remote node should obey it.
// The value has to be set in such a way to avoid IP layer fragmentation
// from causing packet loss.
// However, raising its value can enhance performance at the (small) risk of introducing 
// incompatibility with certain special remote nodes (ex: ones connected via a 
// slow dial up modem).
// On Ethernet networks the standard value is 1460.
// On dial-up links, etc. the default values should be 536.
// Adjust these values according to your network.
//
// Maximum Segment Size for RX (MSS) for local destination networks.
#define TCPIP_TCP_MAX_SEG_SIZE_RX_LOCAL   			(1460)

// Maximum Segment Size for RX (MSS) for non local destination networks.
#define TCPIP_TCP_MAX_SEG_SIZE_RX_NON_LOCAL           (536) 

// Default socket TX buffer size
// Note that this setting affects all TCP sockets that are created
// and, together with TCPIP_TCP_MAX_SOCKETS, has a great impact on
// the heap size that's used by the stack (see TCPIP_STACK_DRAM_SIZE setting).
// When large TX bufferrs are needed, probably a dynamic, per socket approach,
// is a better choice (see TCPIP_TCP_OptionsSet function).
// The performance of a socket is highly dependent on the size of its buffers
// so it's a good idea to use as large as possible buffers for the sockets that need
// Note that some modules (like HTTP) use their own settings to specify the buffer size
// for their TCP sockets.
#define TCPIP_TCP_SOCKET_DEFAULT_TX_SIZE		512			

// Default socket RX buffer size
// Note that this setting affects all TCP sockets that are created
// and, together with TCPIP_TCP_MAX_SOCKETS, has a great impact on
// the heap size that's used by the stack (see TCPIP_STACK_DRAM_SIZE setting).
// When large RX bufferrs are needed, probably a dynamic, per socket approach,
// is a better choice (see TCPIP_TCP_OptionsSet function).
// The performance of a socket is highly dependent on the size of its buffers
// so it's a good idea to use as large as possible buffers for the sockets that need
// high throughput. 
// Note that some modules (like HTTP) use their own settings to specify the buffer size
// for their TCP sockets.
#define TCPIP_TCP_SOCKET_DEFAULT_RX_SIZE		512			
		

// Timeout to retransmit unacked data, ms
#define TCPIP_TCP_START_TIMEOUT_VAL   	(1000ul)

// Timeout for delayed-acknowledgment algorithm, ms
#define TCPIP_TCP_DELAYED_ACK_TIMEOUT		(100ul)

// Timeout for FIN WAIT 2 state, ms
#define TCPIP_TCP_FIN_WAIT_2_TIMEOUT		(5000ul)

// Timeout for keep-alive messages when no traffic is sent, ms
#define TCPIP_TCP_KEEP_ALIVE_TIMEOUT		(10000ul)

// Timeout for the CLOSE_WAIT state, ms
#define TCPIP_TCP_CLOSE_WAIT_TIMEOUT		(200ul)

// Maximum number of retransmission attempts
#define TCPIP_TCP_MAX_RETRIES			(5u)

// Maximum number of keep-alive messages that can be sent 
// without receiving a response before automatically closing 
// the connection
#define TCPIP_TCP_MAX_UNACKED_KEEP_ALIVES	(6u)

// Smaller than all other retries to reduce SYN flood DoS duration
#define TCPIP_TCP_MAX_SYN_RETRIES		(2u)

// Timeout before automatically transmitting unflushed data, ms.
// Default value is 40 ms.
#define TCPIP_TCP_AUTO_TRANSMIT_TIMEOUT_VAL	(40ul)

// Timeout before automatically transmitting a window update
// due to a TCPIP_TCP_Get() or TCPIP_TCP_ArrayGet() function call, ms.
#define TCPIP_TCP_WINDOW_UPDATE_TIMEOUT_VAL	(200ul)

//	The maximum number of sockets to create in the stack.
//	When defining TCPIP_TCP_MAX_SOCKETS take into account
//	the number of interfaces the stack is supporting.
#define TCPIP_TCP_MAX_SOCKETS 			(10)



// The TCP task processing rate: number of milliseconds to generate an TCP tick.
// This is the tick that advances the TCP state machine.
// Note: the System Tick resolution has to
// be fine enough to allow for this TCP tick granularity.  
#define TCPIP_TCP_TASK_TICK_RATE        (5)



#endif  // _TCP_CONFIG_H_
