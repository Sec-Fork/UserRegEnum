#pragma once
#ifndef ENUMERATE_USER_KEYS_H
#define ENUMERATE_USER_KEYS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <Aclapi.h>
#include <sddl.h>

std::wstring GetComputerName(const std::wstring& domainValue, const std::wstring& fullyQualifiedDomainName);
bool EnumerateUserKeys(HKEY hKeyUsers, const std::wstring& domainValue, const std::wstring& dcValue, std::wofstream& outputValue, const std::wstring& fullyQualifiedDomainName);

#endif // ENUMERATE_USER_KEYS_H
