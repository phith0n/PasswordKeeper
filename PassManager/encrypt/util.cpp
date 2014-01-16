#include "stdafx.h"
#include "util.h"
#include <vector>

using namespace std;

char intToHexChar(int x) {
	static const char HEX[16] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'A', 'B',
		'C', 'D', 'E', 'F'
	};
	return HEX[x];
}

string stringToHexString(string src)
{
	string dest("");
	for (int i = 0 ; i < src.length() ; i++)
	{
		char c = src.at(i);
		char d[4] = {'\0'};
		sprintf(d, "%X", c);
		dest += d;
	}
	return dest;
}

int hexCharToInt(char hex) {
	hex = toupper(hex);
	if (isdigit(hex))
		return (hex - '0');
	if (isupper(hex)){
		return (hex - 'A' + 10);
	}
	if (islower(hex))
	{
		return (hex - 'a' + 10);
	}
	return 0;
}

string bytesToHexString(const byte *in, size_t size) {
	string str;
	for (size_t i = 0; i < size; ++i) {
		int t = in[i];
		int a = t / 16;
		int b = t % 16;
		str.append(1, intToHexChar(a));
		str.append(1, intToHexChar(b));
		//if (i != size - 1)
		//	str.append(1, ' ');
	}
	return str;
}

string byteToString(const byte * in, size_t size)
{
	string out("");
	for (int i = 0 ; i < size ; i++)
	{
		out += in[i];
	}
	return out;
}

size_t hexStringToBytes(const string &str, byte *out) {

	vector<string> vec;
	string::size_type currPos = 2, prevPos = 0;
	while(currPos < str.length()){
		string b(str.substr(prevPos, currPos - prevPos));
		vec.push_back(b);
		prevPos = currPos;
		currPos += 2;
	}
	if (prevPos < str.length()) {
		string b(str.substr(prevPos));
		vec.push_back(b);
	}
	typedef vector<string>::size_type sz_type;
	sz_type size = vec.size();
	for (sz_type i = 0; i < size; ++i) {
		int a = hexCharToInt(vec[i][0]);
		int b = hexCharToInt(vec[i][1]);
		out[i] = a * 16 + b;
	}
	return size;
}

size_t stringToBytes(const string src, byte * dest)
{
	size_t i;
	for (i = 0 ; i < src.length() ; i++)
	{
		char c = src.at(i);
		dest[i] = c;
	}
	return i;
}