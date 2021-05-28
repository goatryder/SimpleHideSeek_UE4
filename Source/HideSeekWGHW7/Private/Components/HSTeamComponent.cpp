// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HSTeamComponent.h"
#include "GameFramework/Character.h"
#include "HSCharacter.h"

TSet<TWeakObjectPtr<UHSTeamComponent>> UHSTeamComponent::TeamComponents;


FOnTeamCompTeamChange UHSTeamComponent::NotifyTeamCompTeamChanged;

// Sets default values for this component's properties
UHSTeamComponent::UHSTeamComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial_Blue(TEXT("/Game/Mannequin/Character/Materials/M_Male_Body_Blue.M_Male_Body_Blue"));
	if (FoundMaterial_Blue.Succeeded())
	{
		MaterialTeamBlue = FoundMaterial_Blue.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial_Red(TEXT("/Game/Mannequin/Character/Materials/M_Male_Body_Red.M_Male_Body_Red"));
	if (FoundMaterial_Red.Succeeded())
	{
		MaterialTeamRed = FoundMaterial_Red.Object;
	}

	bTeamIsRed = false;

	// try to setup character
	CharacterOwner = Cast<AHSCharacter>(GetOwner());
}


// Called when the game starts
void UHSTeamComponent::BeginPlay()
{
	Super::BeginPlay();
	
	TeamComponents.Add(this);

	SetTeam(bTeamIsRed);
}


// Called every frame
void UHSTeamComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHSTeamComponent::SetTeam(bool bNewTeamIsRed)
{
	bTeamIsRed = bNewTeamIsRed;

	if (!CharacterOwner)
	{
		return;
	}

	if (bTeamIsRed)
	{
		if (MaterialTeamRed)
		{
			CharacterOwner->GetMesh()->SetMaterial(0, MaterialTeamRed);
		}
	}
	else
	{
		if (MaterialTeamBlue)
		{
			CharacterOwner->GetMesh()->SetMaterial(0, MaterialTeamBlue);
		}
	}

	// notify team changed
	NotifyTeamCompTeamChanged.Broadcast(this, CharacterOwner, bNewTeamIsRed);
}

void UHSTeamComponent::DestroyComponent(bool bPromoteChildren)
{
	Super::DestroyComponent(bPromoteChildren);
	TeamComponents.Remove(this);
}

TArray<UHSTeamComponent*> UHSTeamComponent::GetTeamComponents(bool bTeamRed)
{
	TArray<UHSTeamComponent*> TeamComps;

	for (auto& TeamComp : TeamComponents)
	{
		if (TeamComp.IsValid())
		{
			if (TeamComp.Get()->TeamIsRed() == bTeamRed)
			{
				TeamComps.Add(TeamComp.Get());
			}
		}
	}

	return TeamComps;
}

