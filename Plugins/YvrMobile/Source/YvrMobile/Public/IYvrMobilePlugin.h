// Copyright 2020-2021 Yvr Technology Co., Ltd. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "IHeadMountedDisplayModule.h"

class IYvrMobilePlugin : public IHeadMountedDisplayModule
{

public:
	static inline IYvrMobilePlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< IYvrMobilePlugin >( "YvrMobile" );
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "YvrMobile" );
	}
};
