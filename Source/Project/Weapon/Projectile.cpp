// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Project/Character/BlasterCharacter.h"
#include "Project/Project.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SKeletalMesh, ECollisionResponse::ECR_Block);

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (Tracer)
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(Tracer, CollisionBox,
			FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition);

	if (HasAuthority())
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

}

void AProjectile::OnHit(UPrimitiveComponent* HipComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AProjectile::SpawnTrailSystem()
{
	if (TrailSystem)
	{
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem,
			GetRootComponent(), FName(), GetActorLocation(), GetActorRotation(),
			EAttachLocation::KeepWorldPosition, false);
	}
}

void AProjectile::ExplodeDamage()
{
	APawn* FiringPawn = GetInstigator();
	if (FiringPawn && HasAuthority())
	{
		AController* FiringController = FiringPawn->GetController();
		if (FiringController)
		{
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				this, 
				Damage, 
				10.f, 
				GetActorLocation(),
				DamageInnerReadius, 
				DamageOuterReadius, 
				1.f, 
				UDamageType::StaticClass(), 
				TArray<AActor*>(), 
				this, 
				FiringController);
		}
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(DestroyTimer, this,
		&AProjectile::DestroyTimerFinished, DestroyTime);
}

void AProjectile::DestroyTimerFinished()
{
	Destroy();
}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	if (ImpactParticles)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());

	if (ImpactSound)
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
}

