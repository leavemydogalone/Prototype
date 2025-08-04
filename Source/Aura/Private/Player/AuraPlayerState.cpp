// Copyright Druid Mechanics


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Character/AuraUnitBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;

	StoredAbilities.SetNum(0);
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
	DOREPLIFETIME(AAuraPlayerState, SelectedUnit);
	DOREPLIFETIME(AAuraPlayerState, TeamID);
	DOREPLIFETIME(AAuraPlayerState, StoredAbilities);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::SetSelectedUnit(AActor* NewUnit)
{

	if (NewUnit == SelectedUnit)
	{
		return;
	}

	if (SelectedUnit) {
		LastSelectedUnit = SelectedUnit;
		if (IsValid(LastSelectedUnit) && LastSelectedUnit->Implements<UHighlightInterface>())
		{
			IHighlightInterface::Execute_SetUnselectedHighlight(LastSelectedUnit);
		}

	}

	if (IsValid(NewUnit) && NewUnit->Implements<UUnitInterface>())
	{
		SelectedUnit = NewUnit;
		UE_LOG(LogTemp, Warning, TEXT("Selected Unit: %s"), *GetNameSafe(SelectedUnit));

		if (!HasAuthority() && IsValid(NewUnit) && NewUnit->Implements<UHighlightInterface>()) {
			IHighlightInterface::Execute_SetSelectedHighlight(NewUnit);
		}
	}

}

void  AAuraPlayerState::UnselectCurrentSelectedUnit()
{
	if (SelectedUnit) {
		IHighlightInterface::Execute_SetUnselectedHighlight(SelectedUnit);
		LastSelectedUnit = SelectedUnit;
		SelectedUnit = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Selected Unit is nullptr or does not implement UUnitInterface"));
	}
}

int32 AAuraPlayerState::GetTeamID_Implementation()
{
	return TeamID;
}

void AAuraPlayerState::AddAbilityToStoredAbilities(FUnitAbilityPreviewInfo& UnitAbilityPreviewInfo)
{
	if (StoredAbilities.Num() < MaxStoredAbilities) StoredAbilities.Emplace(UnitAbilityPreviewInfo);
}

void AAuraPlayerState::RemoveFirstStoredAbility()
{
	if (StoredAbilities.Num() > 0)
	{
		StoredAbilities.RemoveAt(0);
	}
}


void AAuraPlayerState::RemoveLastStoredAbility()
{
	if (StoredAbilities.Num() > 0 && AbilitySystemComponent)
	{

		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Event_Unit_Cancel;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AbilitySystemComponent->GetAvatarActor(), GameplayTags.Event_Unit_Cancel, Payload);

		StoredAbilities.RemoveAt(StoredAbilities.Num() - 1);
	}
}


void AAuraPlayerState::OnRep_SelectedUnit()
{
	//write a debug message on screen of the new selected unit

	UE_LOG(LogTemp, Warning, TEXT("Client Selected Unit"));

}

void AAuraPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AAuraPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
}

void AAuraPlayerState::SetAttributePoints(int32 InPoints)
{
	AttributePoints = InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::SetSpellPoints(int32 InPoints)
{
	SpellPoints = InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::OnRep_TeamID(int32 OldTeamID)
{
	UE_LOG(LogTemp, Warning, TEXT("Team ID changed from %d to %d"), OldTeamID, TeamID);
	// OnTeamIDChangedDelegate.Broadcast(TeamID);
}

void AAuraPlayerState::OnRep_StoredAbilities(TArray<FUnitAbilityPreviewInfo>& OldStoredAbilities)
{
	OnStoredAbilitiesArrayChangedDelegate.Broadcast(StoredAbilities);
	UE_LOG(LogTemp, Warning, TEXT("Stored Abilities length changed from %d to %d"), OldStoredAbilities.Num(), StoredAbilities.Num());
	OnStoredAbilitiesCountChangedDelegate.Broadcast(StoredAbilities.Num());
}

void AAuraPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}


