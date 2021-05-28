// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HSTeamComponent.generated.h"

class UMaterialInstance;
class AHSCharacter;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnTeamCompTeamChange, UHSTeamComponent*, AHSCharacter*, bool bNewTeamIsRed);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HIDESEEKWGHW7_API UHSTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHSTeamComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeamComp")
		UMaterialInstance* MaterialTeamBlue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeamComp")
		UMaterialInstance* MaterialTeamRed;

	UPROPERTY(EditDefaultsOnly, Category = "TeamComp")
		bool bTeamIsRed;

	AHSCharacter* CharacterOwner;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		bool TeamIsRed() const { return bTeamIsRed; }
	
	UFUNCTION(BlueprintCallable)
		void SetTeam(bool bNewTeamIsRed);

	virtual void DestroyComponent(bool bPromoteChildren = false) override;

	// ask someone if this is allright to do
	// Store week ptrs to all team components;
	static TSet<TWeakObjectPtr<UHSTeamComponent>> TeamComponents;

	static TArray<UHSTeamComponent*> GetTeamComponents(bool bTeamRed);

	/** Global notification when HSCharacter team changes */
	static FOnTeamCompTeamChange NotifyTeamCompTeamChanged;
};
