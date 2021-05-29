// Fill out your copyright notice in the Description page of Project Settings.


#include "HSCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

#include "Components/HSTeamComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AHSCharacter::AHSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InterractSphere"));
	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->InitSphereRadius(200.0f);


	// try to setup default mesh and anim bp
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundMesh(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	if (FoundMesh.Succeeded()) 
	{
		GetMesh()->SetSkeletalMesh(FoundMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -85.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

		static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial_0(TEXT("/Game/Mannequin/Character/Materials/M_Male_Body.M_Male_Body"));
		if (FoundMaterial_0.Succeeded())
		{
			GetMesh()->SetMaterial(0, FoundMaterial_0.Object);
		}

		static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial_1(TEXT("/Game/Mannequin/Character/Materials/M_UE4Man_ChestLogo.M_UE4Man_ChestLogo"));
		if (FoundMaterial_1.Succeeded())
		{
			GetMesh()->SetMaterial(1, FoundMaterial_1.Object);
		}

		static ConstructorHelpers::FObjectFinder<UAnimBlueprint> FoundMeshAnimClass(TEXT("/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP"));
		if (FoundMeshAnimClass.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(FoundMeshAnimClass.Object->GetAnimBlueprintGeneratedClass());
		}
	}

	// setup team component
	TeamComp = CreateDefaultSubobject<UHSTeamComponent>(TEXT("TeamComponent"));
}

// Called when the game starts or when spawned
void AHSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
