#pragma once
#ifndef REGEX_HPP_
#define REGEX_HPP_
#include <regex>

class RegexMatcher
{
private:
	std::regex regex_;
public:
    RegexMatcher(const std::string& regex);

    bool isMatch(const std::string& str) const;
    bool isMatch(const wchar_t* str) const;
    bool isMatch(const char* str) const;

    static RegexMatcher* of(const wchar_t* str);
};

#endif