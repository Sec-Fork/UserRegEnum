#include "LDAPQuery.h"
#include "SearchOperatingSystem.h"

// 执行 LDAP 搜索, 检索指定计算机对象的 operatingSystem 属性, 即操作系统信息
std::wstring SearchOperatingSystem(const std::wstring& domainValue, const std::wstring& dcValue, const std::wstring& computerName) {
    LDAPQuery ldapQuery(dcValue);

    if (!ldapQuery.Initialize()) {
        std::cerr << "Failed to initialize LDAP connection." << std::endl;
        exit(1);
    }

    if (!ldapQuery.Bind()) {
        std::cerr << "Failed to bind to LDAP server." << std::endl;
        exit(1);
    }


    std::wstring domain = domainValue.substr(0, domainValue.find('.'));
    std::wstring dn = L"DC=" + domain + L"," + L"DC=" + domainValue.substr(domainValue.find('.') + 1);
    std::wstring filter = L"(&(objectClass=computer)(cn=" + computerName + L"))";
    std::wstring attribute = L"operatingSystem";
    if (!ldapQuery.Search(dn, filter, attribute)) {
        std::cerr << "Failed to perform LDAP search." << std::endl;
        exit(1);
    }

    return ldapQuery.GetOperatingSystem();
}
