// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.

#pragma once
#include "YvrInputState.h"
#include "YvrPluginWrapper.h"

#include "InputDevice.h"
#include "IHapticDevice.h"
#include "XRMotionControllerBase.h"

class FYvrInput : public IInputDevice, public FXRMotionControllerBase, public IHapticDevice
{
public:
	FYvrInput(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);
	virtual ~FYvrInput();

public:
	// IInputDevice
	virtual void Tick(float DeltaTime);
	virtual void SendControllerEvents();
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar);
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value);
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values);

	// IMotionController
	static FName DeviceTypeName;
	virtual FName GetMotionControllerDeviceTypeName() const override
	{
		return DeviceTypeName;
	}

	virtual bool GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition, float WorldToMetersScale) const;
	virtual ETrackingStatus GetControllerTrackingStatus(const int32 ControllerIndex, const EControllerHand DeviceHand) const;

	// IHapticDevice overrides
	IHapticDevice* GetHapticDevice() override { return (IHapticDevice*)this; }
	virtual void SetHapticFeedbackValues(int32 ControllerId, int32 Hand, const FHapticFeedbackValues& Values) override;

	virtual void GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const override;
	virtual float GetHapticAmplitudeScale() const override;
private:
	void AddKeysToEngine();
	bool IsAvailable() const;


	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
	YvrInput::FYvrControllerState CachedLeftControllerState;
	YvrInput::FYvrControllerState CachedRightControllerState;
};