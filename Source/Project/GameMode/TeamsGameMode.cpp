// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamsGameMode.h"
#include "Project/GameState/BlasterGameState.h"
#include "Project/PlayerState/BlasterPlayerState.h"
#include "Project/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"

ATeamsGameMode::ATeamsGameMode()
{
	bTeamsMatch = true;
}

void ATeamsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));

	if (BGameState)
	{
		ABlasterPlayerState* BPState = NewPlayer->GetPlayerState<ABlasterPlayerState>();
		if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
		{
			if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
			{
				BGameState->RedTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_RedTeam);
			}
			else
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_BlueTeam);
			}
		}
	}
}

void ATeamsGameMode::Logout(AController* Exiting)
{
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	ABlasterPlayerState* BPState = Exiting->GetPlayerState<ABlasterPlayerState>();
	if (BGameState && BPState)
	{
		if (BGameState->RedTeam.Contains(BPState))
			BGameState->RedTeam.Remove(BPState);

		if (BGameState->BlueTeam.Contains(BPState))
			BGameState->BlueTeam.Remove(BPState);
	}
}

float ATeamsGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	ABlasterPlayerState* AttackerPState = Attacker->GetPlayerState<ABlasterPlayerState>();
	ABlasterPlayerState* VictimPState = Victim->GetPlayerState<ABlasterPlayerState>();

	// ���� if�� �˻��ؾ� �ϳ�
	if (AttackerPState == nullptr || VictimPState == nullptr)
		return BaseDamage;

	if (VictimPState == AttackerPState)
		return BaseDamage;

	if (AttackerPState->GetTeam() == VictimPState->GetTeam())
		return 0.f;

	return BaseDamage;
}

void ATeamsGameMode::PlayerEliminated(ABlasterCharacter* EliminatedCharacter,
	ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackController)
{
	Super::PlayerEliminated(EliminatedCharacter, VictimController, AttackController);

	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	ABlasterPlayerState* AttackerPlayerState = AttackController
		?Cast<ABlasterPlayerState>(AttackController->PlayerState) : nullptr;
	if (BGameState && AttackerPlayerState)
	{
		if (AttackerPlayerState->GetTeam() == ETeam::ET_BlueTeam)
			BGameState->BlueTeamScores();

		if (AttackerPlayerState->GetTeam() == ETeam::ET_RedTeam)
			BGameState->RedTeamScores();
	}
}

void ATeamsGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	
}