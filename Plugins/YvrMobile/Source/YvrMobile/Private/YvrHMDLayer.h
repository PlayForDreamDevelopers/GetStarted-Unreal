// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.

#pragma once
#include "YvrCustomPresent.h"
#include "YvrPluginWrapper.h"

#include "ProceduralMeshComponent.h"
#include "IStereoLayers.h"
#include "XRSwapChain.h"

class FYvrpLayer : public TSharedFromThis<FYvrpLayer, ESPMode::ThreadSafe>
{
public:
#if PLATFORM_ANDROID
	FYvrpLayer(yvrTextureSwapChain* SwapChain);
	~FYvrpLayer();

	yvrTextureSwapChain* TextureSwapChain;
#endif
};

typedef TSharedPtr<FYvrpLayer, ESPMode::ThreadSafe> FYvrpLayerPtr;

//-------------------------------------------------------------------------------------------------
// FYvrLayer
//-------------------------------------------------------------------------------------------------
class UYvrHMDSettings;
class FYvrLayer : public TSharedFromThis<FYvrLayer, ESPMode::ThreadSafe>
{
public:
	FYvrLayer(uint32 InId, const IStereoLayers::FLayerDesc& InDesc);
	FYvrLayer(const FYvrLayer& InLayer);
	~FYvrLayer();

	uint32 GetId() const { return Id; }
	void SetDesc(const IStereoLayers::FLayerDesc& InDesc);
	const IStereoLayers::FLayerDesc& GetDesc() const { return Desc; }
	const FXRSwapChainPtr& GetSwapChain() const { return SwapChain; }
	const FXRSwapChainPtr& GetRightSwapChain() const { return RightSwapChain; }
	const FXRSwapChainPtr& GetDepthSwapChain() const { return DepthSwapChain; }
	const FXRSwapChainPtr& GetFoveationSwapChain() const { return FoveationSwapChain; }
	void MarkTextureForUpdate() { bUpdateTexture = true; }

	bool NeedsPokeAHole() { return (Desc.Flags & IStereoLayers::LAYER_FLAG_SUPPORT_DEPTH) != 0; }
	void HandlePokeAHoleComponent();
	void BuildPokeAHoleMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector2D>& UV0);

	FTextureRHIRef GetTexture() { return Desc.Texture; }

	TSharedPtr<FYvrLayer, ESPMode::ThreadSafe> Clone() const;

	bool CanReuseResources(const FYvrLayer* InLayer) const;
	void Initialize_RenderThread(FYvrCustomPresent* CustomPresent, const FYvrLayer* InLayer = nullptr);
	void UpdateTexture_RenderThread(FYvrCustomPresent* CustomPresent, FRHICommandListImmediate& RHICmdList);

#if PLATFORM_ANDROID
	yvrMatrix4f GenerateTextureCoords(const yvrMatrix4f& invViewModelMatrix);
	yvrLayerProjection2* UpdateLayer_RHIThread(FYvrCustomPresent* CustomPresent, const FMatrix& PlayerTrasform, const FTransform& CurrentTrackingToWorld, const UYvrHMDSettings* Settings);
#endif
	void IncrementSwapChainIndex_RHIThread(FYvrCustomPresent* CustomPresent);
	void ReleaseResources_RHIThread();

	bool bNeedsTexSrgbCreate;

protected:
	uint32 Id;
	IStereoLayers::FLayerDesc Desc;
	FXRSwapChainPtr SwapChain;
	FXRSwapChainPtr DepthSwapChain;
	FXRSwapChainPtr FoveationSwapChain;
	FXRSwapChainPtr RightSwapChain;
	FXRSwapChainPtr RightDepthSwapChain;
	bool bUpdateTexture;
	bool bInvertY;
	bool bHasDepth;
	bool bArrayTexture;

	UProceduralMeshComponent* PokeAHoleComponentPtr;
	AActor* PokeAHoleActor;

	FYvrpLayerPtr YvrpLayer;

#if PLATFORM_ANDROID
	yvrLayerProjection2* LayerProjection = nullptr;
#endif

private:
	void SetDefaultLayerParameters();

#if PLATFORM_ANDROID
	yvrMatrix4f LeftMatrix;
	yvrMatrix4f RightMatrix;

#endif
};

typedef TSharedPtr<FYvrLayer, ESPMode::ThreadSafe> FYvrLayerPtr;


//-------------------------------------------------------------------------------------------------
// FYvrLayerPtr_CompareId
//-------------------------------------------------------------------------------------------------

struct FYvrLayerPtr_CompareId
{
	FORCEINLINE bool operator()(const FYvrLayerPtr& A, const FYvrLayerPtr& B) const
	{
		return A->GetId() < B->GetId();
	}
};


//-------------------------------------------------------------------------------------------------
// FYvrLayerPtr_ComparePriority
//-------------------------------------------------------------------------------------------------

struct FYvrLayerPtr_ComparePriority
{
	FORCEINLINE bool operator()(const FYvrLayerPtr& A, const FYvrLayerPtr& B) const
	{
		if (A->GetDesc().Priority < B->GetDesc().Priority)
			return true;
		if (A->GetDesc().Priority > B->GetDesc().Priority)
			return false;

		return A->GetId() < B->GetId();
	}
};

struct FYvrLayerPtr_CompareTotal
{
	FORCEINLINE bool operator()(const FYvrLayerPtr& A, const FYvrLayerPtr& B) const
	{
		// Draw PoleAHole layers (Android only), EyeFov layer, followed by other layers
		int32 PassA = (A->GetId() == 0) ? 0 : A->NeedsPokeAHole() ? -1 : 1;
		int32 PassB = (B->GetId() == 0) ? 0 : B->NeedsPokeAHole() ? -1 : 1;

		if (PassA != PassB)
			return PassA < PassB;

		// Draw non-FaceLocked layers first
		const IStereoLayers::FLayerDesc& DescA = A->GetDesc();
		const IStereoLayers::FLayerDesc& DescB = B->GetDesc();

		bool bFaceLockedA = (DescA.PositionType == IStereoLayers::ELayerType::FaceLocked);
		bool bFaceLockedB = (DescB.PositionType == IStereoLayers::ELayerType::FaceLocked);

		if (bFaceLockedA != bFaceLockedB)
			return !bFaceLockedA;

		// Draw layers by ascending priority
		if (DescA.Priority != DescB.Priority)
			return DescA.Priority < DescB.Priority;

		// Draw layers by ascending id
		return A->GetId() < B->GetId();
	}
};