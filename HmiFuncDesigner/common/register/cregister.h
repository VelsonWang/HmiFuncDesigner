#ifndef CREGISTER_H
#define CREGISTER_H

#include <iostream>

class CRegister
{
public:
    explicit CRegister();
    ~CRegister();

    static bool CheckRegisterCode();
    static bool AuthRegisterCode(const std::string& code);
    static std::string GengerateCode(const std::string& key, const std::string& origin);
    static std::string GetCPUId() { return get_cpuid(); }

private:
    static std::string get_cpuid();
    static std::string aes_decode(const std::string&key, const std::string& code);
    static std::string aes_encode(const std::string&key, const std::string& code);
};

#endif // CREGISTER_H
