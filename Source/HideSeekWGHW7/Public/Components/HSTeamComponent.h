// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HSTypes.h"
#include "HSTeamComponent.generated.h"

class UMaterialInstance;
class AHSCharacter;


DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnTeamCompTeamChange, UHSTeamComponent*, AHSCharacter*, ETeamType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HIDESEEKWGHW7_API UHSTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHSTeamComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeamComp")
		UMaterialInstance* MaterialTeamSeek;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeamComp")
		UMaterialInstance* MaterialTeamHide;

	UPROPERTY(EditDefaultsOnly, Category = "TeamComp")
		ETeamType TeamType;

	AHSCharacter* CharacterOwner;

	// Can character become seeker if he overlap with other character whoes team is seeker?
	bool bCanSetTeamSeekOnSeekerOverlap;

public:	

	UFUNCTION(BlueprintCallable)
		ETeamType GetTeam() const { return TeamType; }
	
	UFUNCTION(BlueprintCallable)
		void SetTeam(ETeamType NewTeamType);

	virtual void DestroyComponent(bool bPromoteChildren = false) override;

	// ask someone if this is allright to do
	// Store week ptrs to all team components;
	static TSet<TWeakObjectPtr<UHSTeamComponent>> TeamComponents;

	static TArray<UHSTeamComponent*> GetTeamComponents(ETeamType RequestedTeam);

	/** Global notification when HSCharacter team changes */
	static FOnTeamCompTeamChange NotifyTeamCompTeamChanged;

	UFUNCTION()
		void OnOverlapOwnerInteractSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
