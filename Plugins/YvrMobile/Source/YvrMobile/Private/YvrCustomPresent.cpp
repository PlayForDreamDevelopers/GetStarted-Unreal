// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.

#include "YvrCustomPresent.h"
#include "YvrPluginWrapper.h"
#include "YvrMobile.h"

#include "ScreenRendering.h"
#include "PipelineStateCache.h"
#include "ClearQuad.h"
#include "CommonRenderResources.h"


FYvrCustomPresent::FYvrCustomPresent(class FYvrMobileHMD* YvrMobileHMD)
	:FXRRenderBridge(),
	YvrMobileHMD(YvrMobileHMD),
    bUsesArrayTexture(false),
    CurFFRLevel(0)
{
    static const FName RendererModuleName("Renderer");
    RendererModule = FModuleManager::GetModulePtr<IRendererModule>(RendererModuleName);

#if PLATFORM_ANDROID
    FFRParams[1] = FYvrMobilePlugin::GetPluginWrapper().GetFoveationParameter(0);
    FFRParams[2] = FYvrMobilePlugin::GetPluginWrapper().GetFoveationParameter(1);
    FFRParams[3] = FYvrMobilePlugin::GetPluginWrapper().GetFoveationParameter(2);
    FFRParams[4] = FYvrMobilePlugin::GetPluginWrapper().GetFoveationParameter(3);
#endif
}

bool FYvrCustomPresent::NeedsNativePresent()
{
	return true;
}

bool FYvrCustomPresent::Present(int32& SyncInterval)
{
	SyncInterval = 0;
	check(IsInRenderingThread() || IsInRHIThread());
	YvrMobileHMD->SubmitFrame();

	return false;
}

void FYvrCustomPresent::ShutDown()
{

}

bool FYvrCustomPresent::IsArrayTexture()
{
    return bUsesArrayTexture;
}

void FYvrCustomPresent::SetIsArrayTexture(const bool IsArrayTexture)
{
    bUsesArrayTexture = IsArrayTexture;
}

 FXRSwapChainPtr FYvrCustomPresent::CreateSwapChain_RenderThread(uint32 InSizeX, uint32 InSizeY, EPixelFormat InFormat, FClearValueBinding InBinding, uint32 InNumMips, uint32 InNumSamples, uint32 InNumSamplesTileMem, ERHIResourceType InResourceType, const TArray<uint32>& InTextures, ETextureCreateFlags InTexCreateFlags, const TCHAR* DebugName)
 {
 	check(IsInRenderingThread());

 	FTextureRHIRef RHITexture;
 	TArray<FTextureRHIRef> RHITextureSwapChain;
 	{
 		for (int32 TextureIndex = 0; TextureIndex < InTextures.Num(); ++TextureIndex)
 		{
 			FTextureRHIRef TexRef = CreateTexture_RenderThread(InSizeX, InSizeY, InFormat, InBinding, InNumMips, InNumSamples, InNumSamplesTileMem, InResourceType, InTextures[TextureIndex], InTexCreateFlags);

 			FString TexName = FString::Printf(TEXT("%s (%d/%d)"), DebugName, TextureIndex, InTextures.Num());
 			TexRef->SetName(*TexName);
 			RHIBindDebugLabelName(TexRef, *TexName);

 			RHITextureSwapChain.Add(TexRef);
 		}
 	}

 	RHITexture = GDynamicRHI->RHICreateAliasedTexture(RHITextureSwapChain[0]);

 	return CreateXRSwapChain(MoveTemp(RHITextureSwapChain), RHITexture);
 }

 void FYvrCustomPresent::CopyTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture* DstTexture, FRHITexture* SrcTexture,
	 FIntRect DstRect, FIntRect SrcRect, bool bAlphaPremultiply, bool bNoAlphaWrite, bool bInvertY, bool sRGBSource) const
 {
     check(IsInRenderingThread());

     FIntPoint SrcSize;
     FIntPoint DstSize;
     if (SrcTexture->GetTexture2D() && DstTexture->GetTexture2D()) {
         SrcSize = FIntPoint(SrcTexture->GetTexture2D()->GetSizeX(), SrcTexture->GetTexture2D()->GetSizeY());
         DstSize = FIntPoint(DstTexture->GetTexture2D()->GetSizeX(), DstTexture->GetTexture2D()->GetSizeY());
         UE_LOG(LogHMD, Log, TEXT("SrcSize %d %d DstSize %d %d"), SrcSize.X, SrcSize.Y, DstSize.X, DstSize.Y);
     }
     else {
         return;
     }

     if (SrcRect.IsEmpty()) {
         SrcRect = FIntRect(FIntPoint::ZeroValue, SrcSize);
     }

     if (DstRect.IsEmpty()) {
         DstRect = FIntRect(FIntPoint::ZeroValue, DstSize);
     }

     const uint32 viewportWidth = DstRect.Width();
     const uint32 viewportHeight = DstRect.Height();
     float U = SrcRect.Min.X / (float)SrcSize.X;
     float V = SrcRect.Min.Y / (float)SrcSize.Y;
     float USize = SrcRect.Width() / (float)SrcSize.X;
     float VSize = SrcRect.Height() / (float)SrcSize.Y;

	 if (bInvertY)
     {
		 V = 1.0f - V;
		 VSize = -VSize;
	 }

     FRHITexture* SrcTextureRHI = SrcTexture;
     RHICmdList.Transition(FRHITransitionInfo(SrcTextureRHI, ERHIAccess::Unknown, ERHIAccess::SRVGraphics));
     FGraphicsPipelineStateInitializer GraphicsPSOInit;

     if (bNoAlphaWrite) {
         GraphicsPSOInit.BlendState = TStaticBlendState<CW_RGB, BO_Add, BF_One, BF_Zero, BO_Add, BF_One, BF_Zero>::GetRHI();
     }
     else {
         GraphicsPSOInit.BlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_SourceAlpha, BF_Zero, BO_Add, BF_One, BF_Zero>::GetRHI();
     }

     GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
     GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
     GraphicsPSOInit.PrimitiveType = PT_TriangleList;

     const auto FeatureLevel = GMaxRHIFeatureLevel;
     auto ShaderMap = GetGlobalShaderMap(FeatureLevel);
     TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
     GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
     GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();

     if (DstTexture->GetTexture2D())
     {
         sRGBSource &= ((SrcTexture->GetFlags() & TexCreate_SRGB) != 0);
         uint32 NumMips = SrcTexture->GetNumMips();

         for (uint32 MipIndex = 0; MipIndex < NumMips; MipIndex++)
         {
             FRHIRenderPassInfo RPInfo(DstTexture, ERenderTargetActions::Load_Store);
             RPInfo.ColorRenderTargets[0].MipIndex = MipIndex;

             RHICmdList.BeginRenderPass(RPInfo, TEXT("CopyTexture"));
             {
                 const uint32 ViewportWidth = viewportWidth >> MipIndex;
                 const uint32 ViewportHeight = viewportHeight >> MipIndex;
                 const FIntPoint TargetSize(viewportWidth, viewportHeight);

                 if (bNoAlphaWrite)
                 {
                     RHICmdList.SetViewport(DstRect.Min.X, DstRect.Min.Y, 0.0f, DstRect.Max.X, DstRect.Max.Y, 1.0f);
                     DrawClearQuad(RHICmdList, FLinearColor::Black);
                 }

                 RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
                 FRHISamplerState* SamplerState = DstRect.Size() == SrcRect.Size() ? TStaticSamplerState<SF_Point>::GetRHI() : TStaticSamplerState<SF_Bilinear>::GetRHI();
                 if (!sRGBSource)
                 {
                     TShaderMapRef<FScreenPSMipLevel> PixelShader(ShaderMap);
                     GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
                     SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);
                     PixelShader->SetParameters(RHICmdList, SamplerState, SrcTextureRHI, MipIndex);
                 }
                 else {
                     TShaderMapRef<FScreenPSsRGBSourceMipLevel> PixelShader(ShaderMap);
                     GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
                     SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);
                     PixelShader->SetParameters(RHICmdList, SamplerState, SrcTextureRHI, MipIndex);
                 }
                 
                 RHICmdList.SetViewport(DstRect.Min.X, DstRect.Min.Y, 0.0f, DstRect.Min.X + ViewportWidth, DstRect.Min.Y + ViewportHeight, 1.0f);

                 UE_LOG(LogHMD, Log, TEXT("ViewportWidth %d ViewportHeight %d U %f V %f USize %f VSize %f TargetSize %d %d"), 
                     ViewportWidth, ViewportHeight, U, V, USize, VSize, TargetSize.X, TargetSize.Y);

                 RendererModule->DrawRectangle(
                     RHICmdList,
                     0, 0, ViewportWidth, ViewportHeight,
                     U, V, USize, VSize,
                     TargetSize,
                     FIntPoint(1, 1),
                     VertexShader,
                     EDRF_Default);
             }
             RHICmdList.EndRenderPass();
         }
     }
 }