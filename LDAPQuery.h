#ifndef LDAPQUERY_H
#define LDAPQUERY_H

#include <windows.h>
#include <winldap.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

class LDAPQuery {
public:
    LDAPQuery(const std::wstring& dc);
    ~LDAPQuery();
    //void SetCredentials(const std::wstring& username, const std::wstring& password);
    bool Initialize();
    bool Bind();
    bool Search(const std::wstring& dn, const std::wstring& filter, const std::wstring& attribute);
    std::wstring GetdNSHostName();
    std::wstring GetOperatingSystem();

private:
    LDAP* ldap;
    int version;
    ULONG errorCode;
    std::wstring dc;
    //std::wstring domain;
    LDAPMessage* searchResult;
    //std::wstring username;
    //std::wstring password;
};

#endif // LDAPQUERY_H
