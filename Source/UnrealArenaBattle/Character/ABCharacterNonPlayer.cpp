﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/ABAIController.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	// 메쉬가 로딩이 될 때 까진 숨기기
	GetMesh()->SetHiddenInGame(true);

	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;		// 배치된 npc 혹은 SpawnActor로 생성시킨 npc 둘 다
																// AABAIController 에 의해서 통제 되도록 함
}

void AABCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(NPCMeshes.Num() > 0);								// NPCMeshes 값이 들어왔는지 확인

	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);	// 랜덤으로 인덱스 지정

	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AABCharacterNonPlayer::NPCMeshLoadCompleted));
	// RequestAsyncLoad 로 비동기로 로드 진행, 끝날 시 호출되는 함수를 NPCMeshLoadCompleted 로 지정
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle,
		FTimerDelegate::CreateLambda(	// () => {} 같이 C++ 람다
			[&]()						// 본문을 캡쳐해서 (&)
			{
				Destroy();
			}),
		DeadEventDelayTime,				// 상속받은거 5초
		false);							// Loop 할 필요 없으니까
}

void AABCharacterNonPlayer::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid())
	{
		// 유효한 핸들이 있다면 GetLoadedAsset 사용하여 로드한 에셋 가져옴

		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			// 메쉬 지정
			GetMesh()->SetSkeletalMesh(NPCMesh);

			// 생성자에선 메쉬를 안보이게 했다가, 로딩 다 되면 보이도록 설정할거임
			GetMesh()->SetHiddenInGame(false);
		}
	}

	// 핸들 사용 후 해제
	NPCMeshHandle->ReleaseHandle();
}

float AABCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AABCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AABCharacterNonPlayer::GetAIAttackRange()
{
	return 0.0f;
}

float AABCharacterNonPlayer::GetAITurnSpeed()
{
	return 0.0f;
}
