#include "Device.h"
#include "Logger.h"
namespace Cobol {
	Device::Device() {
		mD3DDevice=0;
		mD3DContext=0;
		mSwapChain=0;
		mBackBufferTarget=0;
		mDepthTexture=0;
		mDepthStencilView=0;
		mBlendState =0;
		mVsync = false;
	}
	RESULT Device::InitDevice(int screenW, int screenH, HWND * hwnd) {
		RECT dimension;
		GetClientRect(*hwnd, &dimension);

		unsigned int width = screenW;//dimension.right-dimension.left;
		unsigned int height = screenH;//dimension.bottom-dimension.top;

		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE 
		};

		unsigned int totalDriverType = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeatureLevels = ARRAYSIZE( featureLevels );

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = *hwnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		unsigned int creationFlags = 0;
	#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG ;
	#endif
		HRESULT result;
		unsigned int driver = 0;
		for( driver = 0; driver < totalDriverType; ++driver )
		{
			result = D3D11CreateDeviceAndSwapChain( 0, driverTypes[driver], 0,
			creationFlags, featureLevels, totalFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain,
			&mD3DDevice, &mFeatureLevel, &mD3DContext );
			if( SUCCEEDED( result ) )
			{
				mDriverType = driverTypes[driver];
				break;
			}
		}
		if( FAILED( result ) )
		{
			LOG_MSGERR( "Failed to create the Direct3D device!" );
			return R_FAILED;
		}

		// Tworzenei drugie buffora do wymiany
		ID3D11Texture2D* backBufferTexture;
		result = mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),
		( LPVOID* )&backBufferTexture );
		if( FAILED( result ) )
		{
			LOG_MSGERR( "Failed to get the swap chain back buffer!" );
			return R_FAILED;
		}
		result = mD3DDevice->CreateRenderTargetView( backBufferTexture, 0,
		&mBackBufferTarget );
		if( backBufferTexture )
			backBufferTexture->Release( );
		if( FAILED( result ) )
		{
			LOG_MSGERR( "Failed to create the render target view!" );
		
			return R_FAILED;
		}

		//Tworzenei bufora g³êbokoœci
		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc,sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = height;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		
		result = mD3DDevice->CreateTexture2D(&depthTexDesc, NULL, &mDepthTexture);
		if(FAILED(result))
		{
			LOG_MSGERR(" Failed to create the depth texture!" );
			return R_FAILED;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0 ;
		result = mD3DDevice->CreateDepthStencilView(mDepthTexture, &descDSV, &mDepthStencilView);
		if( FAILED( result ) )
		{
			
			LOG_MSGERR( "Failed to create the depth stencil target view!" );
			return R_FAILED;
		}
		else
			Logger::GetSingletonPtr()->WriteInfo("Poprawne utworzenie bufora g³êbokoœci");
		mD3DContext->OMSetRenderTargets( 1, &mBackBufferTarget, mDepthStencilView );
		D3D11_DEPTH_STENCIL_DESC  depthStencilDesc;
		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = mD3DDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
		if(FAILED(result))
		{
			return R_FAILED;
		}

		// Set the depth stencil state.
		mD3DContext->OMSetDepthStencilState(mDepthStencilState, 1);

		D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
		ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

		// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
		// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		
		result = mD3DDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &mDepthDisabledStencilState);
		if(FAILED(result))
		{
			return R_FAILED;
		}
		//BLEND DESCRITPION
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc,sizeof(blendDesc));

		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = mD3DDevice->CreateBlendState(&blendDesc,&mBlendState);
		if(FAILED(result))
		{
			LOG_MSGERR("Nie mozna utworzyc blend state");
			return R_FAILED;
		}
		float blendFactor[4] = { 0.0f, 0.0f ,0.0f, 0.0f };
		mD3DContext->OMSetBlendState(mBlendState, blendFactor, 0xffffffff);

		D3D11_RASTERIZER_DESC rasterDesc;
		ZeroMemory(&rasterDesc, sizeof(rasterDesc));

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		result = mD3DDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
		mD3DContext->RSSetState(mRasterState);
		D3D11_VIEWPORT viewport;
		//InitBuffers();
		//TODO Zrobiæ viewporty w oddzielnej klasie
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		mD3DContext->RSSetViewports( 1, &viewport );

		mProjMatrix = XMMatrixPerspectiveFovLH( XM_PIDIV2, (float)screenW / (float)screenH,
			0.01f, 100.0f );

		mOrthoMatrix = XMMatrixOrthographicLH((float)screenW,(float)screenH,0.0f, 100.0f);
		
		//mSwapChain->SetFullscreenState(true,NULL);

		Logger::GetSingletonPtr()->WriteInfo("Poprawne utworzone klasy Device");
		return R_OK;
	}
	void Device::BeginScene(float red, float green, float blue, float alpha)
	{
		float color[]  = { red, green, blue, alpha };
		mD3DContext->ClearRenderTargetView(mBackBufferTarget, color);
		mD3DContext->ClearDepthStencilView(mDepthStencilView,D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void Device::EndScene() {
		if(mVsync)
		{
			mSwapChain->Present(1,0);
		}
		else
		{
			mSwapChain->Present(0,0);
		}
	}
	Device::~Device() {
		if(mDepthStencilState)
		{
			mDepthStencilState->Release();
			mDepthStencilState = 0;
		}
		if(mDepthDisabledStencilState)
		{
			mDepthDisabledStencilState->Release();
			mDepthDisabledStencilState = 0;
		}
		if(mD3DDevice)
		{
			mD3DDevice->Release();
			mD3DDevice = 0;
		}
		if(mD3DContext)
		{
			mD3DContext->Release();
			mD3DContext = 0;
		}
		if(mSwapChain)
		{
			mSwapChain->Release();
			mSwapChain = 0;
		}
		if(mBackBufferTarget)
		{
			mBackBufferTarget->Release();
			mBackBufferTarget = 0;
		}
		if(mDepthTexture)
		{
			mDepthTexture->Release();
			mDepthTexture = 0;
		}
		if(mDepthStencilView)
		{
			mDepthStencilView->Release();
			mDepthStencilView = 0;
		}
		if(mBlendState)
		{
			mBlendState->Release();
			mBlendState = 0;
		}
	}
	void Device::SetBufferZ(bool active)
	{
		if(active)
		{
			mD3DContext->OMSetDepthStencilState(mDepthStencilState, 1);
		}
		else
		{
			mD3DContext->OMSetDepthStencilState(mDepthDisabledStencilState, 1);
		}
	}
};