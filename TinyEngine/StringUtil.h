#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <string>
#include <vector>
using namespace std;

class StringUtil
{
public:
	static string format(string format, int x);
	static vector<string> split(string s, char c);
	static string getKeyInArray(string arrayName, int index);
	static string getKeyInArray(string arrayName, string key, int index);
private:
	StringUtil();
};

#endif // !STRING_UTIL_H