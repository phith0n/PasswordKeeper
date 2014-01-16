#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <cstdlib>

typedef unsigned char byte;
typedef unsigned long ulong;

/*
*convert int to hex char.
*example:10 -> 'A',15 -> 'F'
*/
char intToHexChar(int x);

/*
*convert hex char to int.
*example:'A' -> 10,'F' -> 15
*/
int hexCharToInt(char hex);

using std::string;
/*
*convert a byte array to hex string.
*hex string format example:"AFB0807D"
*/
string bytesToHexString(const byte *in, size_t size);

/*
*convert a hex string to a byte array.
*hex string format example:"AFB0807D"
*/
size_t hexStringToBytes(const string &str, byte *out);

/*
*convert a string to a hex string.
*string like "abc" 
*hex string format example:"616263"
*/
string stringToHexString(string src);

/*
*convert a string to a byte array
*/
size_t stringToBytes(const string src, byte * dest);

/*
*convert a bytes array to a string
*/
string byteToString(const byte * in, size_t size);

#endif/*UTIL_H*/