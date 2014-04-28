#ifndef __TEXTCLASS_H__
#define __TEXTCLASS_H__

#include "FontClass.h"
#include "FontShaderClass.h"
namespace Cobol {
	class TextClass
	{
	private:

		struct SentenceType
		{
			ID3D11Buffer *vertexBuffer, *indexBuffer;
			int vertexCount, indexCount, maxLength;
			float red, green, blue;
		};

		struct VertexType
		{
			XMFLOAT3 position;
			XMFLOAT2 texture;
		};

	public:
		TextClass();
		TextClass(const TextClass&);
		~TextClass();

		bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int);
		void Shutdown();
		bool Render(ID3D11DeviceContext*);

		void UpdateText(const char * text,ID3D11DeviceContext * devCon);
		void UpdateText2(const char * text,ID3D11DeviceContext * devCon);
	private:
		bool InitializeSentence(SentenceType**, int, ID3D11Device*);
		bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
		void ReleaseSentence(SentenceType**);
		bool RenderSentence(ID3D11DeviceContext*, SentenceType*);

	private:
		FontClass* mFont;
		FontShaderClass* mFontShader;
		int mScreenWidth, mScreenHeight;
		XMMATRIX mBaseViewMatrix;


		SentenceType* mSentence1;
		SentenceType* mSentence2;

		long long int mLastTime;
		
	};
}
#endif