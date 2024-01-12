﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackInterface.h"
#include "ABCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};


UCLASS()
class UNREALARENABATTLE_API AABCharacterBase : public ACharacter, public IABAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();


protected:
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

protected:
	// Combo Action Section (for 몽타주)

	// 몽타주 만든거 불러오려고 만든 변수 (AM_ComboAttack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// 데이터에셋 만든거 불러오려고 만든 변수 (ABA_ComboAttack)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;

	void ProcessComboCommand();

	// 몽타주가 시작될 때 호출하는 함수
	void ComboActionBegin();

	// 몽타주가 끝날 때 호출, 얘는 몽타주에 설정된 델리게이트를 통해 호출되도록 파라미터 줬음
	// UAnimMontage 클래스 들어가보면 DECLARE_DELEGATE_TwoParams( FOnMontageEnded, class UAnimMontage*, bool /*bInterrupted*/) 
	// 저거랑 똑같이 맞춘것
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// 타이머를 발동 시킬 함수
	void SetComboCheckTimer();

	// 타이머가 발동 되면 입력이 들어왔는지 체크하는 함수
	void ComboCheck();



	// 현재 콤보가 어디까지 진행됐는지 파악하려고
	int32 CurrentCombo = 0;
	
	// 언리얼 엔진에서 제공하는 타이머 함수
	FTimerHandle ComboTimerHandle;

	// 타이머 이전에 입력 커맨드가 들어왔는지 체크
	bool HasNextComboCommand = false;


	
protected:
	// Attack Hit Section
	virtual void AttackHitCheck() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// AActor에 있음

protected:
	// Dead Section

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;
};
