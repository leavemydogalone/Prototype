// Copyright Scitcat Games


#include "Actor/AuraBall.h"
#include "Game/TurnSystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Game/AuraTurnPhase.h"

AAuraBall::AAuraBall()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(RootComponent);
}

UAbilitySystemComponent* AAuraBall::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraBall::BeginPlay()
{
	Super::BeginPlay();
	
	if (ITurnSystemInterface* TurnSystemInterface = GetWorld()->GetGameState<ITurnSystemInterface>())
	{
		FOnTurnPhaseChange& OnTurnPhaseChange = TurnSystemInterface->GetOnTurnPhaseChangeDelegate();
		OnTurnPhaseChange.AddUObject(this, &AAuraBall::HandleTurnPhaseChange);
	}

	InitAbilityActorInfo();

}

void AAuraBall::HandleTurnPhaseChange(EAuraTurnPhase NewTurnPhase)
{
	// Handle turn phase change logic here
	UE_LOG(LogTemp, Warning, TEXT("Turn phase changed on ball!"));

	switch (NewTurnPhase)
	{
	case EAuraTurnPhase::Planning:
		FreezeBall();
		break;
	case EAuraTurnPhase::ActionPhase1:
		UnFreezeBall();
		break;
	default:
		break;
	}
}

void AAuraBall::FreezeBall()
{
	if (BallMesh->IsSimulatingPhysics())
	{
		StoredLinearVelocity = BallMesh->GetPhysicsLinearVelocity();
		StoredAngularVelocity = BallMesh->GetPhysicsAngularVelocityInDegrees();
		BallMesh->SetSimulatePhysics(false);
	}
}

void AAuraBall::UnFreezeBall()
{
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetPhysicsLinearVelocity(StoredLinearVelocity);
	BallMesh->SetPhysicsAngularVelocityInDegrees(StoredAngularVelocity);
}


void AAuraBall::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraBall::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		//InitializeDefaultAttributes();
		AddCharacterAbilities();
	}
	//~ Can add the below if I want to attach the niagara and debuff components
	//OnAscRegistered.Broadcast(AbilitySystemComponent);
}

void AAuraBall::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	AuraASC->AddCharacterAbilities(StartupAbilities);
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

//void AAuraBall::InitializeDefaultAttributes() const
//{
//	ApplyEffectToSelf(DefaultBallAttributes, 1.f);
//}
