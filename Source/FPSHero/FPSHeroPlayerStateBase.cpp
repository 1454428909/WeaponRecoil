// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHeroPlayerStateBase.h"

#include "FPSHeroCharacter.h"
#include "FPSHeroGameMode.h"
#include "Net/UnrealNetwork.h"

AFPSHeroPlayerStateBase::AFPSHeroPlayerStateBase()
{
	bReplicates = true;
}

int AFPSHeroPlayerStateBase::GetMoney() const
{
	return Money;
}

void AFPSHeroPlayerStateBase::SetMoney(int NewMoney)
{
	if(GetLocalRole() == ENetRole::ROLE_Authority)
	{
		this->Money = NewMoney;
		OnRep_Money();
	}
}

void AFPSHeroPlayerStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSHeroPlayerStateBase, Money);
	DOREPLIFETIME(AFPSHeroPlayerStateBase, TeamID);
}

void AFPSHeroPlayerStateBase::SetTeam(int Team)
{
	if(GetLocalRole() == ENetRole::ROLE_Authority)
		TeamID = Team;
}

int AFPSHeroPlayerStateBase::GetTeam() const
{
	return TeamID;
}

APlayerController* AFPSHeroPlayerStateBase::GetPlayerController()
{
	for(auto It = GetWorld()->GetPlayerControllerIterator(); It;++It)
	{
		if(It->IsValid())
		{
			if(this == (*It)->GetPlayerState<AFPSHeroPlayerStateBase>())
			{
				return It->Get();
			}
		}
	}
	return nullptr;
}

void AFPSHeroPlayerStateBase::OnRep_Money()
{
	if(GetPlayerController()){
		if(AFPSHeroHUD* HUD = Cast<AFPSHeroHUD>(GetPlayerController()->GetHUD()))
		{
			HUD->CharacterUpdate();
		}
	}
}

void AFPSHeroPlayerStateBase::Birth_Implementation()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		AFPSHeroGameMode* GameMode = Cast<AFPSHeroGameMode>(GetWorld()->GetAuthGameMode());
		if(GameMode)
		{
			APlayerController* Controller = GetPlayerController();
			if(Controller)
			{
				//��ɾ��֮ǰ��
				APawn* Pawn = Controller->GetPawn();
				if(Pawn && Pawn->IsValidLowLevel())
					Pawn->Destroy();
				do
				{
					FTransform Transform;
					GameMode->GetPlayerBirthTransform(this, Transform);
					GameMode->RestartPlayerAtTransform(Controller, Transform);
					Pawn = Controller->GetPawn();	
				}
				while (!Pawn); // ���ǵ�����λ�ñ��赲����Ҫ��γ���
				AFPSHeroCharacter* Character = Cast<AFPSHeroCharacter>(Pawn);
				Character->OnControllerChanged();
			}
		}	
	}
	
}
