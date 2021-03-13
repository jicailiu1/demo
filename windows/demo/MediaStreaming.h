#pragma once

#include "pch.h"

#include "NativeModules.h"

extern "C" {
    int start(const char* str);
}

    REACT_MODULE(MediaStreaming);
    struct MediaStreaming
    {
        REACT_METHOD(Start, L"start");
        int Start(std::string url) noexcept
        {
            return start(url.c_str());
        }
    };
