#include "SearchDnsHostname.h"
#include "RegEnumKey.h"
#include "RegConnect.h"

int MAX_THREADS = 1000;

// ���ע�������
void checkRegistryConnection(const std::wstring& domainValue, const std::wstring& dcValue, std::wofstream& outputValue, const std::vector<std::wstring>& computerNames, int start, int end, std::mutex& mutex) {
    for (int i = start; i < end; i++) {
        // ��������ע���
        HKEY hKeyUsers;
        LONG result = RegConnectRegistryW(computerNames[i].c_str(), HKEY_USERS, &hKeyUsers);
        if (result == ERROR_SUCCESS) {
            // ������ӳɹ�, ��ö���û���, ���ڻ������ı�����������
            std::lock_guard<std::mutex> lock(mutex);
            EnumerateUserKeys(hKeyUsers, domainValue, dcValue, outputValue, computerNames[i].c_str());
            RegCloseKey(hKeyUsers);
        }
    }
}

void RegConnect(const std::wstring& domainValue, const std::wstring& dcValue, std::wofstream& outputValue, int numThreads) {
    
    // ���ݴ���� numThreads �޸� MAX_THREADS ��ֵ
    if (numThreads > 0) {
        MAX_THREADS = numThreads;
        //std::cout << "numThreads: " << MAX_THREADS << std::endl;  // �������
    }

    // ���ļ�����ȡ�������
    std::vector<std::wstring> computerNames;
    std::wstring computerName;
    std::wistringstream iss(SearchDnsHostname(domainValue, dcValue));

    while (iss >> computerName) {
        computerNames.emplace_back(computerName);
    }

    std::mutex mutex;
    std::vector<std::future<void>> futures;

    // ����������б�ָ�����ɸ���������ÿ����������Ϊһ������
    int n = computerNames.size();
    int batchSize = n / MAX_THREADS + 1;
    //std::cout << MAX_THREADS << std::endl;  // �������
    //std::cout << batchSize << std::endl;  // �������
    for (int i = 0; i < n; i += batchSize) {
        int end = (i + batchSize < n) ? i + batchSize : n;

        // ���̳߳��������첽����
        futures.emplace_back(std::async(std::launch::async, checkRegistryConnection, std::cref(domainValue), std::cref(dcValue), std::ref(outputValue), std::cref(computerNames), i, end, std::ref(mutex)));
    }

    // �ȴ������������
    for (auto& future : futures) {
        future.wait();
    }
}
