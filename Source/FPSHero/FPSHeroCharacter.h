// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "FPSHero.h"
#include "GameFramework/Character.h"
#include "FPSHeroWeaponBase.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "FPSHeroCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AFPSHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSHeroCharacter();
	
	UFUNCTION(BlueprintCallable)
	AFPSHeroWeaponBase* GetWeapon();

	//#TODO 捡武器时有bug

	//#TODO 武器的动画系统
	UFUNCTION(BlueprintCallable)
		void GripWeapon(TSubclassOf<AFPSHeroWeaponBase> GrappedWeaponType);

	UFUNCTION(BlueprintCallable)
		void ThrowWeapon(EWeaponSlot WeaponSlot);

	UFUNCTION(BlueprintCallable)
		bool SwitchToWeaponSlot(EWeaponSlot WeaponSlot);

	UFUNCTION(BlueprintCallable)
		void RemoveWeapon(EWeaponSlot WeaponSlot);

	UFUNCTION(BlueprintCallable)
		void SetViewMode(EViewMode NewViewMode);

	UFUNCTION(BlueprintCallable)
		EViewMode GetViewMode();

	UFUNCTION(BlueprintCallable)
		void SwitchViewMode();

	UFUNCTION(BlueprintCallable)
		void UpdateAnimationClass();

	UFUNCTION(BlueprintCallable)
		UAnimInstance* GetAnimInstance();

	UFUNCTION(BlueprintCallable)
		void PlayMontage(UAnimMontage* Montage);

	UFUNCTION(BlueprintCallable)
		USkeletalMeshComponent* GetCurrentMesh();

	UFUNCTION(BlueprintCallable)
		bool IsTurning();

	UFUNCTION(BlueprintCallable)
	EWeaponSlot GetWeaponTypeSlot(TSubclassOf<AFPSHeroWeaponBase> WeaponType);

	UFUNCTION(BlueprintCallable)
		bool IsFiring();

	UFUNCTION(BlueprintCallable)
		FRotator GetAimingRotation();

	UFUNCTION(BlueprintCallable)
		TSubclassOf<UAnimInstance> GetDefaultAnimClass(EViewMode ViewMode);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<class AFPSHeroWeaponBase>> DefaultWeaponTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	EWeaponSlot DefaultWeaponSlot;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundBase* FireInterruptedSound;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		EViewMode DefaultViewMode;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		TSubclassOf<UAnimInstance> DefaultFPSAnimClass;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		TSubclassOf<UAnimInstance> DefaultTPSAnimClass;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	void EndFire(EFireEndReason Reason = EFireEndReason::MOUSE_REALEASE);

	void EndFireByRelease();

	void SwitchFireMode();

	void SwitchToWeaponSlot1();

	void SwitchToWeaponSlot2();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	bool IsViewBack();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsForceToFireOrientaion = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TMap<EWeaponSlot, AFPSHeroWeaponBase*> Weapons;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		FName FPSWeaponSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		FName TPSWeaponSocketName;

	EWeaponSlot GetWeaponSlot(AFPSHeroWeaponBase* Weapon);

	EWeaponSlot GetNextWeaponSlot(EWeaponSlot WeaponSlot);

	EWeaponSlot FindNextValidSlot(EWeaponSlot WeaponSlot);

	bool FindWeapon(AFPSHeroWeaponBase* Weapon, EWeaponSlot& WeaponSlot);

		 
		/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* FirstPersonCameraHolder;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* ThirdPersonSpringArmComp;

	EViewMode ViewMode;

	EWeaponSlot CurrentWeaponSlot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bIsFiring;

	bool bShouldInitAnim;

	float TurnThresholdStartAngle = 80;

	float TurnThresholdStopAngle = 30;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float TurnRate = 10;

	bool bIsTurning = false;
};


