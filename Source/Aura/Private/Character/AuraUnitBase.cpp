// Copyright Druid Mechanics


#include "Character/AuraUnitBase.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "Aura/Aura.h"

AAuraUnitBase::AAuraUnitBase()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	GetMesh()->MarkRenderStateDirty();

}

//void AAuraUnitBase::PossessedBy(AController* NewController)
//{
//	Super::PossessedBy(NewController);
//
//	if (!HasAuthority()) return;
//	AuraAIController = Cast<AAuraAIController>(NewController);
//}

void AAuraUnitBase::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
}

void AAuraUnitBase::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
		AddCharacterAbilities();
	}
	OnAscRegistered.Broadcast(AbilitySystemComponent);

}

void AAuraUnitBase::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraUnitBase::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void AAuraUnitBase::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void AAuraUnitBase::ShowAbilityPreview(FGameplayTag AbilityTag, FVector TargetLocation)
{
	if (AController* OwnerController = Cast<AController>(GetOwner())) {
		Client_ShowAbilityPreview_Implementation(AbilityTag, TargetLocation);
	}
}

void AAuraUnitBase::Client_ShowAbilityPreview_Implementation(FGameplayTag AbilityTag, FVector TargetLocation)
{
	// Draw a debug sphere at the target location
	if (GEngine)
	{
		const float SphereRadius = 100.0f; // Adjust the radius as needed
		const FColor SphereColor = FColor::Red; // Adjust the color as needed
		const float SphereLifetime = 5.0f; // Duration the sphere will be visible
		const uint8 DepthPriority = 0; // Depth priority for rendering

		DrawDebugSphere(GetWorld(), TargetLocation, SphereRadius, 12, SphereColor, false, SphereLifetime, DepthPriority);
	}

}
