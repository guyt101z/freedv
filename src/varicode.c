//==========================================================================
// Name:            varicode.h
// Purpose:         Varicode encoded and decode functions
// Created:         Nov 24, 2012
// Authors:         David Rowe
//
// To test:
//          $ gcc varicode.c -o varicode -DVARICODE_UNITTEST -Wall
//          $ ./varicode
// 
// License:
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "varicode.h"
#include "varicode_table.h"

#define VARICODE_MAX_BITS (10+2) /* 10 bits for code plus 2 0 bits for inter-character space */

/*
  output is an unpacked array of bits of maximum size max_out.  Note
  unpacked arrays are a more suitable form for modulator input.
*/

int varicode_encode(int varicode_out[], char ascii_in[], int max_out, int n_in) {
    int            n_out, index, n_zeros, v_len;
    unsigned short byte1, byte2, packed;

    n_out = 0;

    while(n_in && (n_out < max_out)) {

        assert((unsigned int)(*ascii_in) < 128);

        index = 2*(unsigned int)(*ascii_in);
        byte1 = varicode_table[index];
        byte2 = varicode_table[index+1];
        packed = (byte1 << 8) + byte2;
        //printf("n_in: %d ascii_in: %c index: %d packed 0x%x\n", n_in, *ascii_in, index, packed);
        ascii_in++;

        n_zeros = 0;
        v_len = 0;
        while ((n_zeros < 2) && (n_out < max_out) && (v_len <= VARICODE_MAX_BITS)) {
            if (packed & 0x8000) {
                *varicode_out = 1;
                n_zeros = 0;
            }
            else {
                *varicode_out = 0;
                n_zeros++;
            }
            //printf("packed: 0x%x *varicode_out: %d n_zeros: %d v_len: %d\n", packed, *varicode_out, n_zeros,v_len );
            packed <<= 1;
            varicode_out++;

            n_out++;
            v_len++;
        }
        assert(v_len <= VARICODE_MAX_BITS);

        n_in--;
            
    }

    return n_out;
}

int varicode_decode(char ascii_out[], int varicode_in[], int max_out, int n_in) {
    int            i, n_out, n_zeros, found, v_len;
    unsigned short byte1, byte2, packed;

    n_out = 0;

    //printf("max_out: %d n_in: %d\n", max_out, n_in);
    while(n_in && (n_out < max_out)) {
        
        n_zeros = 0;
        v_len = 0;
        packed = 0;
        while ((n_zeros < 2) && n_in && (v_len <= VARICODE_MAX_BITS)) {
            if (*varicode_in++) {
                packed |= (0x8000 >> v_len);
                n_zeros = 0;
            }
            else {
                n_zeros++;
            }
            n_in--;
            v_len++;
        }
        
        //printf("packed: 0x%x n_zeros: %d v_len: %d n_in: %d\n", packed, n_zeros, v_len, n_in);
      
        /* v_len might be > VARICODE_MAX_BITS is an error condition */

        if (v_len && (v_len <= VARICODE_MAX_BITS)) {
            byte1 = packed >> 8;
            byte2 = packed & 0xff;

            found = 0;
            for(i=0; i<128; i++) {
                if ((byte1 == varicode_table[2*i]) && (byte2 == varicode_table[2*i+1])) {
                    found = 1;
                    //printf("%c\n", (char)i);
                    *ascii_out++ = i;
                }
            }
            if (found == 1)
                n_out++;
        }
        //printf("n_out: %d n_in: %d\n", n_out, n_in);
    }

    return n_out;
}

#ifdef VARICODE_UNITTEST
int main(void) {
    char *ascii_in;
    int *varicode;
    int  i, n_varicode_bits_out, n_ascii_chars_out;
    char *ascii_out;

    ascii_in = (char*)malloc(sizeof(varicode_table)/2);
    varicode = (int*)malloc(VARICODE_MAX_BITS*sizeof(int)*strlen(ascii_in));
    ascii_out = (char*)malloc(strlen(ascii_in)+1);
    
    for(i=0; i<sizeof(varicode_table)/2; i++)
        ascii_in[i] = (char)i;
    n_varicode_bits_out = varicode_encode(varicode, ascii_in, VARICODE_MAX_BITS*strlen(ascii_in), strlen(ascii_in));
    n_ascii_chars_out = varicode_decode(ascii_out, varicode, strlen(ascii_in), n_varicode_bits_out);
    assert(n_ascii_chars_out == strlen(ascii_in));
    ascii_out[n_ascii_chars_out] = 0;

    //for(i=0; i<n_varicode_bits_out; i++) {
    //    printf("%d \n", varicode[i]);
    //}

    //printf("ascii_out: %s\n", ascii_out);
    if (strcmp(ascii_in, ascii_out) == 0)
        printf("Pass\n");
    else
        printf("Fail\n");

    free(ascii_in);
    free(ascii_out);
    free(varicode);
    return 0;
}
#endif
