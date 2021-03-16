#include "pch.h"
#include "Mediastreaming.h"

namespace winrt {
    using namespace Windows::Foundation;
    using namespace Windows::Graphics::Capture;
    using namespace Windows::ApplicationModel::Core;
    using namespace Microsoft::ReactNative;
}

struct awaitable_event
{
    void set() const noexcept
    {
        SetEvent(os_handle());
    }

    auto operator co_await() const noexcept
    {
        return winrt::resume_on_signal(os_handle());
    }

private:
    HANDLE os_handle() const noexcept
    {
        return handle.get();
    }

    winrt::handle handle{
      winrt::check_pointer(CreateEvent(nullptr,
      /* manual reset */ true, /* initial state */ false,
      nullptr)) };
};

winrt::fire_and_forget MediaStreaming::Start(std::string url) noexcept
{
    winrt::GraphicsCaptureItem item = nullptr;
    //awaitable_event pickEvent;

    /*auto dispatcher = winrt::Windows::UI::Core::CoreWindow::GetForCurrentThread().Dispatcher();
    dispatcher = winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher();
    co_await winrt::resume_foreground(dispatcher);*/
    auto picker = winrt::Windows::Graphics::Capture::GraphicsCapturePicker();
    item = co_await picker.PickSingleItemAsync();
    /*bool access = dispatcher.HasThreadAccess();
     
    co_await dispatcher.RunAsync(
        winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, [&item, &pickEvent]() -> winrt::IAsyncAction
    {
        auto picker = winrt::Windows::Graphics::Capture::GraphicsCapturePicker();
        item = co_await picker.PickSingleItemAsync();
        pickEvent.set();
    });*/


    //auto dispatcher = Dispatcher();
    //auto dispatcher1 = winrt::ReactDispatcherHelper::UIThreadDispatcher();

    

    //auto dispatcher1 = m_context.UIDispatcher();
    //dispatcher1.Post([&item, &pickEvent]() -> winrt::IAsyncAction {
    //    auto picker = winrt::Windows::Graphics::Capture::GraphicsCapturePicker();
    //    item = co_await picker.PickSingleItemAsync();
    //    pickEvent.set();
    //    });

    //co_await pickEvent;

    init(url.c_str(), m_d3dDevice.get(), &m_client);
}

void MediaStreaming::Initialize(React::ReactContext const& reactContext) noexcept
{
    m_reactContext = reactContext;
    auto hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, m_d3dDevice.put(),
        nullptr, nullptr);
    assert(SUCCEEDED(hr));
}