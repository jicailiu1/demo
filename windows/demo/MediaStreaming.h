#pragma once

#include "pch.h"

#include "NativeModules.h"
#include <d3d11.h>

class LiveStreamingClient {
public:
    virtual ~LiveStreamingClient() = default;
    virtual void start() = 0;
    virtual void pause(bool sendStreamInterrupted) = 0;
    virtual void resume() = 0;
    virtual void stop(bool sendEOS = true, bool teardownNetworkSessionImmidiately = false) = 0;
};

extern "C" {
    //int start(const char* str);
    int init(const char* str, ID3D11Device* pDevice, LiveStreamingClient** pClient);
}

REACT_MODULE(MediaStreaming);
struct MediaStreaming
{
    REACT_METHOD(Start, L"start");
    winrt::fire_and_forget Start(std::string url) noexcept;

    REACT_INIT(Initialize)
    void Initialize(React::ReactContext const& reactContext) noexcept;

   React::ReactContext m_reactContext;

   winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice m_device{ nullptr };
   winrt::com_ptr<ID3D11Device> m_d3dDevice;
   winrt::com_ptr<ID3D11DeviceContext> m_d3dContext;

   LiveStreamingClient* m_client{nullptr};
};
