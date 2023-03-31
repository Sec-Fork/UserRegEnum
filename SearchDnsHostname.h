#pragma once
#ifndef SEARCH_DNS_HOSTNAME_H
#define SEARCH_DNS_HOSTNAME_H
#include <string>

std::wstring SearchDnsHostname(const std::wstring& domainValue, const std::wstring& dcValue);

#endif // SEARCH_DNS_HOSTNAME_H