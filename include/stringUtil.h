//
// Created by xiang on 2020/11/30.
//

#ifndef SDPTRANS_STRINGUTIL_H
#define SDPTRANS_STRINGUTIL_H



#include <string>
#include <vector>
#include <algorithm>

#include <sstream>

#include <string.h>
#include <stdio.h>


namespace stringUtil
{
    template<class T> T to(const std::string &strNumber)
    {
        std::stringstream ss(strNumber);
        T num = 0;
        ss >> num;
        return num;
    }

    inline char tolower(char c)
    {
        if (c >= 'A' && c <= 'Z')//是大写字母，执行转换。
            c += 'a' - 'A';//转成大写。
        return c;
    }
    inline std::string tolower(std::string str)
    {
        for (unsigned i = 0; i < str.length(); i++)
            str[i] = tolower(str[i]);
        return str;
    }
    inline char toupper(char c)
    {
        if (c >= 'a' && c <= 'z')
            c += 'A' - 'a';
        return c;
    }
    inline std::string toupper(std::string str)
    {
        for (unsigned i = 0; i < str.length(); i++)
            str[i] = toupper(str[i]);
        return str;
    }

    template<class T>
    std::string digitArrayToString(const std::vector<T>& a, const std::string& delim) {
        std::string out;
        for (int i = 0; i < a.size(); ++i) {
            if (i == a.size() - 1)
                out += std::to_string(a[i]) + delim;
            else
                out += std::to_string(a[i]);
        }
        return out;
        //return trim(strings.Join(strings.Split(fmt.Sprint(a), " "), delim), "[]")
    }

    inline bool contains(const std::vector<std::string>& arr, const std::string& item) {
        return std::find(arr.begin(), arr.end(), item) != arr.end();
    }

    inline std::vector<std::string> split(const std::string &strSource, const std::string &chSplitChar)
    {
        std::vector<std::string> ret;
        std::string::size_type last = 0;
        std::string::size_type index = strSource.find(chSplitChar, last);
        while (index != std::string::npos)
        {
            ret.push_back(strSource.substr(last, index - last));
            last = index + chSplitChar.size();
            index = strSource.find(chSplitChar, last);
        }
        if (index - last > 0)
        {
            ret.push_back(strSource.substr(last, index - last));
        }

        return ret;
    }

    inline std::string concat(const std::vector<std::string> &strv, const std::string &c = " ")
    {
        std::string ret;
        std::vector<std::string>::const_iterator cit = strv.begin();

        for (; cit != strv.end(); cit++)
        {
            ret += *cit;
            ret += c;
        }
        if (ret.size() > 0)
            ret.erase(ret.end() - 1);

        return ret;
    }


    inline static std::string trim(const std::string &str, const std::string& c = " ")
    {
        std::string::size_type pos = str.find_first_not_of(c);
        if (pos == std::string::npos)
        {
            return str;
        }
        std::string::size_type pos2 = str.find_last_not_of(c);
        if (pos2 != std::string::npos)
        {
            return str.substr(pos, pos2 - pos + 1);
        }
        return str.substr(pos);
    }

    inline static std::string& trimSelf(std::string& text, const std::string& delim)
    {
        if (!text.empty())
        {
            text.erase(0, text.find_first_not_of(delim));
            text.erase(text.find_last_not_of(delim) + 1);
        }
        return text;
    }

    inline static std::string join(const std::vector<std::string>& ss, const std::string& delim) {
        std::string ret;
        for (int i = 0; i < ss.size(); ++i) {
            if (i != ss.size() - 1) {
                ret += ss[i] + delim;
            }
            else {
                ret += ss[i];
            }
        }
        return ret;
    }

    inline std::string& replace_all(std::string& str, const std::string& old_value, const std::string& new_value)
    {
        while (true)
        {
            size_t pos = 0;
            if ((pos = str.find(old_value, 0)) != std::string::npos)
                str.replace(pos, old_value.length(), new_value);
            else break;
        }
        return str;
    }

    inline const std::string copyBytes(const std::vector<char>& bytes, std::string& str, unsigned	maxLen = 0)
    {
        unsigned len = (maxLen == 0) ? (unsigned)bytes.size() : std::min(maxLen, (unsigned)bytes.size());
        str.resize(len);
        memcpy(&str[0], &bytes[0], len);
        return str;
    }
    inline const std::string copyBytes(const std::vector<char>& bytes, unsigned maxLen = 0)
    {
        std::string tempStr;
        return copyBytes(bytes, tempStr, maxLen);
    }
    inline const void toBytes(const std::string& str, std::vector<char>& bytes)
    {
        bytes.resize(str.size());
        memcpy(&bytes[0], &str[0], str.size());
    }

    inline std::string getRawString(std::string const& s)
    {
        std::stringstream out;
        out << '\"';
        out << std::hex;
        for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
        {
            // AND 0xFF will remove the leading "ff" in the output,
            // So that we could get "\xab" instead of "\xffab"
            out << " " << (static_cast<short>(*it) & 0xff);
        }
        out << '\"';
        return out.str();
    }

    inline std::string toHexString(const char* buf, int len, const std::string& token)
    {
        std::string output;
        output.reserve(len * (2 + token.size()));
        char temp[8];
        for (int i = 0; i < len; ++i)
        {
            snprintf(temp, 6, "%.2x", (uint8_t)buf[i]);
            output.append(temp, 2);
            output.append(token);
        }

        return output;
    }
}

#endif //SDPTRANS_STRINGUTIL_H
