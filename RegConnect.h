#pragma once
#ifndef REGCONNECT_HPP
#define REGCONNECT_HPP

#include <vector>
#include <string>
#include <future>
#include <mutex>
#include <fstream>
#include <Windows.h>

extern int MAX_THREADS;

void checkRegistryConnection(const std::wstring& domainValue, const std::wstring& dcValue, std::wofstream& outputValue, const std::vector<std::wstring>& computerNames, int start, int end, std::mutex& mutex);
void RegConnect(const std::wstring& dcValue, const std::wstring& domainValue, std::wofstream& outputValue, int numThreads);

#endif // REGCONNECT_HPP
