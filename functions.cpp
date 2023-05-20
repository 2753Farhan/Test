#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <curl/curl.h>
#include <sstream>

size_t WriteCallback(char *ptr, size_t size, size_t nmemb, std::string *userdata)
{
    size_t len = size * nmemb;
    userdata->append(ptr, len);
    return len;
}

std::vector<int> compute_prefix(const std::string& pattern)
{
    int n = pattern.size();
    std::vector<int> prefix(n);
    prefix[0] = 0;

    int j = 0;
    for (int i = 1; i < n; i++)
    {
        while (j > 0 && pattern[j] != pattern[i])
        {
            j = prefix[j-1];
        }
        if (pattern[j] == pattern[i])
        {
            j++;
        }
        prefix[i] = j;
    }

    return prefix;
}

// Search for the pattern within the text using the KMP algorithm
size_t Find(const std::string& text, const std::string& pattern)
{
    int n = text.size();
    int m = pattern.size();

    // Compute the prefix function for the pattern
    std::vector<int> prefix = compute_prefix(pattern);

    int j = 0;
    for (int i = 0; i < n; i++)
    {
        while (j > 0 && pattern[j] != text[i])
        {
            j = prefix[j-1];
        }
        if (pattern[j] == text[i])
        {
            j++;
        }
        if (j == m)
        {
            // Found a match at position i-m+1
            return i-m+1;
        }
    }

    // Pattern not found
    return std::string::npos;
}


