// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraAbilityTypes.h"
#include "AuraUnitDamageVolume.generated.h"

class UGameplayEffect;
class UBoxComponent;

UCLASS()
class AURA_API AAuraUnitDamageVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraUnitDamageVolume();


	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnHit();
	virtual void Destroyed() override;

	//UFUNCTION()
	//virtual void OnVolumeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//TObjectPtr<UBoxComponent> Sphere;

	//bool IsValidOverlap(AActor* OtherActor);

};
