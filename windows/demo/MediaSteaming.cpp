#include "pch.h"
#include "Mediastreaming.h"
#include "d3d11interop.h"

namespace winrt {
    using namespace Windows::Foundation;
    using namespace Windows::Graphics::Capture;
    using namespace Windows::ApplicationModel::Core;
    using namespace Microsoft::ReactNative;
    using namespace Windows::Graphics::DirectX;
    using namespace Windows::Graphics::DirectX::Direct3D11;
    using namespace Windows::Graphics::Imaging;
    using namespace Windows::Storage;
    using namespace Windows::Storage::Streams;

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

void MediaStreaming::Capture() noexcept
{
    m_captureRequested = true;
}

winrt::fire_and_forget save(const winrt::IDirect3DSurface surface)
{
    try {
        auto bitmap = co_await winrt::SoftwareBitmap::CreateCopyFromSurfaceAsync(surface);

        auto file = co_await winrt::KnownFolders::CameraRoll().CreateFileAsync(L"save.jpg", winrt::CreationCollisionOption::ReplaceExisting);
        auto outputStream = co_await file.OpenAsync(winrt::FileAccessMode::ReadWrite);
        auto encoder = co_await winrt::BitmapEncoder::CreateAsync(winrt::BitmapEncoder::JpegEncoderId(), outputStream);
        encoder.SetSoftwareBitmap(bitmap);
        co_await encoder.FlushAsync();
    }
    catch (...) {
        int a = 1;
    }
}


void MediaStreaming::OnFrameArrived(
    winrt::Direct3D11CaptureFramePool const& sender,
    winrt::IInspectable const&)
{
    auto surface = sender.TryGetNextFrame().Surface();
    auto frameTexture = GetDXGIInterfaceFromObject<ID3D11Texture2D>(surface);

    if (m_captureRequested)
    {
        save(surface);
        m_captureRequested = false;
    }

    onData(frameTexture.get());
}


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

    m_framePool = winrt::Direct3D11CaptureFramePool::CreateFreeThreaded(
        m_device,
        winrt::DirectXPixelFormat::R8G8B8A8UIntNormalized,
        1,
        item.Size());
    m_framePool.FrameArrived({ this, &MediaStreaming::OnFrameArrived });

    init(url.c_str(), item.Size().Width, item.Size().Height, m_d3dDevice.get(), &m_client);
    m_client->start();

    m_session = m_framePool.CreateCaptureSession(item);
    m_session.StartCapture();
}

void MediaStreaming::Initialize(React::ReactContext const& reactContext) noexcept
{
    m_reactContext = reactContext;
    auto hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, m_d3dDevice.put(),
        nullptr, nullptr);
    assert(SUCCEEDED(hr));

    auto dxgiDevice = m_d3dDevice.as<IDXGIDevice>();
    m_device = CreateDirect3DDevice(dxgiDevice.get());

}