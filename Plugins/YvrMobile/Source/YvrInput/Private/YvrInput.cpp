// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.

#include "YvrInput.h"
#include "YvrMobile.h"

#include "IYvrInputModule.h"
#include "IHeadMountedDisplay.h"

#define LOCTEXT_NAMESPACE "YvrInput"

using namespace YvrInput;

//**************
//Yvr Key
//**************
//Button
const FKey FYvrKey::YvrButton_A("YvrButton_A");
const FKey FYvrKey::YvrButton_B("YvrButton_B");
const FKey FYvrKey::YvrButton_X("YvrButton_X");
const FKey FYvrKey::YvrButton_Y("YvrButton_Y");
const FKey FYvrKey::YvrButton_Start("YvrButton_Start");
const FKey FYvrKey::YvrButton_Reserved("YvrButton_Reserved");
const FKey FYvrKey::YvrButton_LIndexTrigger("YvrButton_LIndexTrigger");
const FKey FYvrKey::YvrButton_RIndexTrigger("YvrButton_RIndexTrigger");
const FKey FYvrKey::YvrButton_LHandTrigger("YvrButton_LHandTrigger");
const FKey FYvrKey::YvrButton_RHandTrigger("YvrButton_RHandTrigger");
const FKey FYvrKey::YvrButton_LThumbstick("YvrButton_LThumbstick");
const FKey FYvrKey::YvrButton_RThumbstick("YvrButton_RThumbstick");
const FKey FYvrKey::YvrButton_LThumbstickUp("YvrButton_LThumbstickUp");
const FKey FYvrKey::YvrButton_LThumbstickDown("YvrButton_LThumbstickDown");
const FKey FYvrKey::YvrButton_LThumbstickLeft("YvrButton_LThumbstickLeft");
const FKey FYvrKey::YvrButton_LThumbstickRight("YvrButton_LThumbstickRight");
const FKey FYvrKey::YvrButton_RThumbstickUp("YvrButton_RThumbstickUp");
const FKey FYvrKey::YvrButton_RThumbstickDown("YvrButton_RThumbstickDown");
const FKey FYvrKey::YvrButton_RThumbstickLeft("YvrButton_RThumbstickLeft");
const FKey FYvrKey::YvrButton_RThumbstickRight("YvrButton_RThumbstickRight");

//Touch
const FKey FYvrKey::YvrTouch_A("YvrTouch_A");
const FKey FYvrKey::YvrTouch_B("YvrTouch_B");
const FKey FYvrKey::YvrTouch_X("YvrTouch_X");
const FKey FYvrKey::YvrTouch_Y("YvrTouch_Y");
const FKey FYvrKey::YvrTouch_LIndexTrigger("YvrTouch_LIndexTrigger");
const FKey FYvrKey::YvrTouch_RIndexTrigger("YvrTouch_RIndexTrigger");
const FKey FYvrKey::YvrTouch_LThumbstick("YvrTouch_LThumbstick");
const FKey FYvrKey::YvrTouch_RThumbstick("YvrTouch_RThumbstick");

//Analog
const FKey FYvrKey::YvrButton_LIndexTrigger_Axis("YvrButton_LIndexTrigger_Axis");
const FKey FYvrKey::YvrButton_LThumbstick_X("YvrButton_LThumbstick_X");
const FKey FYvrKey::YvrButton_LThumbstick_Y("YvrButton_LThumbstick_Y");
const FKey FYvrKey::YvrButton_RIndexTrigger_Axis("YvrButton_RIndexTrigger_Axis");
const FKey FYvrKey::YvrButton_RThumbstick_X("YvrButton_RThumbstick_X");
const FKey FYvrKey::YvrButton_RThumbstick_Y("YvrButton_RThumbstick_Y");

//**************
//Yvr Key Name
//**************

//Button
const FName FYvrKeyNames::YvrButton_A("YvrButton_A");
const FName FYvrKeyNames::YvrButton_B("YvrButton_B");
const FName FYvrKeyNames::YvrButton_X("YvrButton_X");
const FName FYvrKeyNames::YvrButton_Y("YvrButton_Y");
const FName FYvrKeyNames::YvrButton_Start("YvrButton_Start");
const FName FYvrKeyNames::YvrButton_Reserved("YvrButton_Reserved");
const FName FYvrKeyNames::YvrButton_LIndexTrigger("YvrButton_LIndexTrigger");
const FName FYvrKeyNames::YvrButton_RIndexTrigger("YvrButton_RIndexTrigger");
const FName FYvrKeyNames::YvrButton_LHandTrigger("YvrButton_LHandTrigger");
const FName FYvrKeyNames::YvrButton_RHandTrigger("YvrButton_RHandTrigger");
const FName FYvrKeyNames::YvrButton_LThumbstick("YvrButton_LThumbstick");
const FName FYvrKeyNames::YvrButton_RThumbstick("YvrButton_RThumbstick");
const FName FYvrKeyNames::YvrButton_LThumbstickUp("YvrButton_LThumbstickUp");
const FName FYvrKeyNames::YvrButton_LThumbstickDown("YvrButton_LThumbstickDown");
const FName FYvrKeyNames::YvrButton_LThumbstickLeft("YvrButton_LThumbstickLeft");
const FName FYvrKeyNames::YvrButton_LThumbstickRight("YvrButton_LThumbstickRight");
const FName FYvrKeyNames::YvrButton_RThumbstickUp("YvrButton_RThumbstickUp");
const FName FYvrKeyNames::YvrButton_RThumbstickDown("YvrButton_RThumbstickDown");
const FName FYvrKeyNames::YvrButton_RThumbstickLeft("YvrButton_RThumbstickLeft");
const FName FYvrKeyNames::YvrButton_RThumbstickRight("YvrButton_RThumbstickRight");

//Touch
const FName FYvrKeyNames::YvrTouch_A("YvrTouch_A");
const FName FYvrKeyNames::YvrTouch_B("YvrTouch_B");
const FName FYvrKeyNames::YvrTouch_X("YvrTouch_X");
const FName FYvrKeyNames::YvrTouch_Y("YvrTouch_Y");
const FName FYvrKeyNames::YvrTouch_LIndexTrigger("YvrTouch_LIndexTrigger");
const FName FYvrKeyNames::YvrTouch_RIndexTrigger("YvrTouch_RIndexTrigger");
const FName FYvrKeyNames::YvrTouch_LThumbstick("YvrTouch_LThumbstick");
const FName FYvrKeyNames::YvrTouch_RThumbstick("YvrTouch_RThumbstick");

class FYvrInputModule : public IYvrInputModule
{
	virtual TSharedPtr< class IInputDevice > CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override
	{
		return TSharedPtr< class IInputDevice >(new FYvrInput(InMessageHandler));
	}
};

IMPLEMENT_MODULE(FYvrInputModule, YvrInput)

FName FYvrInput::DeviceTypeName(TEXT("YvrInputDevice"));

FYvrInput::FYvrInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
	:MessageHandler(InMessageHandler)
{
	AddKeysToEngine();
	IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);
}

FYvrInput::~FYvrInput()
{
	IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
}

// Helper Functions
void FYvrInput::AddKeysToEngine()
{
	// Register the FKeys
	//Button
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_A, LOCTEXT("YvrButton_A", "Yvr Button (R) A"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_B, LOCTEXT("YvrButton_B", "Yvr Button (R) B"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_X, LOCTEXT("YvrButton_X", "Yvr Button (L) X"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_Y, LOCTEXT("YvrButton_Y", "Yvr Button (L) Y"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_Start, LOCTEXT("YvrButton_Start", "Yvr Button (L) Start"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_Reserved, LOCTEXT("YvrButton_Reserved", "Yvr Button Reserved"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_LIndexTrigger, LOCTEXT("YvrButton_LIndexTrigger", "Yvr Button (L) Index Trigger"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_RIndexTrigger, LOCTEXT("YvrButton_RIndexTrigger", "Yvr Button (R) Index Trigger"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_LHandTrigger, LOCTEXT("YvrButton_LHandTrigger", "Yvr Button (L) Hand Trigger"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_RHandTrigger, LOCTEXT("YvrButton_RHandTrigger", "Yvr Button (R) Hand Trigger"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_LThumbstick, LOCTEXT("YvrButton_LThumbstick", "Yvr Button (L) Thumbstick"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_RThumbstick, LOCTEXT("YvrButton_RThumbstick", "Yvr Button (R) Thumbstick"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_LThumbstickUp, LOCTEXT("YvrButton_LThumbstickUp", "Yvr Button (L) Thumbstick Up"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_LThumbstickDown, LOCTEXT("YvrButton_LThumbstickDown", "Yvr Button (L) Thumbstick Down"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_LThumbstickLeft, LOCTEXT("YvrButton_LThumbstickLeft", "Yvr Button (L) Thumbstick Left"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_LThumbstickRight, LOCTEXT("YvrButton_LThumbstickRight", "Yvr Button (L) Thumbstick Right"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_RThumbstickUp, LOCTEXT("YvrButton_RThumbstickUp", "Yvr Button (R) Thumbstick Up"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_RThumbstickDown, LOCTEXT("YvrButton_RThumbstickDown", "Yvr Button (R) Thumbstick Down"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_RThumbstickLeft, LOCTEXT("YvrButton_RThumbstickLeft", "Yvr Button (R) Thumbstick Left"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_RThumbstickRight, LOCTEXT("YvrButton_RThumbstickRight", "Yvr Button (R) Thumbstick Right"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	//Touch
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrTouch_A, LOCTEXT("YvrTouch_A", "Yvr Touch (R) A"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrTouch_B, LOCTEXT("YvrTouch_B", "Yvr Touch (R) B"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrTouch_X, LOCTEXT("YvrTouch_X", "Yvr Touch (L) X"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrTouch_Y, LOCTEXT("YvrTouch_Y", "Yvr Touch (L) Y"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrTouch_LIndexTrigger, LOCTEXT("YvrTouch_LIndexTrigger", "Yvr Touch (L) Index Trigger"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrTouch_RIndexTrigger, LOCTEXT("YvrTouch_RIndexTrigger", "Yvr Touch (R) Index Trigger"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrTouch_LThumbstick, LOCTEXT("YvrTouch_LThumbstick", "Yvr Touch (L) Thumbstick"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrTouch_RThumbstick, LOCTEXT("YvrTouch_RThumbstick", "Yvr Touch (R) Thumbstick"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	//Analog
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_LIndexTrigger_Axis, LOCTEXT("YvrButton_LIndexTrigger_Axis", "Yvr Button (L) Index Trigger Axis"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_LThumbstick_X, LOCTEXT("YvrButton_LThumbstick_X", "Yvr Button (L) Thumbstick X"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_LThumbstick_Y, LOCTEXT("YvrButton_LThumbstick_Y", "Yvr Button (L) Thumbstick Y"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_RIndexTrigger_Axis, LOCTEXT("YvrButton_RIndexTrigger_Axis", "Yvr Button (R) Index Trigger Axis"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_RThumbstick_X, LOCTEXT("YvrButton_RThumbstick_X", "Yvr Button (R) Thumbstick X"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(FYvrKey::YvrButton_RThumbstick_Y, LOCTEXT("YvrButton_RThumbstick_Y", "Yvr Button (R) Thumbstick Y"), FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
}

bool FYvrInput::IsAvailable() const
{
	if (IYvrMobilePlugin::IsAvailable())
	{
		return true;
	}
	return false;
}


void FYvrInput::Tick(float DeltaTime){}

void FYvrInput::SendControllerEvents()
{
#if PLATFORM_ANDROID
	if (IsAvailable())
	{
		//Left Controller
		{
			yvrControllerState State;
			FYvrMobilePlugin::GetPluginWrapper().GetControllerState(yvrController::kControllerLeft, &State);

			uint32_t RemoteButton = State.Buttons;
			uint32_t RemoteTouch = State.Touches;
			float RemoteIndexTrigger = State.IndexTrigger;
			float RemoteThumbstickX = State.Thumbstick[0];
			float RemoteThumbstickY = State.Thumbstick[1];

			bool bButtonPressed = false;
			bool bTouchPressed = false;
			float IndexTriggerAxis = 0.0;
			float ThumbstickAxisX = 0.0;
			float ThumbstickAxisY = 0.0;

			//simulate left index trigger pressed event
			if (RemoteIndexTrigger >= 0.5)
			{
				RemoteButton |= (1 << EYvrControllerButton::LIndexTrigger);
			}

			for (uint32_t Index = 0; Index < EYvrControllerButton::Total; ++Index)
			{
				//Button
				FYvrButtonState& ButtonState = CachedLeftControllerState.Buttons[Index];
				bButtonPressed = RemoteButton & (1 << Index);
				if (bButtonPressed != ButtonState.bIsPressed)
				{
					ButtonState.bIsPressed = bButtonPressed;

					if (ButtonState.bIsPressed)
					{
						MessageHandler->OnControllerButtonPressed(ButtonState.Key, 0, false);
						UE_LOG(LogHMD, Log, TEXT("Button %s pressed"), *ButtonState.Key.ToString());
					}
					else
					{
						MessageHandler->OnControllerButtonReleased(ButtonState.Key, 0, false);
						UE_LOG(LogHMD, Log, TEXT("Button %s released"), *ButtonState.Key.ToString());
					}
				}

				//Touch
				FYvrButtonState& TouchState = CachedLeftControllerState.Touches[Index];
				bTouchPressed = RemoteTouch & (1 << Index);

				if (bTouchPressed != TouchState.bIsPressed)
				{
					TouchState.bIsPressed = bTouchPressed;
					if (TouchState.bIsPressed)
					{
						MessageHandler->OnControllerButtonPressed(TouchState.Key, 0, false);
						UE_LOG(LogHMD, Log, TEXT("Touch %s pressed"), *TouchState.Key.ToString());
					}
					else
					{
						MessageHandler->OnControllerButtonReleased(TouchState.Key, 0, false);
						UE_LOG(LogHMD, Log, TEXT("Touch %s released"), *TouchState.Key.ToString());
					}
				}

				//Analog
				IndexTriggerAxis = RemoteIndexTrigger;
				if (IndexTriggerAxis != CachedLeftControllerState.IndexTriggerAxis)
				{
					CachedLeftControllerState.IndexTriggerAxis = IndexTriggerAxis;
					MessageHandler->OnControllerAnalog(FYvrKey::YvrButton_LIndexTrigger_Axis.GetFName(), 0, IndexTriggerAxis);
					UE_LOG(LogHMD, Log, TEXT("IndexTriggerAxis %f "), IndexTriggerAxis);
				}

				ThumbstickAxisX = RemoteThumbstickX;
				if (ThumbstickAxisX != CachedLeftControllerState.ThumbstickAxis[0])
				{
					CachedLeftControllerState.ThumbstickAxis[0] = ThumbstickAxisX;
					MessageHandler->OnControllerAnalog(FYvrKey::YvrButton_LThumbstick_X.GetFName(), 0, ThumbstickAxisX);
					UE_LOG(LogHMD, Log, TEXT("Left ThumbstickAxisX %f "), ThumbstickAxisX);
				}

				ThumbstickAxisY = RemoteThumbstickY;
				if (ThumbstickAxisY != CachedLeftControllerState.ThumbstickAxis[1])
				{
					CachedLeftControllerState.ThumbstickAxis[1] = ThumbstickAxisY;
					MessageHandler->OnControllerAnalog(FYvrKey::YvrButton_LThumbstick_Y.GetFName(), 0, ThumbstickAxisY);
					UE_LOG(LogHMD, Log, TEXT("Left ThumbstickAxisY %f "), ThumbstickAxisY);
				}
			}
		}

		//Right Controller
		{
			yvrControllerState State;
			FYvrMobilePlugin::GetPluginWrapper().GetControllerState(yvrController::kControllerRight, &State);

			uint32_t RemoteButton = State.Buttons;
			uint32_t RemoteTouch = State.Touches;
			float RemoteIndexTrigger = State.IndexTrigger;
			float RemoteThumbstickX = State.Thumbstick[0];
			float RemoteThumbstickY = State.Thumbstick[1];

			bool bButtonPressed = false;
			bool bTouchPressed = false;
			float IndexTriggerAxis = 0.0;
			float ThumbstickAxisX = 0.0;
			float ThumbstickAxisY = 0.0;

			//simulate right index trigger pressed event
			if (RemoteIndexTrigger >= 0.5)
			{
				RemoteButton |= (1 << EYvrControllerButton::RIndexTrigger);
			}

			for (uint32_t Index = 0; Index < EYvrControllerButton::Total; ++Index)
			{
				//Button
				FYvrButtonState& ButtonState = CachedRightControllerState.Buttons[Index];
				bButtonPressed = RemoteButton & (1 << Index);
				if (bButtonPressed != ButtonState.bIsPressed)
				{
					ButtonState.bIsPressed = bButtonPressed;

					if (ButtonState.bIsPressed)
					{
						MessageHandler->OnControllerButtonPressed(ButtonState.Key, 0, false);
						UE_LOG(LogHMD, Log, TEXT("Button %s pressed"), *ButtonState.Key.ToString());
					}
					else
					{
						MessageHandler->OnControllerButtonReleased(ButtonState.Key, 0, false);
						UE_LOG(LogHMD, Log, TEXT("Button %s released"), *ButtonState.Key.ToString());
					}
				}

				//Touch
				FYvrButtonState& TouchState = CachedRightControllerState.Touches[Index];
				bTouchPressed = RemoteTouch & (1 << Index);

				if (bTouchPressed != TouchState.bIsPressed)
				{
					TouchState.bIsPressed = bTouchPressed;
					if (TouchState.bIsPressed)
					{
						MessageHandler->OnControllerButtonPressed(TouchState.Key, 0, false);
						UE_LOG(LogHMD, Log, TEXT("Touch %s pressed"), *TouchState.Key.ToString());
					}
					else
					{
						MessageHandler->OnControllerButtonReleased(TouchState.Key, 0, false);
						UE_LOG(LogHMD, Log, TEXT("Touch %s released"), *TouchState.Key.ToString());
					}
				}

				//Analog
				IndexTriggerAxis = RemoteIndexTrigger;
				if (IndexTriggerAxis != CachedRightControllerState.IndexTriggerAxis)
				{
					CachedRightControllerState.IndexTriggerAxis = IndexTriggerAxis;
					MessageHandler->OnControllerAnalog(FYvrKey::YvrButton_RIndexTrigger_Axis.GetFName(), 0, IndexTriggerAxis);
					UE_LOG(LogHMD, Log, TEXT("IndexTriggerAxis %f "), IndexTriggerAxis);
				}

				ThumbstickAxisX = RemoteThumbstickX;
				if (ThumbstickAxisX != CachedRightControllerState.ThumbstickAxis[0])
				{
					CachedRightControllerState.ThumbstickAxis[0] = ThumbstickAxisX;
					MessageHandler->OnControllerAnalog(FYvrKey::YvrButton_RThumbstick_X.GetFName(), 0, ThumbstickAxisX);
					UE_LOG(LogHMD, Log, TEXT("Right ThumbstickAxisX %f "), ThumbstickAxisX);
				}

				ThumbstickAxisY = RemoteThumbstickY;
				if (ThumbstickAxisY != CachedRightControllerState.ThumbstickAxis[1])
				{
					CachedRightControllerState.ThumbstickAxis[1] = ThumbstickAxisY;
					MessageHandler->OnControllerAnalog(FYvrKey::YvrButton_RThumbstick_Y.GetFName(), 0, ThumbstickAxisY);
					UE_LOG(LogHMD, Log, TEXT("Right ThumbstickAxisY %f "), ThumbstickAxisY);
				}

			}
		}
	}
#endif
}

void FYvrInput::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

bool FYvrInput::Exec(UWorld * InWorld, const TCHAR * Cmd, FOutputDevice & Ar)
{
	return false;
}

void FYvrInput::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value){}

void FYvrInput::SetChannelValues(int32 ControllerId, const FForceFeedbackValues & values){}

bool FYvrInput::GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition, float WorldToMetersScale) const
{
#if PLATFORM_ANDROID
	if (IsAvailable())
	{
		FYvrMobileHMD* YvrHMD = static_cast<FYvrMobileHMD*>(GEngine->XRSystem->GetHMDDevice());
		switch (DeviceHand)
		{
		case EControllerHand::Left:
		{
			yvrPoseState LeftPoseState;
			const float PredictTimeMs = FYvrMobilePlugin::GetPluginWrapper().GetPredictedDisplayTime();
			FYvrMobilePlugin::GetPluginWrapper().GetNodePose(LeftController, PredictTimeMs, LeftPoseState);

			FQuat OutQuat;
			YvrHMD->PoseToOrientationAndPosition(LeftPoseState.pose, OutQuat, OutPosition, WorldToMetersScale);
			OutOrientation = OutQuat.Rotator();
			break;
		}
		case EControllerHand::Right:
		{
			yvrPoseState RightPoseState;
			const float PredictTimeMs = FYvrMobilePlugin::GetPluginWrapper().GetPredictedDisplayTime();
			FYvrMobilePlugin::GetPluginWrapper().GetNodePose(RightController, PredictTimeMs, RightPoseState);
			FQuat OutQuat;
			YvrHMD->PoseToOrientationAndPosition(RightPoseState.pose, OutQuat, OutPosition, WorldToMetersScale);
			OutOrientation = OutQuat.Rotator();
			break;
		}
		default:
			break;
		}
		return true;
	}
#endif
	return false;
}

ETrackingStatus FYvrInput::GetControllerTrackingStatus(const int32 ControllerIndex, const EControllerHand DeviceHand) const
{
#if PLATFORM_ANDROID
	if (IsAvailable())
	{
		return ETrackingStatus::Tracked;
	}
#endif
	return ETrackingStatus::NotTracked;
}

void FYvrInput::SetHapticFeedbackValues(int32 ControllerId, int32 Hand, const FHapticFeedbackValues& Values)
{
#if PLATFORM_ANDROID
	if (IsAvailable())
	{
		float FreqMin, FreqMax = 0.f;
		GetHapticFrequencyRange(FreqMin, FreqMax);

		const float InitialFreq = (Values.Frequency > 0.0f) ? Values.Frequency : 1.0f;
		const float Frequency = FMath::Lerp(FreqMin, FreqMax, FMath::Clamp(InitialFreq, 0.f, 1.f));
		const float Amplitude = Values.Amplitude * GetHapticAmplitudeScale();

		yvrController YvrController = (EControllerHand(Hand) == EControllerHand::Left) ? yvrController::kControllerLeft : yvrController::kControllerRight;
		FYvrMobilePlugin::GetPluginWrapper().SetControllerVibration(YvrController, Frequency, Amplitude);
		UE_LOG(LogHMD, Log, TEXT("%d Controller Vibration %f %f"), (int)YvrController, Frequency, Amplitude);
	}
#endif
}

void FYvrInput::GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const
{
	MinFrequency = 0.f;
	MaxFrequency = 1.f;
}

float FYvrInput::GetHapticAmplitudeScale() const
{
	return 1.f;
}