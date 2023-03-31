#include <iostream>
#include "SearchDnsHostname.h"
#include "RegConnect.h"

void print_help() {
    std::cout << "Usage example: \n"
        << "  program.exe -d missyou.com -dc dc.missyou.com -o result.csv\n"
        << "Options:\n"
        << "  -d, --domain=Domain               domain name to search\n"
        << "  -dc, --dc=domainControllers       domain controller to query\n"
        << "  -t, --threads=Theads              number of threads, default: 1000\n"
        << "  -o, --output=outputResult         output result file\n"
        << "  -h, --help                        display this help and exit\n";
}

int main(int argc, char** argv) {
    std::string domain;
    std::string dc;
    int threads = 0;
    std::string output;
    bool dcFlag = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_help();
            return 0;
        }
        else if (arg == "-d" || arg == "--domain") {
            if (i + 1 >= argc) {
                std::cout << "Missing argument for " << arg << std::endl;
                return 1;
            }
            domain = argv[++i];
        }
        else if (arg == "-dc" || arg == "--dc") {
            if (i + 1 >= argc) {
                std::cout << "Missing argument for " << arg << std::endl;
                return 1;
            }
            dc = argv[++i];
            dcFlag = true;
        }
        else if (arg == "-t" || arg == "--threads") {
            if (i + 1 >= argc) {
                std::cout << "Missing argument for " << arg << std::endl;
                return 1;
            }
            threads = std::stoi(argv[++i]);
        }
        else if (arg == "-o" || arg == "--output") {
            if (i + 1 >= argc) {
                std::cout << "Missing argument for " << arg << std::endl;
                return 1;
            }
            output = argv[++i];
        }
        else {
            std::cout << "Unknown option: " << arg << std::endl;
            return 1;
        }
    }

    std::wstring dcValue;
    if (dcFlag) {
        dcValue = std::wstring(dc.begin(), dc.end());
    }
    else
    {
        std::cout << "Missing required argument: -dc or --dc" << std::endl;
        return 1;
    }

    std::wstring domainValue;
    if (!domain.empty()) {
        domainValue = std::wstring(domain.begin(), domain.end());
    }
    else
    {
        std::cout << "Missing required argument: -d or --domain" << std::endl;
        return 1;
    }

    std::wstring outputValue;
    if (!output.empty()) {
        outputValue = std::wstring(output.begin(), output.end());
    }
    else
    {
        std::cout << "Missing required argument: -o or --output" << std::endl;
        return 1;
    }

    // 打开输出文件，并设置为追加模式
    std::wofstream outfile(outputValue, std::ios::app);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return false;
    }

    // 写入标题
    outfile.imbue(std::locale("zh_cn.utf-8"));
    outfile << wchar_t(0xfeff);
    outfile << L"计算机,os,sid,登陆的用户\n";

    RegConnect(domainValue, dcValue, outfile, threads);

    outfile.close();

    std::cout << "Success!" << std::endl;

    return 0;
}
