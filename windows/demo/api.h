#if DLLEXPORT
#define CLASS_DECLSPEC __declspec(dllexport)
#elif DLLIMPORT
#define CLASS_DECLSPEC __declspec(dllimport)
#else
#define CLASS_DECLSPEC
#endif


extern "C" {
CLASS_DECLSPEC int init(const char* str, int width, int height, ID3D11Device* pDevice, LiveStreamingClient** pClient);

CLASS_DECLSPEC void onData(ID3D11Texture2D* texture);
}
