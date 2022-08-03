// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

class URocketMovementComponent;

/**
 * 
 */

UCLASS()
class PROJECT_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()
	
public:
	AProjectileRocket();

	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	virtual void OnHit(UPrimitiveComponent* HipComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;


	UPROPERTY(EditAnywhere)
	USoundCue* ProjectileLoop;

	UPROPERTY()
	UAudioComponent* ProjectileLoopComponent;

	UPROPERTY(EditAnywhere)
	USoundAttenuation* LoopingSoundAttenuation;

	UPROPERTY(VisibleAnywhere)
	URocketMovementComponent* RocketMovementComponent;

private:



};
