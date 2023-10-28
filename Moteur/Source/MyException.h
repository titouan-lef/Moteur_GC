#pragma once
#include "EngineException.h"

#include <sstream>
#include <iostream>
#include <string>
#include "dxerr.h"


#define GFX_EXCEPT_NOINFO(hr) MyException::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw MyException::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) MyException::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) MyException::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw MyException::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) MyException::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) MyException::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

namespace MyException {
    static wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
    {
        wchar_t* wString = new wchar_t[4096];
        MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
        return wString;
    };

    static std::string ConvertWStringToString(const wchar_t* wString)
    {
        int numChars = WideCharToMultiByte(CP_UTF8, 0, wString, -1, NULL, 0, NULL, NULL);
        char* buf = new char[numChars];
        WideCharToMultiByte(CP_UTF8, 0, wString, -1, buf, numChars, NULL, NULL);
        std::string strResult(buf);
        delete[] buf;
        return strResult;
    }

    class Exception : public EngineException {
        using EngineException::EngineException;
    };

    class HrException : public Exception {
    public:
        HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
        std::string GetErrorDescription() const noexcept;
        std::string GetErrorInfo() const noexcept;
        std::string func;
        int m_line;
    private:
        HRESULT hr;
        std::string info;
    };

    class InfoException : public Exception
    {
    public:
        InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        std::string GetErrorInfo() const noexcept;
    private:
        std::string info;
    };

    class DeviceRemovedException : public HrException {
        using HrException::HrException;
    public:
        const char* GetType()const noexcept override;
    private:
        std::string reason;
    };
}
