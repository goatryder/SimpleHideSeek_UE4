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
		MaterialTeamSeek = FoundMaterial_Blue.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial_Red(TEXT("/Game/Mannequin/Character/Materials/M_Male_Body_Red.M_Male_Body_Red"));
	if (FoundMaterial_Red.Succeeded())
	{
		MaterialTeamHide = FoundMaterial_Red.Object;
	}

	TeamType = ETeamType::Hide;

	// try to setup character
	CharacterOwner = Cast<AHSCharacter>(GetOwner());
}


// Called when the game starts
void UHSTeamComponent::BeginPlay()
{
	Super::BeginPlay();
	
	TeamComponents.Add(this);

	SetTeam(TeamType);
}


// Called every frame
void UHSTeamComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHSTeamComponent::SetTeam(ETeamType NewTeamType)
{
	TeamType = NewTeamType;

	switch (TeamType)
	{
	case ETeamType::Hide:
		if (CharacterOwner && MaterialTeamHide)
		{
			CharacterOwner->GetMesh()->SetMaterial(0, MaterialTeamHide);
		}
		break;

	case ETeamType::Seek:
		if (CharacterOwner && MaterialTeamSeek)
		{
			CharacterOwner->GetMesh()->SetMaterial(0, MaterialTeamSeek);
		}
		break;
	
	default:
		break;
	}

	// notify team changed
	NotifyTeamCompTeamChanged.Broadcast(this, CharacterOwner, TeamType);

}

void UHSTeamComponent::DestroyComponent(bool bPromoteChildren)
{
	Super::DestroyComponent(bPromoteChildren);
	TeamComponents.Remove(this);
}

TArray<UHSTeamComponent*> UHSTeamComponent::GetTeamComponents(ETeamType RequestedTeam)
{
	TArray<UHSTeamComponent*> TeamComps;

	for (auto& TeamComp : TeamComponents)
	{
		if (TeamComp.IsValid())
		{
			if (TeamComp.Get()->GetTeam() == RequestedTeam)
			{
				TeamComps.Add(TeamComp.Get());
			}
		}
	}

	return TeamComps;
}

