// Fill out your copyright notice in the Description page of Project Settings.


#include "TimewarsGameInstance.h"

#include "FormationHelper.h"

UTimewarsGameInstance::UTimewarsGameInstance()
{
	FormationHelper();
}

UFormationHelper* UTimewarsGameInstance::FormationHelper()
{
	UE_LOG(LogTemp, Display, TEXT("Creating FormationHelper"))
	return
         IsValid(FormationHelperInstance) ?
         FormationHelperInstance :
         FormationHelperInstance = NewObject<UFormationHelper>(
             this,
             FName("FormationHelper"));
}
