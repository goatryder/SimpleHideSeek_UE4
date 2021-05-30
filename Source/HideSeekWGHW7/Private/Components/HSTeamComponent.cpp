// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HSTeamComponent.h"
#include "GameFramework/Character.h"
#include "HSCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HSGameMode.h"

TSet<TWeakObjectPtr<UHSTeamComponent>> UHSTeamComponent::TeamComponents;

FOnTeamCompTeamChange UHSTeamComponent::NotifyTeamCompTeamChanged;

UHSTeamComponent::UHSTeamComponent()
{
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

	TeamComponents.Add(this);
}

void UHSTeamComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterOwner)
	{
		CharacterOwner->InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &UHSTeamComponent::OnOverlapOwnerInteractSphere);
	}

	SetTeam(TeamType);
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

		// set can change team to seeker when overlap with other seeker
		bCanSetTeamSeekOnSeekerOverlap = true;

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

void UHSTeamComponent::OnOverlapOwnerInteractSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHSGameMode* GM = Cast<AHSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!bCanSetTeamSeekOnSeekerOverlap || TeamType == ETeamType::Seek || !GM || GM->GetGameStage() != EHSGameStage::Seeking)
	{
		return;
	}

	// Try to change team from hide to seek when overlapping with other seeker
	if (UHSTeamComponent* OtherTeamComp = Cast<UHSTeamComponent>(OtherActor->GetComponentByClass(UHSTeamComponent::StaticClass())))
	{
		if (OtherTeamComp->GetTeam() == ETeamType::Seek)
		{
			SetTeam(ETeamType::Seek);
			bCanSetTeamSeekOnSeekerOverlap = false;
		}
	}
}

