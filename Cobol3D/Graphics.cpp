#include "GraphicClass.h"
#include "Logger.h"
#include "MyMath.h"

#include <string>

namespace Cobol {
	GraphicClass::GraphicClass() : beginTime( std::clock()) {
		mFrameCB=0;
		mSkinCB=0;
		mStatCB=0;
		mPassCB=0;
		mMaterCB=0;
		
		mDevice = 0;
		mCamera = 0;
		mModel = 0;
		mTextureShader = 0;
		mLightShader = 0;
		mLight = 0;
		mBitmap = 0;
		mText = 0;
	};

	int GraphicClass::InitBuffer() {
		HRESULT result;
		
		D3D11_BUFFER_DESC constDesc;
	
		ZeroMemory( &constDesc, sizeof( constDesc ) );
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = sizeof( VSPerFrameCB );
		constDesc.Usage = D3D11_USAGE_DYNAMIC;
		constDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		result  =  mDevice->getDevice()->CreateBuffer( &constDesc, 0, &mFrameCB);
		if(FAILED(result))
		{
			LOG_MSGERR("Problem with creating constant buffer.")
			return false;
		}
			
		ZeroMemory( &constDesc, sizeof( constDesc ) );
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = sizeof(  VSPerSkinedCB  );
		constDesc.Usage =  D3D11_USAGE_DEFAULT;
		constDesc.CPUAccessFlags = 0;

		result  =  mDevice->getDevice()->CreateBuffer( &constDesc, 0, &mSkinCB);
		if(FAILED(result))
		{
			LOG_MSGERR("Problem with creating constant buffer.")
			return false;
		}
	
		ZeroMemory( &constDesc, sizeof( constDesc ) );
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = sizeof(  VSPerStaticCB );
		constDesc.Usage =  D3D11_USAGE_DYNAMIC;
		constDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		result  =  mDevice->getDevice()->CreateBuffer( &constDesc, 0, &mStatCB);
		if(FAILED(result))
		{
			LOG_MSGERR("Problem with creating constant buffer.")
			return false;
		}
		
		ZeroMemory( &constDesc, sizeof( constDesc ) );
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = sizeof(  VSPerPassCB  );
		constDesc.Usage = D3D11_USAGE_DYNAMIC;
		constDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		result  =  mDevice->getDevice()->CreateBuffer( &constDesc, 0, &mPassCB);
		if(FAILED(result))
		{
			LOG_MSGERR("Problem with creating constant buffer.")
			return false;
		}
		
		ZeroMemory( &constDesc, sizeof( constDesc ) );
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = sizeof(  VSPerMaterialCB  );
		constDesc.Usage =  D3D11_USAGE_DYNAMIC;
		constDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		result  =  mDevice->getDevice()->CreateBuffer( &constDesc, 0, &mMaterCB);
		if(FAILED(result))
		{
			LOG_MSGERR("Problem with creating constant buffer.")
			return false;
		}

		return true;
	}
	void GraphicClass::Frame(InputData &input)
	{
		
		if(mDevice)
		{
			ID3D11DeviceContext * devCon = mDevice->getContext();
			
			//UPDATING BUFFERS
			XMMATRIX viewMatrix, worldMatrix = XMMatrixIdentity(), projectionMatrix,orthoMatrix;

			static float rotation = 0.0f;


			// Update the rotation variable each frame.
			rotation += (float)XM_PI * 0.0008f;
			if(rotation > 2*XM_PI)
			{
				rotation -= 2*XM_PI;
			}
			 XMMATRIX mSpin = XMMatrixRotationZ( rotation);
			XMMATRIX mOrbit = XMMatrixRotationY(rotation  );
			XMMATRIX mTranslate = XMMatrixTranslation( -0.0f, 0.0f, -1.0f );
			 XMMATRIX mScale = XMMatrixScaling( 0.8f, 0.8f, 0.8f );


			 worldMatrix = mScale * mSpin * mTranslate *mOrbit;
			
			// Get the world, view, and projection matrices from the camera and d3d objects.
			mCamera->getMatrix(viewMatrix);
			mDevice->GetProjectionMatrix(projectionMatrix);
			mDevice->GetOrthoMatrix(orthoMatrix);
			TransposeMatrix(worldMatrix);
			TransposeMatrix(viewMatrix); 
			TransposeMatrix(projectionMatrix);
			TransposeMatrix(orthoMatrix);

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			
			
			{
				VSPerFrameCB * dataPtr;
				devCon->Map( mFrameCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				// Get a pointer to the data in the constant buffer.
				dataPtr = (VSPerFrameCB*)mappedResource.pData;
				// Copy the matrices into the constant buffer.
				dataPtr->appTime = (float)( std::clock()-beginTime);
				dataPtr->projMatrix= projectionMatrix;
				// Unlock the constant buffer.
				devCon->Unmap( mFrameCB, 0);
				devCon->VSSetConstantBuffers(0,1,&mFrameCB);
			}
			devCon->VSSetConstantBuffers(1,1,&mSkinCB);
			{
				VSPerStaticCB * dataPtr;
				devCon->Map( mStatCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				// Get a pointer to the data in the constant buffer.
				dataPtr = (VSPerStaticCB*)mappedResource.pData;
				// Copy the matrices into the constant buffer.
				dataPtr->worldMatrix = worldMatrix;
				// Unlock the constant buffer.
				devCon->Unmap( mStatCB, 0);
				devCon->VSSetConstantBuffers(2,1, &mStatCB);
			}

			{
				VSPerPassCB * dataPtr;
				devCon->Map(  mPassCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				// Get a pointer to the data in the constant buffer.
				dataPtr = (VSPerPassCB*)mappedResource.pData;
				// Copy the matrices into the constant buffer.
				dataPtr->viewMatrix = viewMatrix;
				dataPtr->cameraPosition = mCamera->GetPosition();
				dataPtr->padding = 0.0f;
				// Unlock the constant buffer.
				devCon->Unmap( mPassCB, 0);
				
				devCon->VSSetConstantBuffers(3,1, &mPassCB);
			}
		

			devCon->VSSetConstantBuffers(4,1, &mMaterCB);
			
			//#############################################################33
			mDevice->BeginScene(0.0f,0,0.2f,1.0f);
			
			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			//mModel->Render(mDevice->getContext());

			// Render the model using the color shader.
			//mTextureShader->Render(mDevice->getContext(), mModel->GetIndexCount(),mModel->getTexture());
			//mLightShader->Render(mDevice->getContext(), mModel->GetIndexCount(),mModel->getTexture(), 
			//	mLight->GetDirection(),mLight->GetDiffuseColor(),mLight->GettAmbientColor(),mLight->GetSpecularColor(),mLight->GetSpecularPower());
			mModel2->Render(mDevice->getContext());
			mLightShader->Render(mDevice->getContext(), mModel2->GetIndexCount(),mModel->getTexture(), 
				mLight->GetDirection(),mLight->GetDiffuseColor(),mLight->GettAmbientColor(),mLight->GetSpecularColor(),mLight->GetSpecularPower());
		
			mCamera->Render();
				//Chaning on ortho matrix
			{
				VSPerFrameCB * dataPtr;
				devCon->Map( mFrameCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				// Get a pointer to the data in the constant buffer.
				dataPtr = (VSPerFrameCB*)mappedResource.pData;
				// Copy the matrices into the constant buffer.
				dataPtr->appTime = (float)( std::clock()-beginTime);
				dataPtr->projMatrix= orthoMatrix;
				// Unlock the constant buffer.
				devCon->Unmap( mFrameCB, 0);
				devCon->VSSetConstantBuffers(0,1,&mFrameCB);
			}
			devCon->VSSetConstantBuffers(1,1,&mSkinCB);
			{
				VSPerStaticCB * dataPtr;
				devCon->Map( mStatCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				// Get a pointer to the data in the constant buffer.
				dataPtr = (VSPerStaticCB*)mappedResource.pData;
				// Copy the matrices into the constant buffer.
				XMMATRIX mWorld2 = XMMatrixRotationX(input.appTime);
				TransposeMatrix(mWorld2);
				dataPtr->worldMatrix = XMMatrixIdentity();
				// Unlock the constant buffer.
				devCon->Unmap( mStatCB, 0);
				devCon->VSSetConstantBuffers(2,1, &mStatCB);
			}
			{
				VSPerPassCB * dataPtr;
				devCon->Map(  mPassCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				// Get a pointer to the data in the constant buffer.
				dataPtr = (VSPerPassCB*)mappedResource.pData;
				// Copy the matrices into the constant buffer.
				dataPtr->viewMatrix =  XMMatrixIdentity();
				dataPtr->cameraPosition = mCamera->GetPosition();
				dataPtr->padding = 0.0f;
				// Unlock the constant buffer.
				devCon->Unmap( mPassCB, 0);
				
				devCon->VSSetConstantBuffers(3,1, &mPassCB);
			}
			mDevice->SetBufferZ(false);
			static float ooo = 0;
			ooo+=10.1f;
			mText->Render(devCon);
			mBitmap->Render(devCon,ooo,ooo);
			mTextureShader->Render(devCon,mBitmap->GetIndexCount(), mBitmap->getTexture());
			
			mDevice->SetBufferZ(true);
			std::string wyjscie = "MouseX: ";
			//toString(input.MouseX);
			static std::string nowy = " ";
			
			if(input.litera != NULL  ) {
				
				
					if(input.litera == 0x7F)
					{
						if(!nowy.empty())
							nowy.erase(nowy.end()-1);
					}
					else
					{
						nowy+=input.litera;
						
					}
					
				}
				
			
			
			wyjscie+=toString(input.MouseX)+" MouseY: "+toString(input.MouseY)+nowy;
			mText->UpdateText(wyjscie.c_str(),devCon);

			//mCamera->SetRotation(XMFLOAT3(0.0f,0.0f,input.appTime*0.001f));
			mDevice->EndScene();
			//mCamera->SetPosition(XMFLOAT3(0.0f,0.0f,mCamera->GetPosition().z-0.001f));
		}
	}
	
	bool GraphicClass::Init(HWND &hwnd, int screenH, int screenW )
	{
		bool result; 
		mDevice=new Device();
		if(!mDevice)
		{
			BOTHMSG("Problem with creating GraphicClass::mDevice")
			return false;
		}
		result = !FAILED(mDevice->InitDevice(screenH, screenW, &hwnd));
		if(!result)
		{
		
			BOTHMSG("Problem with init GraphicClass::mDevice")
			return false;
		}

		mCamera  = new Camera();
		if(!mCamera)
		{
			BOTHMSG("Problem with creating GraphicClass::mCamera")
			return false;
		}
		mCamera->SetPosition(XMFLOAT3(0.0f, 0.0f, -10.0f));
		
		mModel = new ModelClass();
		if(!mModel)
		{
			BOTHMSG("Problem with creating GraphicClass::mModel")
			return false;
		}
		if(!mModel->Init(mDevice->getDevice(),"data/Texture1.dds",L"data/chata.cobM"))
		{
			BOTHMSG("Problem with init mModel")
			return false;
		}
		
		mModel2 = new ModelClass();
		if(!mModel)
		{
			BOTHMSG("Problem with creating GraphicClass::mModel")
			return false;
		}
		if(!mModel2->Init(mDevice->getDevice(),"data/Texture1.dds",L"data/chata.cobM"))
		{
			BOTHMSG("Problem with init mModel")
			return false;
		}
		mTextureShader = new TextureShaderClass();
		if(!mTextureShader)
		{
			BOTHMSG("Problem with creating GraphicClass::mTextureShader")
			return false;
		}
		if(!mTextureShader->Init(mDevice->getDevice(),hwnd))
		{
			BOTHMSG("Problem with init mTextureShader")
			return false;
		}
		mBitmap = new BitmapClass();
		if(!mBitmap)
		{
			BOTHMSG("Problem with creating GraphicClass::mBitmap")
			return false;
		}
		if(!mBitmap->Init(mDevice->getDevice(),"data/Tex1.gif",L"",XMFLOAT2(screenH,screenW),XMFLOAT2(256,256)))
		{
			BOTHMSG("Problem with init GraphicClass::mBitmap")
			return false;
		}
		
		mLightShader = new LightShaderClass();
		if(!mLightShader)
		{
			BOTHMSG("Problem with creating GraphicClass::mLightShader")
			return false;
		}
		if(!mLightShader->Init(mDevice->getDevice(),hwnd))
		{
			BOTHMSG("Problem with init mLightShader")
			return false;
		}
		mText = new TextClass();
		if(!mText->Initialize(mDevice->getDevice(), mDevice->getContext(), hwnd, screenH, screenW))
		{
			BOTHMSG("Problem with init mText");
			return false;
		}
		
		
		
		mLight = new LightClass();
		mLight->SetDiffuseColor(XMFLOAT4(1.0f,1.0f,1.0f, 0.0f));
		mLight->SetDirection(XMFLOAT3(1.0f,0.0f,1.0f));
		mLight->SetAmbientColor(XMFLOAT4(0.2f,0.2f,0.2f,0.0f));
		mLight->SetSpecularColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		mLight->SetSpecularPower(32.0f);
		InitBuffer();

		return true;
	}
	int GraphicClass::Clean()
	{
		if(mBitmap)
		{
			delete mBitmap;
			mBitmap = 0;
		}
		if(mLight)
		{
			delete mLight;
			mLight = 0;
		}
		if(mText)
		{
			mText->Shutdown();
			delete mText;
			mText = 0;
		}
		// Release the light shader object.
		if(mLightShader)
		{
			mLightShader->Shutdown();
			delete mLightShader;
			mLightShader = 0;
		}
		if(mFrameCB) {
			mFrameCB->Release();
			mFrameCB=0;
		}
		if(mSkinCB)
		{
			mSkinCB->Release();
			mSkinCB=0;
		}
		if(mStatCB)
		{
			mStatCB->Release();
			mStatCB=0;
		}
		if(mPassCB)
		{
			mPassCB->Release();
			mPassCB=0;
		}
		if(mMaterCB)
		{
			mMaterCB->Release();
			mMaterCB=0;
		}
		if(mDevice)
		{
			delete mDevice;
			mDevice = 0;
		}
		
		return true;
	}
	GraphicClass::~GraphicClass() {
		Clean();
	}

}