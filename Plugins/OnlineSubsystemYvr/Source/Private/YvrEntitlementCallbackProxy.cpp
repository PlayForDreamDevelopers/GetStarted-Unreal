#include "YvrEntitlementCallbackProxy.h"
#include "Online.h"

UYvrEntitlementCallbackProxy::UYvrEntitlementCallbackProxy(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

UYvrEntitlementCallbackProxy* UYvrEntitlementCallbackProxy::VerifyEntitlement()
{
	UYvrEntitlementCallbackProxy* Proxy = NewObject<UYvrEntitlementCallbackProxy>();
	Proxy->SetFlags(RF_StrongRefOnFrame);
	return Proxy;
}

void UYvrEntitlementCallbackProxy::Activate()
{
	auto YvrIdentityInterface = Online::GetIdentityInterface(YVR_SUBSYSTEM);

	if (YvrIdentityInterface.IsValid())
	{

		auto Unused = new FUniqueNetIdString("UNUSED", YVR_SUBSYSTEM);
		YvrIdentityInterface->GetUserPrivilege(
			*Unused,
			EUserPrivileges::CanPlay,
			IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate::CreateUObject(this, &UYvrEntitlementCallbackProxy::OnUserPrivilegeCompleteDelegate)
		);
	}
	else
	{
		UE_LOG_ONLINE_ENTITLEMENT(Warning, TEXT("Yvr platform service not available. Skipping entitlement check."));
		OnFailure.Broadcast();
	}
}

void UYvrEntitlementCallbackProxy::OnUserPrivilegeCompleteDelegate(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 Result)
{
    if (Result == static_cast<uint32>(IOnlineIdentity::EPrivilegeResults::NoFailures))
    {
		OnSuccess.Broadcast();
    }
    else
    {
		OnFailure.Broadcast();
    }
}