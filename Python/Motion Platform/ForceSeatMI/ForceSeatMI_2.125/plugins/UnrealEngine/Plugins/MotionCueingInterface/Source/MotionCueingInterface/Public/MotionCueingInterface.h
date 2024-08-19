// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Runtime/Launch/Resources/Version.h"
#include "Modules/ModuleManager.h"

class MotionHost;

class FMotionCueingInterfaceModule : public IModuleInterface
{
public:
	virtual MotionHost* CreateMotionHost(const char* profile = nullptr, const char* appID = nullptr) = 0;

public:
	static inline FMotionCueingInterfaceModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FMotionCueingInterfaceModule>("MotionCueingInterface");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("MotionCueingInterface");
	}
};
