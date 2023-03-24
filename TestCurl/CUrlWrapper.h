#pragma once
#include <string>
#include <curl/curl.h>
#include <functional>
class CUrlWrapper
{
public:
    void Init(std::function<CURLcode(int len, void* content)> onReceive);
    CUrlWrapper();
    ~CUrlWrapper();
    CURLcode GetHttp(const char* url);
    /// <summary>
    /// ����ǻص�,��Ҫ��������!
    /// </summary>
    std::function<CURLcode(int len, void* content)> _onReceive;
private:
    CURL* _handle;
};

