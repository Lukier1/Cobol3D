#include "GraphicClass.h"
#include "Logger.h"
#include "MyMath.h"

#include <string>


#define _CRT_NONSTDC_NO_WARNINGS
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
		mTex = 0;
		mMask = 0;
		mMultiTexShader = nullptr;
	};

	/*Funkcja zajmujaca sie tworzenie jednego buforu*/
	int GraphicClass::CreateBuffer(ID3D11Buffer * &buffer, UINT size, bool dynamic)
	{
		D3D11_BUFFER_DESC constDesc;
		HRESULT result;

		ZeroMemory(&constDesc, sizeof(constDesc));
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = size;
		constDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT ;
		constDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;

		result = mDevice->getDevice()->CreateBuffer(&constDesc, 0, &buffer);
		if (FAILED(result))
		{
			LOG_MSGERR("Problem with creating buffer.")
			return false;
		}
		return true;
	}

	/*Inicjalizacja wszystkich buforów*/
	int GraphicClass::InitBuffer() {
		if (!CreateBuffer(mFrameCB, sizeof(VSPerFrameCB), true))
			return false;
		if (!CreateBuffer(mSkinCB, sizeof(VSPerSkinedCB), true))
			return false;
		if (!CreateBuffer(mStatCB, sizeof(VSPerStaticCB), true))
			return false;
		if (!CreateBuffer(mPassCB, sizeof(VSPerPassCB), true))
			return false;
		if (!CreateBuffer(mMaterCB, sizeof(VSPerMaterialCB), true))
			return false;
		return true;
	}
	/* Aktulizacja sta³ego bufora o warstoœci przekazane przez wskaŸnik data_new */
	void GraphicClass::UpdateBuffer(ID3D11Buffer * &buffer, int num_reg, void * data_new, int size) {
		if (data_new == NULL || buffer == NULL)
		{
			BOTHMSG("Null pointer error in update buffer")
		}
		ID3D11DeviceContext * devCon = mDevice->getContext();
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		void* dataPtr;
		devCon->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		// Get a pointer to the data in the constant buffer.
		dataPtr = mappedResource.pData;
		// Copy the matrices into the constant buffer.
		memcpy(dataPtr, data_new, size);
		// Unlock the constant buffer.
		//Jon
		devCon->Unmap(buffer, 0);
		devCon->VSSetConstantBuffers(num_reg, 1, &buffer);
	}

	void GraphicClass::Frame(InputData &input)
	{
		static bool frustum = true;
		if (mDevice)
		{
			ID3D11DeviceContext * devCon = mDevice->getContext();

			//UPDATING BUFFERS
			XMMATRIX viewMatrix, worldMatrix = XMMatrixIdentity(), projectionMatrix, orthoMatrix;

			static float rotation = 0.0f;


			// Update the rotation variable each frame.
			rotation += (float)XM_PI * 0.0008f;
			if (rotation > 2 * XM_PI)
			{
				rotation -= 2 * XM_PI;
			}
			XMMATRIX mSpin = XMMatrixRotationZ(rotation);
			XMMATRIX mOrbit = XMMatrixRotationY(rotation);
			XMMATRIX mTranslate = XMMatrixTranslation(-0.0f, 0.0f, -1.0f);
			XMMATRIX mScale = XMMatrixScaling(0.8f, 0.8f, 0.8f);


			worldMatrix = mScale * mSpin * mTranslate *mOrbit;

			// Get the world, view, and projection matrices from the camera and d3d objects.
			mCamera->getMatrix(viewMatrix);
			mDevice->GetProjectionMatrix(projectionMatrix);
			mDevice->GetOrthoMatrix(orthoMatrix);
			mFrustum->ConstructFrustum(1000.0f, projectionMatrix, viewMatrix);


			TransposeMatrix(worldMatrix);
			TransposeMatrix(viewMatrix);
			TransposeMatrix(projectionMatrix);
			TransposeMatrix(orthoMatrix);


			//Ustawianie buforów do renderowania 3D
			{
				VSPerFrameCB data;
				data.appTime = (float)(std::clock() - beginTime);
				data.projMatrix = projectionMatrix;

				UpdateBuffer(mFrameCB, 0, &data, sizeof(data));
			}
			devCon->VSSetConstantBuffers(1, 1, &mSkinCB);
			{
				VSPerStaticCB  data;
				data.worldMatrix = worldMatrix;

				UpdateBuffer(mStatCB, 2, &data, sizeof(data));
			}

			{
				VSPerPassCB  data;
				data.viewMatrix = viewMatrix;
				data.cameraPosition = mCamera->GetPosition();
				data.padding = 0.0f;

				UpdateBuffer(mPassCB, 3, &data, sizeof(data));
			}


			devCon->VSSetConstantBuffers(4, 1, &mMaterCB);

			//############################################################# Poczatek Sceny
			mDevice->BeginScene(0.0f, 0, 0.2f, 1.0f);

			int renderCount = 0;
			for (int i = 0, modelCount = mModelList->GetModelCount(); i <modelCount; ++i) {
				// Render the model using the color shader.
				XMFLOAT4 color;
				XMFLOAT3 pos;
				mModelList->GetDate(i, pos, color);

				if (!frustum && !mFrustum->CheckSphere(pos.x, pos.y, pos.z, 2.0f))
					continue;
				++renderCount;
				TransposeMatrix(worldMatrix);
				worldMatrix *= XMMatrixTranslation(pos.x, pos.y, pos.z);
				TransposeMatrix(worldMatrix);
				devCon->VSSetConstantBuffers(1, 1, &mSkinCB);
				{
					VSPerStaticCB  data;
					data.worldMatrix = worldMatrix;

					UpdateBuffer(mStatCB, 2, &data, sizeof(data));
				}

				mModel2->Render(mDevice->getContext());
				mLightShader->Render(mDevice->getContext(), mModel2->GetIndexCount(), mModel->getTexture(),
					mLight->GetDirection(), color, mLight->GettAmbientColor(), mLight->GetSpecularColor(), mLight->GetSpecularPower());
				worldMatrix = mScale * mSpin * mTranslate *mOrbit;
				TransposeMatrix(worldMatrix);
			}
			mCamera->Render();

			//Changing on ortho matrix
			{
				VSPerFrameCB data;
				data.appTime = (float)(std::clock() - beginTime);
				data.projMatrix = orthoMatrix;
				UpdateBuffer(mFrameCB, 0, &data, sizeof(data));
			}
			devCon->VSSetConstantBuffers(1, 1, &mSkinCB);
			{
				VSPerStaticCB data;
				XMMATRIX mWorld2 = XMMatrixRotationX(input.appTime);
				TransposeMatrix(mWorld2);
				data.worldMatrix = XMMatrixIdentity();
				UpdateBuffer(mStatCB, 2, &data, sizeof(data));
			}
			{
				VSPerPassCB data;
				data.viewMatrix = XMMatrixIdentity();
				data.cameraPosition = mCamera->GetPosition();
				data.padding = 0.0f;
				UpdateBuffer(mPassCB, 3, &data, sizeof(data));
			}

			mDevice->SetBufferZ(false);

			mText->Render(devCon);
			mBitmap->Render(devCon, 0.0f, 0.0f);
			ID3D11ShaderResourceView * tex[] = { mBitmap->getTexture(), mTex->getTexture(), mMask->getTexture() };
			mMultiTexShader->Render(devCon,mBitmap->GetIndexCount(), tex);
			
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
					if (input.litera == 'f')
						frustum = !frustum;
			}
			if (input.IX != 0 || input.IY != 0)
			{
				static float rotY = 0.0f, rotZ  = 0.0f;
				rotY += (float)(-input.IX)/12;
				rotZ += (float)(-input.IY) / 12;
				mCamera->SetRotation(XMFLOAT3(0, rotY, 0.0f));

			}
			
			
			wyjscie+=toString(input.MouseX)+" MouseY: "+toString(input.MouseY)+" Render count: "+toString(renderCount);
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
		if(!mModel->Init(mDevice->getDevice(),"data/Texture1.dds",L"data/import.cobM"))
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
		if(!mModel2->Init(mDevice->getDevice(),"data/Texture1.dds",L"data/import2.cobM"))
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
		if(!mBitmap->Init(mDevice->getDevice(),"data/Brick.gif",L"",XMFLOAT2(screenH,screenW),XMFLOAT2(256,256)))
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
		mMultiTexShader = new MultiTexShaderClass();
		if (!mMultiTexShader)
		{
			BOTHMSG("Problem with creating GraphicClass::mMultiTexShader")
			return false;
		}
		if (!mMultiTexShader->Init(mDevice->getDevice(), hwnd))
		{
			BOTHMSG("Problem with init mMultiTexShader")
			return false;
		}
		mTex = new TextureClass();
		if (!mTex)
		{
			BOTHMSG("Problem with creating GraphicClass::mTex")
				return false;
		}

		if (!mTex->Init(mDevice->getDevice(), "data/Grass.gif"))
		{
			BOTHMSG("Problem with init GraphicClass::mTex")
			return false;
		}
		mMask = new TextureClass();
		if (!mMask)
		{
			BOTHMSG("Problem with creating GraphicClass::mMask")
				return false;
		}

		if (!mMask->Init(mDevice->getDevice(), "data/alpha.gif"))
		{
			BOTHMSG("Problem with init GraphicClass::mMask")
				return false;
		}
		mText = new TextClass();
		if(!mText->Initialize(mDevice->getDevice(), mDevice->getContext(), hwnd, screenH, screenW))
		{
			BOTHMSG("Problem with init mText");
			return false;
		}
		mModelList = new ModelListClass();
		if (!mModelList->Init(50)) {
			BOTHMSG("Problem with init mModelList");
			return false;
		}
		mFrustum = new FrustumClass();
		
		
		mLight = new LightClass();
		mLight->SetDiffuseColor(XMFLOAT4(1.0f,1.0f,1.0f, 0.0f));
		mLight->SetDirection(XMFLOAT3(1.0f,0.0f,1.0f));
		mLight->SetAmbientColor(XMFLOAT4(0.2f,0.2f,0.2f,0.0f));
		mLight->SetSpecularColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		mLight->SetSpecularPower(32.0f);
		if(!InitBuffer())
		if (!mLightShader->Init(mDevice->getDevice(), hwnd))
		{
			BOTHMSG("Problem with init buffers");
			return false;
		}

		return true;
	}
	int GraphicClass::Clean()
	{
		//ReportLiveObjects();
		
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
		delete mFrustum;
		mFrustum = 0;

		delete mModelList;
		mModelList = 0;

		delete mMultiTexShader;
		mMultiTexShader = 0;
		delete mTex;
		mTex = 0;
		delete mMask;
		mMask = 0;
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