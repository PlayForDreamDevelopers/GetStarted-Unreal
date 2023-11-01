// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.

#include "YvrHMDLayer.h"
#include "YvrMobile.h"
#include "YvrCustomPresent.h"

//#include "MediaTexture.h"
//#include "ScreenRendering.h"
//#include "ScenePrivate.h"
//#include "PostProcess/SceneFilterRendering.h"
#include "PostProcess/SceneRenderTargets.h"
#include "HeadMountedDisplayTypes.h" // for LogHMD
#include "XRThreadUtils.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/GameEngine.h"
#include "Engine/Public/SceneUtils.h"

#define YVR_TEXTURE_COUNT 3

//-------------------------------------------------------------------------------------------------
// FYvrLayer
//-------------------------------------------------------------------------------------------------

FYvrLayer::FYvrLayer(uint32 InId, const IStereoLayers::FLayerDesc& InDesc) :
	bNeedsTexSrgbCreate(false),
	Id(InId),
	bUpdateTexture(false),
	bInvertY(false),
	bHasDepth(false),
	PokeAHoleComponentPtr(nullptr),
	PokeAHoleActor(nullptr),
	YvrpLayer(nullptr)
{
#if PLATFORM_ANDROID
	LayerProjection = new yvrLayerProjection2();
	memset(LayerProjection, 0, sizeof(yvrLayerProjection2));
#endif
	SetDesc(InDesc);
	SetDefaultLayerParameters();
}


FYvrLayer::FYvrLayer(const FYvrLayer& Layer) :
	bNeedsTexSrgbCreate(Layer.bNeedsTexSrgbCreate),
	Id(Layer.Id),
	Desc(Layer.Desc),
	SwapChain(Layer.SwapChain),
	DepthSwapChain(Layer.DepthSwapChain),
	FoveationSwapChain(Layer.FoveationSwapChain),
	RightSwapChain(Layer.RightSwapChain),
	RightDepthSwapChain(Layer.RightDepthSwapChain),
#if PLATFORM_ANDROID
	LayerProjection(Layer.LayerProjection),
#endif
	bUpdateTexture(Layer.bUpdateTexture),
	bInvertY(Layer.bInvertY),
	bHasDepth(Layer.bHasDepth),
	bArrayTexture(Layer.bArrayTexture),
	PokeAHoleComponentPtr(Layer.PokeAHoleComponentPtr),
	PokeAHoleActor(Layer.PokeAHoleActor),
	YvrpLayer(Layer.YvrpLayer)
{
	SetDefaultLayerParameters();
}


FYvrLayer::~FYvrLayer()
{
}


void FYvrLayer::SetDesc(const IStereoLayers::FLayerDesc& InDesc)
{

	if (Desc.Texture != InDesc.Texture || Desc.LeftTexture != InDesc.LeftTexture)
	{
		bUpdateTexture = true;
	}

	Desc = InDesc;

	HandlePokeAHoleComponent();
}

void FYvrLayer::HandlePokeAHoleComponent()
{
	if (NeedsPokeAHole())
	{
		const FString BaseComponentName = FString::Printf(TEXT("YvrPokeAHole_%d"), Id);
		const FName ComponentName(*BaseComponentName);

		if (PokeAHoleComponentPtr == nullptr) {
			UWorld* World = nullptr;
			for (const FWorldContext& Context : GEngine->GetWorldContexts())
			{
				if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE)
				{
					World = Context.World();
				}
			}

			if (!World)
			{
				return;
			}

			PokeAHoleActor = World->SpawnActor<AActor>();

			PokeAHoleComponentPtr = NewObject<UProceduralMeshComponent>(PokeAHoleActor, ComponentName);
			PokeAHoleComponentPtr->RegisterComponent();

			TArray<FVector> Vertices;
			TArray<int32> Triangles;
			TArray<FVector> Normals;
			TArray<FVector2D> UV0;
			TArray<FLinearColor> VertexColors;
			TArray<FProcMeshTangent> Tangents;

			BuildPokeAHoleMesh(Vertices, Triangles, UV0);
			PokeAHoleComponentPtr->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, false);

			UMaterial* PokeAHoleMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/YvrMobile/Materials/PokeAHoleMaterial.PokeAHoleMaterial'")));

			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(PokeAHoleMaterial, NULL);
			PokeAHoleComponentPtr->SetMaterial(0, DynamicMaterial);
		}
		PokeAHoleComponentPtr->SetWorldTransform(Desc.Transform);

	}

	return;
}

void FYvrLayer::BuildPokeAHoleMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector2D>& UV0)
{
	if (Desc.HasShape<FQuadLayer>())
	{
		const float QuadScale = 0.99;

		FIntPoint TexSize = Desc.Texture.IsValid() ? Desc.Texture->GetTexture2D()->GetSizeXY() : Desc.LayerSize;
		float AspectRatio = TexSize.X ? (float)TexSize.Y / (float)TexSize.X : 3.0f / 4.0f;

		float QuadSizeX = Desc.QuadSize.X;
		float QuadSizeY = (Desc.Flags & IStereoLayers::LAYER_FLAG_QUAD_PRESERVE_TEX_RATIO) ? Desc.QuadSize.X * AspectRatio : Desc.QuadSize.Y;

		Vertices.Init(FVector::ZeroVector, 4);
		Vertices[0] = FVector(0.0, -QuadSizeX / 2, -QuadSizeY / 2) * QuadScale;
		Vertices[1] = FVector(0.0, QuadSizeX / 2, -QuadSizeY / 2) * QuadScale;
		Vertices[2] = FVector(0.0, QuadSizeX / 2, QuadSizeY / 2) * QuadScale;
		Vertices[3] = FVector(0.0, -QuadSizeX / 2, QuadSizeY / 2) * QuadScale;

		UV0.Init(FVector2D::ZeroVector, 4);
		UV0[0] = FVector2D(1, 0);
		UV0[1] = FVector2D(1, 1);
		UV0[2] = FVector2D(0, 0);
		UV0[3] = FVector2D(0, 1);

		Triangles.Reserve(6);
		Triangles.Add(0);
		Triangles.Add(1);
		Triangles.Add(2);
		Triangles.Add(0);
		Triangles.Add(2);
		Triangles.Add(3);
	}
}

TSharedPtr<FYvrLayer, ESPMode::ThreadSafe> FYvrLayer::Clone() const
{
	return MakeShareable(new FYvrLayer(*this));
}

bool FYvrLayer::CanReuseResources(const FYvrLayer* InLayer) const
{
#if PLATFORM_ANDROID
	if (InLayer && InLayer->YvrpLayer)
		return true;
#endif
	return false;
}

void FYvrLayer::Initialize_RenderThread(FYvrCustomPresent* CustomPresent, const FYvrLayer* InLayer)
{
#if PLATFORM_ANDROID
	check(IsInRenderingThread());

	uint32 SizeX = Desc.LayerSize.X;
	uint32 SizeY = Desc.LayerSize.Y;
	EPixelFormat ColorFormat = PF_R8G8B8A8;
	uint32 NumMips = 1;
	uint32 NumSamples = 1;
	uint32 NumSamplesTileMem = 1;
	uint32 ArraySize = 1;
	FClearValueBinding ColorTextureBinding = FClearValueBinding();
	ERHIResourceType ResourceType = RRT_Texture2D;
	ETextureCreateFlags ColorTexCreateFlags = TexCreate_ShaderResource | TexCreate_RenderTargetable | (IsMobileColorsRGB() ? TexCreate_SRGB : TexCreate_None);

	TArray<uint32> ColorTextures;
	ColorTextures.SetNum(YVR_TEXTURE_COUNT);
	bArrayTexture = CustomPresent->IsArrayTexture();
	static const auto CVarMobileMSAA = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.MobileMSAA"));
	NumSamplesTileMem = (CVarMobileMSAA ? CVarMobileMSAA->GetValueOnAnyThread() : 1);

	if (Id == 0)
	{
		if (bArrayTexture)
		{
			ResourceType = RRT_Texture2DArray;
			ArraySize = 2;
		}
	}
	else
	{
		bInvertY = true;
		check((Desc.Flags & IStereoLayers::LAYER_FLAG_HIDDEN) == 0);

		if (Desc.Texture.IsValid())
		{
			NumMips = Desc.Texture->GetNumMips();
			NumSamples = Desc.Texture->GetNumSamples();

			FRHITexture2D* Texture2D = Desc.Texture->GetTexture2D();
			FRHITextureCube* TextureCube = Desc.Texture->GetTextureCube();

			if (Texture2D)
			{
				SizeX = Texture2D->GetSizeX();
				SizeY = Texture2D->GetSizeY();
			}
			else if (TextureCube)
			{
				SizeX = SizeY = TextureCube->GetSize();
			}
		}
		else
		{
			SizeX = Desc.LayerSize.X;
			SizeY = Desc.LayerSize.Y;
		}

		if (SizeX == 0 || SizeY == 0)
			return;

		if (Desc.HasShape<FQuadLayer>())
		{
		}
		else
		{
			UE_LOG(LogHMD, Log, TEXT("Layer no quad shape"));
			return;
		}
	}

	if (CanReuseResources(InLayer))
	{
		Id = InLayer->Id;
		SwapChain = InLayer->SwapChain;
		DepthSwapChain = InLayer->DepthSwapChain;
		FoveationSwapChain = InLayer->FoveationSwapChain;
		RightSwapChain = InLayer->RightSwapChain;
		RightDepthSwapChain = InLayer->RightDepthSwapChain;
		bUpdateTexture = InLayer->bUpdateTexture;
		bNeedsTexSrgbCreate = InLayer->bNeedsTexSrgbCreate;
		YvrpLayer = InLayer->YvrpLayer;
		LayerProjection = InLayer->LayerProjection;
	}
	else
	{
		ExecuteOnRHIThread([&]()
		{
			yvrSwapChainCreateInfo CreateInfo;
			CreateInfo.Format = IsMobileColorsRGB();
			CreateInfo.Width = SizeX;
			CreateInfo.Height = SizeY;
			CreateInfo.BufferCount = YVR_TEXTURE_COUNT;
			CreateInfo.ArraySize = ArraySize;
			auto YvrSwapChain = FYvrMobilePlugin::GetPluginWrapper().CreateTextureSwapChain(&CreateInfo);
			YvrpLayer = MakeShareable<FYvrpLayer>(new FYvrpLayer(YvrSwapChain));

			for (int i = 0; i < YVR_TEXTURE_COUNT; ++i)
			{
				uint32 TextureID = 0;
				TextureID = FYvrMobilePlugin::GetPluginWrapper().GetTextureSwapChainHandle(YvrSwapChain, i);
				ColorTextures[i] = TextureID;
			}
		});

		SwapChain = CustomPresent->CreateSwapChain_RenderThread(SizeX, SizeY, ColorFormat, ColorTextureBinding, NumMips, NumSamples, NumSamplesTileMem, ResourceType, ColorTextures, ColorTexCreateFlags, *FString::Printf(TEXT("Yvr Color Swapchain %d"), Id));
	}
	bUpdateTexture = true;
#endif
}

void FYvrLayer::UpdateTexture_RenderThread(FYvrCustomPresent* CustomPresent, FRHICommandListImmediate& RHICmdList)
{
	check(IsInRenderingThread());
	check((Desc.Flags & IStereoLayers::LAYER_FLAG_HIDDEN) == 0);

	if (bUpdateTexture && SwapChain.IsValid())
	{
		// Copy textures
		if (Desc.Texture.IsValid())
		{
			bool bAlphaPremultiply = true;
			bool bNoAlphaWrite = (Desc.Flags & IStereoLayers::LAYER_FLAG_TEX_NO_ALPHA_CHANNEL) != 0;

			// Left
			{
				FRHITexture* SrcTexture = Desc.LeftTexture.IsValid() ? Desc.LeftTexture : Desc.Texture;
				FRHITexture* DstTexture = SwapChain->GetTexture();

				CustomPresent->CopyTexture_RenderThread(RHICmdList, DstTexture, SrcTexture, FIntRect(), FIntRect(), bAlphaPremultiply, bNoAlphaWrite, bInvertY);
			}

			// Right
			if (RightSwapChain.IsValid())
			{
				FRHITexture* SrcTexture = Desc.Texture;
				FRHITexture* DstTexture =  RightSwapChain->GetTexture();

				CustomPresent->CopyTexture_RenderThread(RHICmdList, DstTexture, SrcTexture, FIntRect(), FIntRect(), false, false, false);
			}

			if (!(Desc.Flags & IStereoLayers::LAYER_FLAG_TEX_CONTINUOUS_UPDATE))
			{
				bUpdateTexture = false;
			}
		}

		// Generate mips
		SwapChain->GenerateMips_RenderThread(RHICmdList);

		if (RightSwapChain.IsValid())
		{
			RightSwapChain->GenerateMips_RenderThread(RHICmdList);
		}
	}

}

#if PLATFORM_ANDROID
yvrMatrix4f FYvrLayer::GenerateTextureCoords(const yvrMatrix4f& invViewModelMatrix)
{
	yvrMatrix4f projectionMatrix;
	const yvrMatrix4f& inv = invViewModelMatrix;

	FVector4 Fov = FYvrMobilePlugin::GetPluginWrapper().GetEyeFov(0);
	float ProjectionCompensate = FPlatformMath::Tan(FMath::DegreesToRadians(Fov.X));

	projectionMatrix.M[0][0] = 0.5f * ProjectionCompensate * ((inv.M[0][0] * 2.0f * inv.M[2][3] - inv.M[0][3] * 2.0f * inv.M[2][0]) - inv.M[2][0]);
	projectionMatrix.M[0][1] = 0.5f * ProjectionCompensate * ((inv.M[0][1] * 2.0f * inv.M[2][3] - inv.M[0][3] * 2.0f * inv.M[2][1]) - inv.M[2][1]);
	projectionMatrix.M[0][2] = 0.5f * ((inv.M[0][2] * 2.0f * inv.M[2][3] - inv.M[0][3] * 2.0f * inv.M[2][2]) - inv.M[2][2]);
	projectionMatrix.M[0][3] = 0.0f;

	projectionMatrix.M[1][0] = 0.5f * ProjectionCompensate * ((inv.M[1][0] * 2.0f * inv.M[2][3] - inv.M[1][3] * 2.0f * inv.M[2][0]) - inv.M[2][0]);
	projectionMatrix.M[1][1] = 0.5f * ProjectionCompensate * ((inv.M[1][1] * 2.0f * inv.M[2][3] - inv.M[1][3] * 2.0f * inv.M[2][1]) - inv.M[2][1]);
	projectionMatrix.M[1][2] = 0.5f * ((inv.M[1][2] * 2.0f * inv.M[2][3] - inv.M[1][3] * 2.0f * inv.M[2][2]) - inv.M[2][2]);
	projectionMatrix.M[1][3] = 0.0f;

	projectionMatrix.M[2][0] = -ProjectionCompensate * (inv.M[2][0]);
	projectionMatrix.M[2][1] = -ProjectionCompensate * (inv.M[2][1]);
	projectionMatrix.M[2][2] = -(inv.M[2][2]);
	projectionMatrix.M[2][3] = 0.0f;

	projectionMatrix.M[3][0] = 0.0f;
	projectionMatrix.M[3][1] = 0.0f;
	projectionMatrix.M[3][2] = 0.0f;
	projectionMatrix.M[3][3] = 1.0f;
	return projectionMatrix;
}

yvrLayerProjection2* FYvrLayer::UpdateLayer_RHIThread(FYvrCustomPresent* CustomPresent, const FMatrix& PlayerTrasform, const FTransform& CurrentTrackingToWorld, const UYvrHMDSettings* Settings)
{
	for (int i = 0; i < 2; ++i)
	{
		LayerProjection->mTextures[i].VertexCoord[0] = { -1.0f, -1.0f, 0.0f, 1.0f };
		LayerProjection->mTextures[i].VertexCoord[1] = {  1.0f, -1.0f, 0.0f, 1.0f };
		LayerProjection->mTextures[i].VertexCoord[2] = { -1.0f,  1.0f, 0.0f, 1.0f };
		LayerProjection->mTextures[i].VertexCoord[3] = {  1.0f,  1.0f, 0.0f, 1.0f };

		LayerProjection->mTextures[i].TextureCoord[0] = { 0.0f, 0.0f };
		LayerProjection->mTextures[i].TextureCoord[1] = { 1.0f, 0.0f };
		LayerProjection->mTextures[i].TextureCoord[2] = { 0.0f, 1.0f };
		LayerProjection->mTextures[i].TextureCoord[3] = { 1.0f, 1.0f };

		LayerProjection->mTextures[i].eyeMask = i ? kEyeMaskRight : kEyeMaskLeft;
		LayerProjection->mTextures[i].imageType = kTypeTexture;
		LayerProjection->mTextures[i].layerFlags = 0;

		LayerProjection->mTextures[i].ColorScale = { 1.0f, 1.0f, 1.0f, 1.0f };
		LayerProjection->mTextures[i].ColorOffset = { 0.0f, 0.0f, 0.0f, 0.0f };

		if (Id == 0)
		{
			if (bArrayTexture)
			{
				LayerProjection->mTextures[i].imageType = kTypeTextureArray;
				LayerProjection->mTextures[i].TexCoordsMatrix = yvrMatrix4f();
			}
			else
			{
				LayerProjection->mTextures[i].TexCoordsMatrix = i ? RightMatrix : LeftMatrix;
			}

			LayerProjection->mTextures[i].ColorScale = FYvrMobileHMD::LinearColorToYvrVector4f(Settings->ColorScale);
			LayerProjection->mTextures[i].ColorOffset = FYvrMobileHMD::LinearColorToYvrVector4f(Settings->ColorOffset);

			uint32 TextureId = *(uint32*)SwapChain->GetTexture()->GetNativeResource();
			CustomPresent->SetFoveatedData(TextureId, (uint32)Settings->FFRLevel);
		}
		else
		{
			FTransform TrackingToWorld;
			FMatrix ViewTransform;
			switch (Desc.PositionType)
			{
			case IStereoLayers::WorldLocked:
				TrackingToWorld = CurrentTrackingToWorld;
				ViewTransform = PlayerTrasform;
				break;

			case IStereoLayers::TrackerLocked:
				TrackingToWorld = FTransform::Identity;
				ViewTransform = FMatrix::Identity;
				break;

			case IStereoLayers::FaceLocked:
				TrackingToWorld = FTransform::Identity;
				ViewTransform = FMatrix::Identity;
				break;
			}

			FIntPoint TextureSize = Desc.Texture.IsValid() ? Desc.Texture->GetTexture2D()->GetSizeXY() : Desc.LayerSize;
			float AspectRatio = TextureSize.X ? (float)TextureSize.Y / (float)TextureSize.X : 3.0f / 4.0f;
			float QuadSizeX = Desc.QuadSize.X;
			float QuadSizeY = (Desc.Flags & IStereoLayers::LAYER_FLAG_QUAD_PRESERVE_TEX_RATIO) ? QuadSizeX * AspectRatio : Desc.QuadSize.Y;

			FVector QuadScale = Desc.Transform.GetScale3D();
			FVector QuadSize = FVector(-GEngine->XRSystem->GetWorldToMetersScale(), QuadSizeX, QuadSizeY);
			FVector Scale = QuadScale * QuadSize;
			FQuat Orientation = Desc.Transform.Rotator().Quaternion();
			FVector Location = Desc.Transform.GetLocation();

			Location = TrackingToWorld.InverseTransformPosition(Location) / GEngine->XRSystem->GetWorldToMetersScale();
			Orientation = TrackingToWorld.InverseTransformRotation(Orientation);
			Scale = Scale / GEngine->XRSystem->GetWorldToMetersScale();

			Scale = FVector(Scale.Y, Scale.Z, -Scale.X);
			Orientation = FQuat(-Orientation.Y, -Orientation.Z, Orientation.X, Orientation.W);
			Location = FVector(Location.Y, Location.Z, -Location.X);

			float IPD = FYvrMobilePlugin::GetPluginWrapper().GetIPD();
			float HalfIpd = i ? IPD / 2.0f : -IPD / 2.0f;
			FMatrix EyeOffset = FTranslationMatrix(FVector(HalfIpd, 0.0f, 0.0f));

			FMatrix Model = FScaleRotationTranslationMatrix(Scale, Orientation.Rotator(), Location);
			FMatrix InverseView = EyeOffset * ViewTransform;
			FMatrix InverseModelView = InverseView * Model.Inverse();
			LayerProjection->mTextures[i].TexCoordsMatrix = GenerateTextureCoords(FYvrMobileHMD::ToYvrMatrix(InverseModelView));

			if (Settings->bApplyColorScaleAndOffsetToAllLayers)
			{
				LayerProjection->mTextures[i].ColorScale = FYvrMobileHMD::LinearColorToYvrVector4f(Settings->ColorScale);
				LayerProjection->mTextures[i].ColorOffset = FYvrMobileHMD::LinearColorToYvrVector4f(Settings->ColorOffset);
			}
		}

		LayerProjection->mTextures[i].SwapChainIndex = SwapChain.IsValid() ? SwapChain->GetSwapChainIndex_RHIThread() : 0;
		LayerProjection->mTextures[i].ColorSwapChain = YvrpLayer->TextureSwapChain;
	}
	return LayerProjection;
}
#endif

void FYvrLayer::IncrementSwapChainIndex_RHIThread(FYvrCustomPresent* CustomPresent)
{
	check((Desc.Flags & IStereoLayers::LAYER_FLAG_HIDDEN) == 0);

	if (SwapChain.IsValid())
	{
		SwapChain->IncrementSwapChainIndex_RHIThread();
	}
}

void FYvrLayer::ReleaseResources_RHIThread()
{
#if PLATFORM_ANDROID
	check(IsInRHIThread());

	Id = 0;
	SwapChain.Reset();
	DepthSwapChain.Reset();
	FoveationSwapChain.Reset();
	RightSwapChain.Reset();
	RightDepthSwapChain.Reset();
	delete LayerProjection;
	LayerProjection = nullptr;
	bUpdateTexture = false;
#endif
}

void FYvrLayer::SetDefaultLayerParameters()
{
#if PLATFORM_ANDROID
	LeftMatrix =
	{{
		{0.25f, 0.0f, -0.25f, 0.0f},
		{0.0f,  0.5f, -0.5f,  0.0f},
		{0.0f,  0.0f, -1.0f,  0.0f},
		{0.0f,  0.0f, -1.0f,  0.0f}
	}};

	RightMatrix =
	{{
		{0.25f, 0.0f, -0.75f, 0.0f},
		{0.0f,  0.5f, -0.5f,  0.0f},
		{0.0f,  0.0f, -1.0f,  0.0f},
		{0.0f,  0.0f, -1.0f,  0.0f}
	}};
#endif
}

#if PLATFORM_ANDROID
FYvrpLayer::FYvrpLayer(yvrTextureSwapChain* SwapChain)
{
	TextureSwapChain = SwapChain;
}

FYvrpLayer::~FYvrpLayer()
{
	if (IsInGameThread())
	{
		ExecuteOnRenderThread([this]()
		{
			ExecuteOnRHIThread_DoNotWait([this]()
			{
				if (TextureSwapChain != nullptr)
				{
					FYvrMobilePlugin::GetPluginWrapper().DestroyTextureSwapChain(TextureSwapChain);
					TextureSwapChain = nullptr;
				}
			});
		});
	}
	else
	{
		ExecuteOnRHIThread_DoNotWait([this]()
		{
			if (TextureSwapChain != nullptr)
			{
				FYvrMobilePlugin::GetPluginWrapper().DestroyTextureSwapChain(TextureSwapChain);
				TextureSwapChain = nullptr;
			}
		});
	}
}
#endif