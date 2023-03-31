#include "SearchOperatingSystem.h"
#include "RegEnumKey.h"

// ��ȡ computerName, fullyQualifiedDomainName = test.missyou.com -> ���� test
std::wstring GetComputerName(const std::wstring& domainValue, const std::wstring& fullyQualifiedDomainName)
{
    std::wstring domain = domainValue;
    std::wstring::size_type pos = fullyQualifiedDomainName.find(domain);
    if (pos != std::wstring::npos) {
        return fullyQualifiedDomainName.substr(0, pos - 1);
    }
    return fullyQualifiedDomainName;
}

// ��ָ���� Windows ע�����ö���û����Ӽ�
bool EnumerateUserKeys(HKEY hKeyUsers, const std::wstring& domainValue, const std::wstring& dcValue, std::wofstream& outputValue, const std::wstring& fullyQualifiedDomainName)
{
    // ��ȡ���������
    std::wstring computerName = GetComputerName(domainValue, fullyQualifiedDomainName);

    DWORD index = 0;
    TCHAR subKeyName[MAX_PATH];
    DWORD subKeyNameSize = MAX_PATH;

    while (RegEnumKeyEx(hKeyUsers, index++, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
        LPWSTR sid = subKeyName;
        PSID pSid = nullptr;

        if (ConvertStringSidToSidW(sid, &pSid)) {
            // ������ SID �������û��˻���Ϣ
            WCHAR szUserName[256] = { 0 };
            DWORD cchUserName = sizeof(szUserName) / sizeof(WCHAR);
            WCHAR szDomainName[256] = { 0 };
            DWORD cchDomainName = sizeof(szDomainName) / sizeof(WCHAR);
            SID_NAME_USE snu;

            if (!LookupAccountSidW(fullyQualifiedDomainName.c_str(), pSid, szUserName, &cchUserName, szDomainName, &cchDomainName, &snu)) {
                std::cerr << "Failed to lookup account SID: " << GetLastError() << std::endl;
                LocalFree(pSid);
            }
            else {
                // ��ȡ����ϵͳ��Ϣ
                std::wstring os = SearchOperatingSystem(domainValue, dcValue, computerName);
                os.erase(std::remove(os.begin(), os.end(), '\n'), os.end());

                std::wstringstream line;
                line << fullyQualifiedDomainName << L","
                    << os << L","
                    << subKeyName << L",\""
                    << szDomainName << L"\\" << szUserName << L"\""
                    << std::endl;
                outputValue << line.str();

                LocalFree(pSid);
            }
        }

        subKeyNameSize = MAX_PATH;
    }

    return true;
}