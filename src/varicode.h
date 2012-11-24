//==========================================================================
// Name:            varicode.h
// Purpose:         Varicode encoded and decode functions
// Created:         Nov 24, 2012
// Authors:         David Rowe
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

#ifndef __VARICODE__
#define __VARICODE__

#ifdef __cplusplus
extern "C" {

#endif
int varicode_encode(int varicode_out[], char ascii_in[], int max_out, int n_in);
int varicode_decode(char ascii_out[], int varicode_in[], int max_out, int n_in);

#ifdef __cplusplus
}
#endif

#endif
