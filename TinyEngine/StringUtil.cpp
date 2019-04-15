#include "StringUtil.h"

string StringUtil::format(string format, int x)
{
	char s[64];
	sprintf_s(s, format.c_str(), x);
	return std::string(s);
}

vector<string> StringUtil::split(string s, char c)
{
	vector<string> res;

	int length = s.length();

	int st = 0;
	int ed;
	while (st < length && s[st] == c)
	{
		st++;
	}

	while (st < length)
	{
		ed = st + 1;
		while (ed < s.length() && s[ed] != ',')
		{
			ed++;
		}

		res.push_back(s.substr(st, ed - st));

		st = ed + 1;
		while (st < length && s[st] == c)
		{
			st++;
		}
	}

	return res;
}

string StringUtil::getKeyInArray(string arrayName, int index)
{
	char s[64];
	sprintf_s(s, "%s[%d]", arrayName.c_str(), index);
	return std::string(s);
}

string StringUtil::getKeyInArray(string arrayName, string key, int index)
{
	char s[64];
	sprintf_s(s, "%s[%d].%s", arrayName.c_str(), index, key.c_str());
	return std::string(s);
}

StringUtil::StringUtil() {}
