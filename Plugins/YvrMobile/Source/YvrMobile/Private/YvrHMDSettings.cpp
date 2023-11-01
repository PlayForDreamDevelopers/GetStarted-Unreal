#include "YvrHMDSettings.h"
#include "Engine/RendererSettings.h"

UYvrHMDSettings::UYvrHMDSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ColorSpace(EYvrColorSpace::ColorSpace_SRGB)
	, FFRLevel(EYvrFixedFoveatedRenderingLevel::FFR_Off)
	, CPULevel(4)
	, GPULevel(5)
	, PixelDensity(1.0f)
	, DisplayRefreshRate(90.0f)
	, BasePosition(FVector::ZeroVector)
	, BaseOrientation(FQuat::Identity)
	, bApplyColorScaleAndOffsetToAllLayers(false)
	, ColorScale(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	, ColorOffset(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f))
	, bUseHWsRGBEncoding(false)
{
#if WITH_EDITOR
	ResetsRGBConfig();
#endif
}

void UYvrHMDSettings::PostInitProperties()
{
	Super::PostInitProperties();
	bUseHWsRGBEncoding = true;
}

#if WITH_EDITOR
void UYvrHMDSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Super::UpdateDefaultConfigFile();
	HandlesRGBHWSupport();
}

void UYvrHMDSettings::HandlesRGBHWSupport()
{
	URendererSettings* const Settings = GetMutableDefault<URendererSettings>();
	static auto* MobileUseHWsRGBEncodingCVAR = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Mobile.UseHWsRGBEncoding"));

	if (bUseHWsRGBEncoding != Settings->bMobileUseHWsRGBEncoding)
	{
		Settings->bMobileUseHWsRGBEncoding = bUseHWsRGBEncoding;
		Settings->UpdateSinglePropertyInConfigFile(Settings->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(URendererSettings, bMobileUseHWsRGBEncoding)), GetDefaultConfigFilename());
	}

	if (MobileUseHWsRGBEncodingCVAR && MobileUseHWsRGBEncodingCVAR->GetInt() != (int)bUseHWsRGBEncoding)
	{
		MobileUseHWsRGBEncodingCVAR->Set((int)bUseHWsRGBEncoding);
	}
}

void UYvrHMDSettings::ResetsRGBConfig()
{
	// To Prevent sRGB from being turned off by AndroidSetting
	FEditorDelegates::RefreshAllBrowsers.AddUObject(this,&UYvrHMDSettings::HandlesRGBHWSupport);
}
#endif 