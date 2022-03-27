/*
 * circBufT.h
 *
 * Header File for circular buffer
 *
 * Created on: 9/03/2020
 *     Author: Jia Shen Ui (27674749)
 *             Xin Tong Ow Yong (41511780)
 *             Azmelia Raihana Mohd Halmi (73414184)
 *
 *     Group Name: FitnessMonGroup14
 */


// *******************************************************
// 
// circBufT.h
//
// Support for a circular buffer of uint32_t values on the 
//  Tiva processor.
// P.J. Bones UCECE
// Last modified:  7.3.2017
// 
// *******************************************************



#include <stdint.h>
#ifndef CIRCBUFT_H_
#define CIRCBUFT_H_

// Buffer structure
typedef struct {
	uint32_t size;		// Number of entries in buffer
	uint32_t windex;	// index for writing, mod(size)
	uint32_t rindex;	// index for reading, mod(size)
	uint32_t *data;		// pointer to the data
} circBuf_t;

// *******************************************************
// initCircBuf: Initialise the circBuf instance. Reset both indices to
// the start of the buffer.  Dynamically allocate and clear the the 
// memory and return a pointer for the data.  Return NULL if 
// allocation fails.


uint32_t *
initCircBuf (circBuf_t *buffer, uint32_t size);

// *******************************************************
// writeCircBuf: insert entry at the current windex location,
// advance windex, modulo (buffer size).
void
writeCircBuf (circBuf_t *buffer, uint32_t entry);

// *******************************************************
// readCircBuf: return entry at the current rindex location,
// advance rindex, modulo (buffer size). The function deos not check
// if reading has advanced ahead of writing.
uint32_t
readCircBuf (circBuf_t *buffer);

// *******************************************************
// freeCircBuf: Releases the memory allocated to the buffer data,
// sets pointer to NULL and other fields to 0. The buffer can
// re initialised by another call to initCircBuf().
void
freeCircBuf (circBuf_t *buffer);

// *******************************************************

typedef struct myCircBuffers{
    circBuf_t x_circBuff;
    circBuf_t y_circBuff;
    circBuf_t z_circBuff;
}circBuffers_t;

circBuffers_t acc_buffers; //buffers for acceleration in 3 axis

#define BUF_SIZE 10
#define BUFFER_DELAY 10
#define REF_DELAY 5

#endif /*CIRCBUFT_H_*/
