// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.

#pragma once

//#include "YvrMobile.h"

#include "XRRenderBridge.h"
#include "XRSwapChain.h"

#include "RendererInterface.h"

class FYvrMobileHMD;
class FYvrCustomPresent :	public FXRRenderBridge
{
public:
	FYvrCustomPresent(class FYvrMobileHMD* YvrMobileHMD);
	// FXRRenderBridge/FRHICustomPresent
	virtual bool NeedsNativePresent() override;
	virtual bool Present(int32& SyncInterval) override;
	virtual ~FYvrCustomPresent() {}
	void SubmitFrame();
	void ShutDown();

	bool IsArrayTexture();
	void SetIsArrayTexture(const bool IsArrayTexture);

	virtual FTextureRHIRef CreateTexture_RenderThread(uint32 InSizeX, uint32 InSizeY, EPixelFormat InFormat, FClearValueBinding InBinding, uint32 InNumMips, uint32 InNumSamples, uint32 InNumSamplesTileMem, ERHIResourceType InResourceType, uint32 InTexture, ETextureCreateFlags TexCreateFlags) = 0;
	FXRSwapChainPtr CreateSwapChain_RenderThread(uint32 InSizeX, uint32 InSizeY, EPixelFormat InFormat, FClearValueBinding InBinding, uint32 InNumMips, uint32 InNumSamples, uint32 InNumSamplesTileMem, ERHIResourceType InResourceType, const TArray<uint32>& InTextures, ETextureCreateFlags InTexCreateFlags, const TCHAR* DebugName);
	void CopyTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture* DstTexture, FRHITexture* SrcTexture, FIntRect DstRect = FIntRect(), FIntRect SrcRect = FIntRect(), bool bAlphaPremultiply = false, bool bNoAlphaWrite = false, bool bInvertY = true, bool sRGBSource = false) const;

	virtual void SetFoveatedData(const uint32 TextureId, const uint32 Level) = 0;

protected:
	FYvrMobileHMD* YvrMobileHMD;
	IRendererModule* RendererModule;
	bool bUsesArrayTexture;
	FVector4 FFRParams[5];
	uint32 CurFFRLevel;
};

FYvrCustomPresent* CreateCustomPresent_OpenGL(FYvrMobileHMD* InYvrHMD);

