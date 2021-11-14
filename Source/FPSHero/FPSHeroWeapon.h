#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSHeroWeapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;
class UDamageType;
class AFPSHeroCharacter;
class USceneComponent;
class USoundBase;
class UFPSHeroRecoilBase;
struct FHitResult;

UENUM()
enum class FireMode
{
	None = 0,
	Single = 1,
	Auto = 2
};


UCLASS(BlueprintType)
class FPSHERO_API AFPSHeroWeapon : public AActor
{
	GENERATED_BODY()

public:
	AFPSHeroWeapon();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void SetOwner(AFPSHeroCharacter* MyOwner);

	void SwitchFireMode();

	void PlayFireEffect();

protected:
	virtual void SingleFire();

	void ApplyNewRecoilCameraOffset(float Pitch, float Yaw);

public:
	/* Blueprint Methods */

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void EndFire();

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void dealHit(const FHitResult& Hit);

	/* Blueprint Attributes */

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		FName MussleName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float ShootIntervalSecond;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FireMode Mode;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		bool IsFireModeLocked;

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

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
		float BodyDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
		float HeadDamage;


protected:
	UPROPERTY(EditAnywhere, Category = "Component")
		USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		USceneComponent* Root;

	AFPSHeroCharacter* Owner = nullptr;

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

