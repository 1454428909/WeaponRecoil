#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSHeroWeaponBase.h"
#include "FPSHeroWeapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;
class UDamageType;
class AFPSHeroCharacter;
class USceneComponent;
class USoundBase;
class UFPSHeroRecoilBase;
struct FHitResult;

UCLASS(BlueprintType)
class FPSHERO_API AFPSHeroWeapon : public AFPSHeroWeaponBase
{
	GENERATED_BODY()

public:
	AFPSHeroWeapon();

	virtual void Tick(float DeltaSeconds) override;

	void PlayFireEffect();
	
protected:
	virtual void SingleFire();

	void ApplyNewRecoilCameraOffset(float Pitch, float Yaw);

public:
	/* Blueprint Methods */

	virtual void Fire() override;

	virtual void EndFire(EFireEndReason Reason = EFireEndReason::MOUSE_REALEASE) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void dealHit(const FHitResult& Hit);

	/* Blueprint Attributes */

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		FName MussleName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float ShootIntervalSecond;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Recoil")
		UFPSHeroRecoilBase* RecoilInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
		UParticleSystem* MussleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
		UParticleSystem* DefaultFleshEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
		UParticleSystem* HeadshotEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
		UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float BodyDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float HeadDamage;

protected:

	//float FireTimer = 0;
	FTimerHandle FireTimer;
	bool IsFiring = false;
	// ͳ��֮ǰ������ӵ��������ڵ��ں�������ֹͣ�������˥��
	float CurrentFiredAmmo = 0;
	// ͳ��ͣ�����Ϣ��ʱ��
	float SecondsSinceStopFire = 0;
	// ��¼ֹͣ����ʱ���ۼ�����ӵ���
	float FiredAmmoWhenStop = 0;
	// ͳ�ƾ�ͷƫ����
	float PitchOffset = 0, YawOffset = 0;
	// ��¼ֹͣ����ʱ�ľ�ͷƫ��
	float PitchOffsetWhenStop = 0, YawOffsetWhenStop = 0;
};

