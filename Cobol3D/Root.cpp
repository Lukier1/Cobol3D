#include "Logger.h"
#include "Root.h"
#include "Material.h"
#include "Mesh.h"
#include "SceneMesh.h"
#include "Camera.h"
#include <xnamath.h>


namespace Cobol {
	
	
	bool Root::NewWindow(HINSTANCE &hInstance, int cmdShow) {
		
		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof( WNDCLASSEX ) ;
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.hInstance = hInstance;
		wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
		wndClass.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = "CobolWindowClass";
		if( !RegisterClassEx( &wndClass ) )
			return false;
		RECT rc = { 0, 0, mResW, mResH };
		AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
		mHwnd = CreateWindowA( "CobolWindowClass", "Puste Okno",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.
		left,
		rc.bottom - rc.top, NULL, NULL, hInstance, NULL );
		if( !mHwnd )
			return false;
		ShowWindow( mHwnd, cmdShow );

		return true;
	}
	LRESULT CALLBACK Root::WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) 
	{
		PAINTSTRUCT paintStruct;
		HDC hDC;
		switch( message )
		{
		case WM_PAINT:
			hDC = BeginPaint( hwnd, &paintStruct );
			EndPaint( hwnd, &paintStruct );
		break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
		break;
		default:
			return DefWindowProc( hwnd, message, wParam, lParam );
		}
		return 0;
	}
		Material* mat ;
	bool Root::Run() {
		InitDirectX();
	mat = new Material(mD3DDevice, mD3DContext);
		mat->LoadContent();
		mat->InitPipeLine();

		MSG msg = {0};
		while(msg.message != WM_QUIT)
		{
			if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Render();
				// Update and Draw
				//demo->Update( 0.0f );
				//demo->Render( );
			}
		}
		return true;
	}

	bool Root::InitDirectX() {
		//####################################################
		//INICIJALIZACJA DIRECTX
	
		RECT dimension;
		GetClientRect(mHwnd, &dimension);

		unsigned int width = dimension.right-dimension.left;
		unsigned int height = dimension.bottom-dimension.top;

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
		swapChainDesc.OutputWindow = mHwnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		unsigned int creationFlags = 0;
	#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
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
			return false;
		}
		ID3D11Texture2D* backBufferTexture;
		result = mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),
		( LPVOID* )&backBufferTexture );
		if( FAILED( result ) )
		{
			LOG_MSGERR( "Failed to get the swap chain back buffer!" );
			return false;
		}
		result = mD3DDevice->CreateRenderTargetView( backBufferTexture, 0,
		&mBackBufferTarget );
		if( backBufferTexture )
			backBufferTexture->Release( );
		if( FAILED( result ) )
		{
			LOG_MSGERR( "Failed to create the render target view!" );
		
			return false;
		}

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
			return false;
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
			return false;
		}
		else
			Logger::GetSingletonPtr()->WriteInfo("Poprawne utworzenie bufora g³êbokoœci");
		mD3DContext->OMSetRenderTargets( 1, &mBackBufferTarget, mDepthStencilView );

		InitBuffers();
		//TODO Zrobiæ viewporty w oddzielnej klasie

		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		mD3DContext->RSSetViewports( 1, &viewport );

		XMVECTOR Eye = XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
		XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		mViewMatrix = XMMatrixLookAtLH( Eye, At, Up );
		mProjMatrix = XMMatrixPerspectiveFovLH( XM_PIDIV2, 800.0f / 600.0f,
			0.01f, 100.0f );
		
		
		return false;
	}

	void Root::InitBuffers()
	{

		D3D11_BUFFER_DESC constDesc;
		ZeroMemory( &constDesc, sizeof( constDesc ) );
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = sizeof( ConstantBuffer );
		constDesc.Usage = D3D11_USAGE_DEFAULT;
		constDesc.CPUAccessFlags = 0;

		HRESULT result  = mD3DDevice->CreateBuffer( &constDesc, 0, &mCB);
		if(FAILED(result))
		{
			LOG_MSGERR("Problem with creating constant buffer.")
		}
		sMesh = new SceneMesh();
		sMesh->setMaterial(new Material(mD3DDevice,mD3DContext));
		mesh = new CubeMesh();
		mesh->LoadContent(mD3DDevice,mD3DContext);
		sMesh->setMesh(mesh);
		
	}

	void Root::DrawSceneMesh(SceneMesh * sceneMesh)
	{

		 XMFLOAT4 vLightDirs[2] =
		{
			XMFLOAT4( -0.577f, 0.577f, -0.577f, 1.0f ),
			XMFLOAT4( 0.0f, 0.0f, -1.0f, 1.0f ),
		};
		XMFLOAT4 vLightColors[2] =
		{
			XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f ),
			XMFLOAT4( 0.5f, 0.0f, 0.0f, 1.0f )
		};
		static float t = 0;
		t+=0.001f;
		XMMATRIX mRotate = XMMatrixRotationY( -2.0f * t );
		XMVECTOR vLightDir = XMLoadFloat4( &vLightDirs[1] );
		vLightDir = XMVector3Transform( vLightDir, mRotate );
		XMStoreFloat4( &vLightDirs[1], vLightDir );
		XMMATRIX worldMat = sceneMesh->getTransform();
	
		 ConstantBuffer cb;
		 cb.worldMatrix = worldMat; TransposeMatrix( cb.worldMatrix );
		 cb.viewMatrix =mViewMatrix; TransposeMatrix( cb.viewMatrix);
		 cb.projMatrix = mProjMatrix; TransposeMatrix( cb.projMatrix );
		 cb.vlightColor[0] = vLightColors[0];
		 cb.vlightColor[1] = vLightColors[1];
		 cb.vlightDir[0] = vLightDirs[0];
		 cb.vlightDir[1] = vLightDirs[1];
		 mD3DContext->UpdateSubresource(mCB, 0, NULL, &cb, 0, 0 );
		 mD3DContext->VSSetConstantBuffers(0,1,&mCB);
	 mD3DContext->PSSetConstantBuffers(0,1,&mCB);
	
		sceneMesh->DrawMesh(mD3DDevice, mD3DContext);

	}
	void Root::Render()
	{
		if( mD3DContext == 0 )
			return;

		static float angle = 0.0f;
		angle+=0.001f;
	
		float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
		mD3DContext->ClearRenderTargetView( mBackBufferTarget, clearColor );
		mD3DContext->ClearDepthStencilView( mDepthStencilView,D3D11_CLEAR_DEPTH, 1.0f, 0 );
		{
			float pos[] = {0.0f, 0.0f, 0.0f};
			sMesh->setPosition(pos);
			DrawSceneMesh(sMesh);
		}
		{
			float pos[] = {1.0f, 2.0f, 3.0f};
			sMesh->setPosition(pos);
			DrawSceneMesh(sMesh);
		}
		
		mSwapChain->Present( 0, 0 );
	}

	void Root::Clean()
	{
	//	if( mViewCB ) mViewCB->Release( );	mViewCB = nullptr;
	//	if( mProjCB ) mProjCB->Release( );	mProjCB = nullptr;
	//	if( mWorldCB )  mWorldCB ->Release( ); mWorldCB = nullptr;
		if(mCB) mCB->Release(); mCB = nullptr;
	}
}