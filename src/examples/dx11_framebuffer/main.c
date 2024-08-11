#include <stdio.h>

#include <initguid.h>

#include <windows.h>
#include <dxgi.h>
#include <d3d11.h>

// Only supported by MSVC unfortunately...
// But Windows is being stupid so this is a temporary fix.
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

static LRESULT CALLBACK window_proc(HWND hwnd, UINT code, WPARAM wparam, LPARAM lparam) {
    switch (code) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProcW(hwnd, code, wparam, lparam);
    }
}

static int poll_window() {
    MSG msg;

    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        DispatchMessageW(&msg);

        if (msg.message == WM_QUIT) {
            return FALSE;
        }
    }

    return TRUE;
}

int main() {
    WNDCLASSEXW wndc = { sizeof(WNDCLASSEXW) };
    wndc.lpszClassName = L"Cubensis Example Class";
    wndc.lpfnWndProc = window_proc;
    wndc.style = CS_VREDRAW | CS_HREDRAW;
    RegisterClassExW(&wndc);

    HWND hwnd = CreateWindowExW(0, L"Cubensis Example Class", L"Cubensis Framebuffer Demo", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, NULL, NULL);
    if (hwnd == NULL) {
        return 0;
    }

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(scd));
    scd.BufferCount = 1;
    scd.BufferDesc.Width = 1280;
    scd.BufferDesc.Height = 720;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 165;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.OutputWindow = hwnd;

    D3D_FEATURE_LEVEL featuresRequested = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL featuresSupported;
    ZeroMemory(&featuresSupported, sizeof(featuresSupported));

    IDXGISwapChain *swapChain;
    ID3D11Device *device;
    ID3D11DeviceContext *ctx;

    HRESULT hr;

    // In a future patch, this should probably be switched
    // to a software driver for emulating the GPU hardware.
    if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featuresRequested, 1, 
            D3D11_SDK_VERSION, &scd, &swapChain, &device, &featuresSupported, &ctx))) {
        printf("ERROR: 0x%x\n", hr);
        DestroyWindow(hwnd);
        return 0;
    }

    D3D11_RENDER_TARGET_VIEW_DESC vdesc;
    vdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    vdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    vdesc.Buffer.ElementOffset = 0;
    vdesc.Buffer.ElementWidth = 1280;
    vdesc.Texture2D.MipSlice = 0;

    ID3D11RenderTargetView *view;
    FLOAT clearColor[4] = { 0.5f, 0.0f, 0.8f, 1.0f };

    ID3D11Texture2D *screenBuffer;
    swapChain->lpVtbl->GetBuffer(swapChain, 0, &IID_ID3D11Texture2D, &screenBuffer);

    device->lpVtbl->CreateRenderTargetView(device, (ID3D11Resource *)screenBuffer, &vdesc, &view);

    ctx->lpVtbl->OMSetRenderTargets(ctx, 1, &view, NULL);
    ctx->lpVtbl->ClearRenderTargetView(ctx, view, clearColor);
    
    swapChain->lpVtbl->Present(swapChain, 0, 0);

    ShowWindow(hwnd, SW_SHOW);

    while (poll_window()) {
        swapChain->lpVtbl->Present(swapChain, 1, 0);
    }

    screenBuffer->lpVtbl->Release(screenBuffer);

    view->lpVtbl->Release(view);

    ctx->lpVtbl->Release(ctx);
    device->lpVtbl->Release(device);
    swapChain->lpVtbl->Release(swapChain);

    DestroyWindow(hwnd);

    UnregisterClassW(L"Cubensis Example Class", NULL);

    return 0;
}