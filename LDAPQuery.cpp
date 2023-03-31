#include "LDAPQuery.h"

#pragma comment(lib, "wldap32.lib")

// ���캯������ʼ�� LDAP ��ѯ����
//LDAPQuery::LDAPQuery(const std::wstring& dc) : ldap(nullptr), version(LDAP_VERSION3), errorCode(0), dc(dc), searchResult(nullptr) {}

LDAPQuery::LDAPQuery(const std::wstring& dc) : ldap(nullptr), version(LDAP_VERSION3), errorCode(0), dc(dc), searchResult(nullptr) {}

// �����������ͷ���Դ
LDAPQuery::~LDAPQuery() {
    if (searchResult) {
        ldap_msgfree(searchResult);
    }
    if (ldap) {
        ldap_unbind_s(ldap);
    }
}

// ��ʼ�� LDAP ��ѯ����
bool LDAPQuery::Initialize() {
    // ��ʼ�� LDAP ����
    ldap = ldap_initW(const_cast<wchar_t*>(dc.c_str()), LDAP_PORT);
    if (!ldap) {
        std::cerr << "Failed to initialize LDAP." << std::endl;
        return false;
    }

    // ���� LDAP Э��汾
    errorCode = ldap_set_option(ldap, LDAP_OPT_PROTOCOL_VERSION, &version);
    if (errorCode != LDAP_SUCCESS) {
        std::cerr << "Failed to set LDAP option." << std::endl;
        ldap_unbind_s(ldap);
        return false;
    }

    return true;
}

// �� LDAP ����
bool LDAPQuery::Bind() {
    errorCode = ldap_bind_sW(ldap, nullptr, nullptr, LDAP_AUTH_NEGOTIATE);
    if (errorCode != LDAP_SUCCESS) {
        std::cerr << "Failed to bind LDAP." << std::endl;
        ldap_unbind_s(ldap);
        return false;
    }

    return true;
}


// ִ�� LDAP ��ѯ, ������������, �ֱ��� LDAP Ŀ¼���� (dn), ��ѯ������ (filter), ��Ҫ���ص����� (attribute)
bool LDAPQuery::Search(const std::wstring& dn, const std::wstring& filter, const std::wstring& attribute) {
    PWSTR attributes[] = { const_cast<wchar_t*>(attribute.c_str()), nullptr };
    PWSTR filterPtr = const_cast<wchar_t*>(filter.c_str());
    PWSTR dnPtr = const_cast<wchar_t*>(dn.c_str());

    errorCode = ldap_search_sW(ldap, dnPtr, LDAP_SCOPE_SUBTREE, filterPtr, attributes, 0, &searchResult);
    if (errorCode != LDAP_SUCCESS) {
        std::cerr << "Failed to search LDAP." << std::endl;
        ldap_unbind_s(ldap);
        return false;
    }

    return true;
}

// �� LDAP ��ѯ�������ȡ "dNSHostName" ���Ե�ֵ
std::wstring LDAPQuery::GetdNSHostName() {
    std::wstringstream output;

    // ������ض����ַ�����
    auto outputBuffer = std::wcout.rdbuf();
    std::wcout.rdbuf(output.rdbuf());

    // ���� LDAP ��ѯ�����������ַ�����
    for (LDAPMessage* entry = ldap_first_entry(ldap, searchResult); entry != nullptr; entry = ldap_next_entry(ldap, entry))
    {
        PWSTR* values = ldap_get_valuesW(ldap, entry, const_cast<wchar_t*>(L"dNSHostName"));

        if (values != nullptr)
        {
            std::wcout << values[0] << std::endl;
            ldap_value_freeW(values);
        }
    }

    std::wcout.rdbuf(outputBuffer);

    return output.str();
}


// �� LDAP ��ѯ�������ȡ "operatingSystem" ���Ե�ֵ
std::wstring LDAPQuery::GetOperatingSystem() {
    std::wostringstream output;

    auto outputBuffer = std::wcout.rdbuf();
    std::wcout.rdbuf(output.rdbuf());

    for (LDAPMessage* entry = ldap_first_entry(ldap, searchResult); entry != nullptr; entry = ldap_next_entry(ldap, entry))
    {
        PWSTR* values = ldap_get_valuesW(ldap, entry, const_cast<wchar_t*>(L"operatingSystem"));

        if (values != nullptr)
        {
            output << values[0] << std::endl;
            ldap_value_freeW(values);
        }
    }

    std::wcout.rdbuf(outputBuffer);
    return output.str();
}
