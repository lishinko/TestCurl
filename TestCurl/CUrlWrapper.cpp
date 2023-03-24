#include "pch.h"
#include "CUrlWrapper.h"
#include <sstream>
#include <format>
void CUrlWrapper::Init(std::function<CURLcode(int len, void* content)> onReceive)
{
    _onReceive = onReceive;
}
CUrlWrapper::CUrlWrapper()
{
    auto ret = curl_global_init(CURL_GLOBAL_WIN32);
    if (ret != CURLE_OK)
    {
        OutputDebugString(TEXT("curl init failed!"));
    }
    else
    {
        const char* v = curl_version();
        std::stringstream ss;
        ss << "curl init ok! v = " << v;
        OutputDebugStringA(ss.str().c_str());
        _handle = curl_easy_init();
    }
}

CUrlWrapper::~CUrlWrapper()
{
    curl_easy_cleanup(_handle);
    _handle = nullptr;
    curl_global_cleanup();
}
size_t onReceive(void* curlReturnDataPtr, size_t size, size_t nmemb, void* pThis) {
    CUrlWrapper* wrapper = reinterpret_cast<CUrlWrapper*>(pThis);
    int sizeInBytes = size * nmemb;
    auto ret = wrapper->_onReceive(sizeInBytes, curlReturnDataPtr);
    return sizeInBytes;
}

CURLcode CUrlWrapper::GetHttp(const char* url)
{
    curl_easy_setopt(_handle, CURLOPT_URL, url);
    curl_easy_setopt(_handle, CURLOPT_WRITEFUNCTION, onReceive);
    curl_easy_setopt(_handle, CURLOPT_WRITEDATA, this);
    auto ret = curl_easy_perform(_handle);
    if (ret != CURLE_OK)
    {
        auto log = std::format("curl failed! ret = {}", static_cast<int>(ret));
        OutputDebugStringA(log.c_str());
    }

    return ret;
}
