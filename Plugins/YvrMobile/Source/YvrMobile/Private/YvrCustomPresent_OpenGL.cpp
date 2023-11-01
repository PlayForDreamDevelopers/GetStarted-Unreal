// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.

#include "YvrCustomPresent.h"
#include "YvrMobile.h"

#include "OpenGLDrvPrivate.h"
#include "OpenGLResources.h"

#if PLATFORM_ANDROID
#include "Android/AndroidOpenGL.h"
#endif

typedef void(*PFNGLTEXTUREFOVEATIONPARAMETERSEXT)(GLuint texture, GLuint layer, GLuint focalPoint, GLfloat focalX, GLfloat focalY, GLfloat gainX, GLfloat gainY, GLfloat foveaArea);
PFNGLTEXTUREFOVEATIONPARAMETERSEXT glTextureFoveationParametersQCOM = NULL;

class FYvrOpenGLCustomPresent : public FYvrCustomPresent
{
public:
	FYvrOpenGLCustomPresent(FYvrMobileHMD* InYvrHMD);

	virtual FTextureRHIRef CreateTexture_RenderThread(uint32 InSizeX, uint32 InSizeY, EPixelFormat InFormat, FClearValueBinding InBinding, uint32 InNumMips, uint32 InNumSamples, uint32 InNumSamplesTileMem, ERHIResourceType InResourceType, uint32 InTexture, ETextureCreateFlags InTexCreateFlags) override;

	virtual void SetFoveatedData(const uint32 TextureId, const uint32 Level) override;
};


FYvrOpenGLCustomPresent::FYvrOpenGLCustomPresent(FYvrMobileHMD* InYvrHMD) :
	FYvrCustomPresent(InYvrHMD)
{
#if PLATFORM_ANDROID
	glTextureFoveationParametersQCOM = (PFNGLTEXTUREFOVEATIONPARAMETERSEXT)eglGetProcAddress("glTextureFoveationParametersQCOM");
#endif
}

FTextureRHIRef FYvrOpenGLCustomPresent::CreateTexture_RenderThread(uint32 InSizeX, uint32 InSizeY, EPixelFormat InFormat, FClearValueBinding InBinding, uint32 InNumMips, uint32 InNumSamples, uint32 InNumSamplesTileMem, ERHIResourceType InResourceType, uint32 InTexture, ETextureCreateFlags InTexCreateFlags)
{
	check(IsInRenderingThread());

	FOpenGLDynamicRHI* DynamicRHI = static_cast<FOpenGLDynamicRHI*>(GDynamicRHI);

	switch (InResourceType)
	{
	case RRT_Texture2D:
		return DynamicRHI->RHICreateTexture2DFromResource(InFormat, InSizeX, InSizeY, InNumMips, InNumSamples, InNumSamplesTileMem, InBinding, (GLuint)InTexture, InTexCreateFlags).GetReference();

	case RRT_Texture2DArray:
		return DynamicRHI->RHICreateTexture2DArrayFromResource(InFormat, InSizeX, InSizeY, 2, InNumMips, InNumSamples, InNumSamplesTileMem, InBinding, (GLuint)InTexture, InTexCreateFlags).GetReference();

	case RRT_TextureCube:
		return DynamicRHI->RHICreateTextureCubeFromResource(InFormat, InSizeX, false, 1, InNumMips, InNumSamples, InNumSamplesTileMem, InBinding, (GLuint)InTexture, InTexCreateFlags).GetReference();

	default:
		return nullptr;
	}
}

void FYvrOpenGLCustomPresent::SetFoveatedData(const uint32 TextureId, const uint32 Level)
{
#if PLATFORM_ANDROID
	if (CurFFRLevel != Level)
	{
		GLuint TextureHandler = 0;
		FVector4 FFRValues = FFRParams[Level];
		FVector2D Gain = FVector2D(FFRValues[0], FFRValues[1]);
		float Area = FFRValues[2];
		float Minimum = FFRValues[3];

		if (bUsesArrayTexture)
		{
			GLint CurrentBindingTexture = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &CurrentBindingTexture);
			glBindTexture(GL_TEXTURE_2D_ARRAY, TextureId);

			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_FOVEATED_FEATURE_BITS_QCOM, GL_FOVEATION_ENABLE_BIT_QCOM | GL_FOVEATION_SCALED_BIN_METHOD_BIT_QCOM);
			glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_FOVEATED_MIN_PIXEL_DENSITY_QCOM, Minimum);

			glTextureFoveationParametersQCOM(TextureId, 0, 0, 0.0f, 0.0f, Gain.X, Gain.Y, Area);
			glTextureFoveationParametersQCOM(TextureId, 1, 0, 0.0f, 0.0f, Gain.X, Gain.Y, Area);

			glBindTexture(GL_TEXTURE_2D_ARRAY, CurrentBindingTexture);
			glFlush();
		}
		else
		{
			GLint CurrentBindingTexture = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &CurrentBindingTexture);
			glBindTexture(GL_TEXTURE_2D, TextureId);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_FOVEATED_FEATURE_BITS_QCOM, GL_FOVEATION_ENABLE_BIT_QCOM | GL_FOVEATION_SCALED_BIN_METHOD_BIT_QCOM);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_FOVEATED_MIN_PIXEL_DENSITY_QCOM, Minimum);

			glTextureFoveationParametersQCOM(TextureId, 0, 0, -0.5f, 0.0f, Gain.X * 2.0f, Gain.Y, Area);
			glTextureFoveationParametersQCOM(TextureId, 0, 1, 0.5f, 0.0f, Gain.X * 2.0f, Gain.Y, Area);

			glBindTexture(GL_TEXTURE_2D, CurrentBindingTexture);
			glFlush();
		}
		CurFFRLevel = Level;
	}
#endif
}

FYvrCustomPresent* CreateCustomPresent_OpenGL(FYvrMobileHMD* InYvrHMD)
{
	return new FYvrOpenGLCustomPresent(InYvrHMD);
}