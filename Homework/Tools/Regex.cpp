#include "Regex.hpp"

RegexMatcher::RegexMatcher(const std::string& regex)
{
	regex_ = std::regex(regex);
}

bool RegexMatcher::isMatch(const std::string& str) const
{
	return std::regex_search(str, regex_);
}

bool RegexMatcher::isMatch(const wchar_t* str) const
{
	uint64_t size = wcstombs(NULL, str, 0);
	char* strbuf = new char[size + 1];
	wcstombs(strbuf, str, size + 1);

	bool res = isMatch(strbuf);
    delete[] strbuf;

    return res;
}

bool RegexMatcher::isMatch(const char* str) const
{
    return std::regex_search(str, regex_);
}

RegexMatcher* RegexMatcher::of(const wchar_t* str)
{
	uint64_t size = wcstombs(NULL, str, 0);
	char* strbuf = new char[size + 1];
	wcstombs(strbuf, str, size + 1);

	try {
        RegexMatcher* res = new RegexMatcher(strbuf);
		delete[] strbuf;
		return res;
	}
	catch (std::regex_error& e) {
		delete[] strbuf;
		return NULL;
	}
}