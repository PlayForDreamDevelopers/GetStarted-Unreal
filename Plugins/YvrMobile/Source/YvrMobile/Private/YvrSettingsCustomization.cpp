// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.
#include "YvrSettingsCustomization.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#include "PropertyHandle.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "YvrRuntimeSettings"

FYvrSettingsCustomization::FYvrSettingsCustomization()
    :SavedLayoutBuilder(nullptr)
{
    YvrSettings = GetMutableDefault<UYvrHMDSettings>();
}

TSharedRef<IDetailCustomization> FYvrSettingsCustomization::MakeInstance()
{
    return MakeShareable(new FYvrSettingsCustomization);
}

void FYvrSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
    SavedLayoutBuilder = &DetailLayout;
    DetailLayout.EditCategory(TEXT("YvrMobile"));
}
#endif