#include "LDAPQuery.h"

#pragma comment(lib, "wldap32.lib")

// 构造函数，初始化 LDAP 查询对象
//LDAPQuery::LDAPQuery(const std::wstring& dc) : ldap(nullptr), version(LDAP_VERSION3), errorCode(0), dc(dc), searchResult(nullptr) {}

LDAPQuery::LDAPQuery(const std::wstring& dc) : ldap(nullptr), version(LDAP_VERSION3), errorCode(0), dc(dc), searchResult(nullptr) {}

// 析构函数，释放资源
LDAPQuery::~LDAPQuery() {
    if (searchResult) {
        ldap_msgfree(searchResult);
    }
    if (ldap) {
        ldap_unbind_s(ldap);
    }
}

// 初始化 LDAP 查询对象
bool LDAPQuery::Initialize() {
    // 初始化 LDAP 连接
    ldap = ldap_initW(const_cast<wchar_t*>(dc.c_str()), LDAP_PORT);
    if (!ldap) {
        std::cerr << "Failed to initialize LDAP." << std::endl;
        return false;
    }

    // 设置 LDAP 协议版本
    errorCode = ldap_set_option(ldap, LDAP_OPT_PROTOCOL_VERSION, &version);
    if (errorCode != LDAP_SUCCESS) {
        std::cerr << "Failed to set LDAP option." << std::endl;
        ldap_unbind_s(ldap);
        return false;
    }

    return true;
}

// 绑定 LDAP 连接
bool LDAPQuery::Bind() {
    errorCode = ldap_bind_sW(ldap, nullptr, nullptr, LDAP_AUTH_NEGOTIATE);
    if (errorCode != LDAP_SUCCESS) {
        std::cerr << "Failed to bind LDAP." << std::endl;
        ldap_unbind_s(ldap);
        return false;
    }

    return true;
}


// 执行 LDAP 查询, 接受三个参数, 分别是 LDAP 目录名称 (dn), 查询过滤器 (filter), 和要返回的属性 (attribute)
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

// 在 LDAP 查询结果中提取 "dNSHostName" 属性的值
std::wstring LDAPQuery::GetdNSHostName() {
    std::wstringstream output;

    // 将输出重定向到字符串流
    auto outputBuffer = std::wcout.rdbuf();
    std::wcout.rdbuf(output.rdbuf());

    // 遍历 LDAP 查询结果并输出到字符串流
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


// 在 LDAP 查询结果中提取 "operatingSystem" 属性的值
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
