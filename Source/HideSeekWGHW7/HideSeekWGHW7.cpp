// Fill out your copyright notice in the Description page of Project Settings.

#include "HideSeekWGHW7.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, HideSeekWGHW7, "HideSeekWGHW7" );

const FString EnumToStringLocal(const TCHAR* Enum, int32 EnumValue)
{
    const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Enum, true);
    if (!EnumPtr)
        return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();

#if WITH_EDITOR
    return EnumPtr->GetDisplayNameText(EnumValue).ToString();
#else
    return EnumPtr->GetEnumName(EnumValue);
#endif
}
