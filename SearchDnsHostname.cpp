#include "LDAPQuery.h"
#include "SearchDnsHostname.h"

// 执行 LDAP 搜索, 检索所有计算机对象的 dNSHostName 属性
std::wstring SearchDnsHostname(const std::wstring& domainValue, const std::wstring& dcValue)
{
    LDAPQuery ldapQuery(dcValue);

    if (!ldapQuery.Initialize()) {
        std::cerr << "Failed to initialize LDAP connection.\n";
        std::exit(1);
    }

    if (!ldapQuery.Bind()) {
        std::cerr << "Failed to bind to LDAP server.\n";
        std::exit(1);
    }

    std::wstring domain = domainValue.substr(0, domainValue.find('.'));
    std::wstring dn = L"DC=" + domain + L"," + L"DC=" + domainValue.substr(domainValue.find('.') + 1);
    std::wstring filter = L"(&(objectClass=computer))";
    std::wstring attribute = L"dNSHostName";

    if (!ldapQuery.Search(dn, filter, attribute)) {
        std::cerr << "Failed to perform LDAP search.\n";
        std::exit(1);
    }

    return ldapQuery.GetdNSHostName();

    
}
