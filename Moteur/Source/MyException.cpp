#include "MyException.h"

MyException::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept : Exception(line, file), hr(hr)
{
    m_line = line;
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char* MyException::HrException::what() const noexcept
{

    std::ostringstream oss;
    oss << GetType() << std::endl

        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorDescription() << std::endl;


    if (!info.empty())
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    }
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* MyException::HrException::GetType() const noexcept
{
    return "Graphics Exception";
}


HRESULT MyException::HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string MyException::HrException::GetErrorString() const noexcept
{
    std::wstring wStr = DXGetErrorString(hr);

    int length = WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::vector<char> buffer(length);
    WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), -1, buffer.data(), length, nullptr, nullptr);

    return std::string(buffer.data());
}


std::string MyException::HrException::GetErrorDescription() const noexcept
{
    wchar_t wbuf[512];
    DXGetErrorDescription(hr, wbuf, sizeof(wbuf) / sizeof(wchar_t));
    return ConvertWStringToString(wbuf);
}

std::string MyException::HrException::GetErrorInfo() const noexcept
{
    return info;
}

const char* MyException::DeviceRemovedException::GetType() const noexcept
{
    return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

MyException::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file)
{

    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char* MyException::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* MyException::InfoException::GetType() const noexcept
{
    return "Graphics Info Exception";
}

std::string MyException::InfoException::GetErrorInfo() const noexcept
{
    return info;
}