#include "stdafx.h"
#include "tea.h"
#include <cstring> //for memcpy,memset
#include "md5.h"

#define random(x) (rand()%x)
#define TIMES 64

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

TEA::TEA(const byte *key, int round /*= 32*/, bool isNetByte /*= false*/)
:_round(round)
,_isNetByte(isNetByte) {
	if (key != 0)
		memcpy(_key, key, 16);
	else
		memset(_key, 0, 16);
}

TEA::TEA(const TEA &rhs)
:_round(rhs._round)
,_isNetByte(rhs._isNetByte) {
	memcpy(_key, rhs._key, 16);
}

TEA& TEA::operator=(const TEA &rhs) {
	if (&rhs != this) {
		_round = rhs._round;
		_isNetByte = rhs._isNetByte;
		memcpy(_key, rhs._key, 16);
	}
	return *this;
}

void TEA::encrypt(const byte *in, byte *out) {
	encrypt((const ulong*)in, (ulong*)out);
}

void TEA::decrypt(const byte *in, byte *out) {
	decrypt((const ulong*)in, (ulong*)out);
}

void TEA::encrypt(const ulong *in, ulong *out) {

	ulong *k = (ulong*)_key;
	register ulong y = this->ntoh(in[0]);
	register ulong z = this->ntoh(in[1]);
	register ulong a = this->ntoh(k[0]);
	register ulong b = this->ntoh(k[1]);
	register ulong c = this->ntoh(k[2]);
	register ulong d = this->ntoh(k[3]);
	register ulong delta = 0x9E3779B9; /* (sqrt(5)-1)/2*2^32 */
	register int round = _round;
	register ulong sum = 0;

	while (round--) {	/* basic cycle start */
		sum += delta;
		y += ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
		z += ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);
	}	/* end cycle */
	out[0] = this->ntoh(y);
	out[1] = this->ntoh(z);
}

ulong TEA::ntoh(ulong netlong) { return _isNetByte ? ntohl(netlong) : netlong; }

ulong TEA::hton(ulong hostlong) { return _isNetByte ? htonl(hostlong) : hostlong; }

void TEA::decrypt(const ulong *in, ulong *out) {

	ulong *k = (ulong*)_key;
	register ulong y = this->ntoh(in[0]);
	register ulong z = this->ntoh(in[1]);
	register ulong a = this->ntoh(k[0]);
	register ulong b = this->ntoh(k[1]);
	register ulong c = this->ntoh(k[2]);
	register ulong d = this->ntoh(k[3]);
	register ulong delta = 0x9E3779B9; /* (sqrt(5)-1)/2*2^32 */
	register int round = _round;
	register ulong sum = 0;

	if (round == 32)
		sum = 0xC6EF3720; /* delta << 5*/
	else if (round == 16)
		sum = 0xE3779B90; /* delta << 4*/
	else
		sum = delta * round;

	while (round--) {	/* basic cycle start */
		z -= ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);
		y -= ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
		sum -= delta;
	}	/* end cycle */
	out[0] = this->ntoh(y);
	out[1] = this->ntoh(z);
}

bool decode(const string src, const string strKey, string& dest)
{
	int rlen = src.at(0) - '0';
	string enc = src.substr(1), strOut = "";
	int len = enc.length(), i;
	const int SIZE_IN = 16, SIZE_OUT = 8, SIZE_KEY = 16;
	byte plain[SIZE_IN] = {0}, crypt[SIZE_OUT] = {0}, key[SIZE_KEY] = {0};

	for (i = 0 ; i < len ; i += SIZE_IN)
	{
		string strNow = enc.substr(i, SIZE_IN);
		size_t size_in = hexStringToBytes(strNow, crypt);
		size_t size_key = hexStringToBytes(strKey, key);

		if (size_in != SIZE_IN / 2 || size_key != SIZE_KEY)
			return false;

		TEA tea(key, TIMES, false);
		tea.decrypt(crypt, plain);
		if (i >= len - SIZE_IN && rlen != 0)
		{
			strOut += byteToString(plain, rlen);
			//strOut += bytesToHexString(plain, rlen);
		}else{
			strOut += byteToString(plain, SIZE_OUT);
			//strOut += bytesToHexString(plain, SIZE_OUT);
		}
	}//
	dest = strOut;
	return true;
}
bool encrypt(const string src, const string strKey, string& dest)
{
	string strCrypt("");
	srand((unsigned)time(0));
	const int SIZE_IN = 8, SIZE_OUT = 8, SIZE_KEY = 16;
	byte plain[SIZE_IN], crypt[SIZE_OUT], key[SIZE_KEY];
	int len = src.length();
	for (size_t i = 0 ; i < len ; i += 8)
	{
		string strNow;
		if (len - i < 8)
		{	
			strNow = src.substr(i);
			int remain = 8 - (len - i);
			for (size_t j = 0 ; j < remain ; j++)
			{
				char c = random(128)&0xff;
				strNow += c;
			}
		}else{
			strNow = src.substr(i, 8);
		}
		size_t size_in = stringToBytes(strNow, plain);
		size_t size_key = hexStringToBytes(strKey, key);

		if (size_in != SIZE_IN || size_key != SIZE_KEY)
			return false;

		TEA tea(key, TIMES, false);
		tea.encrypt(plain, crypt);
		strCrypt += bytesToHexString(crypt, SIZE_OUT);
	}
	char cLen = src.length() % 8 + '0';
	dest = cLen + strCrypt;
	return true;
}