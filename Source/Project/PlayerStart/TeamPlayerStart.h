// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Project/BlasterTypes/Team.h"
#include "TeamPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ATeamPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	ETeam Team;
};
