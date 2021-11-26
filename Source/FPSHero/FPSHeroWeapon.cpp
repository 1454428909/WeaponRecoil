#include "FPSHeroWeapon.h"
#include "FPSHero.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "ButtonBoxComponent.h"
#include "FPSHeroCharacter.h"
#include "FPSHeroRecoilBase.h"
AFPSHeroWeapon::AFPSHeroWeapon()
{
	MussleName = "MuzzleSocket";

	BodyDamage = 50;
	HeadDamage = 100;

	this->PrimaryActorTick.bCanEverTick = true;
	ShootIntervalSecond = 1;
	Mode = FireMode::Single;
}

void AFPSHeroWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(!IsFiring){
		// ֹͣ����󣬼���������ָ���׼�Ļָ�
		SecondsSinceStopFire += DeltaSeconds;
		// �������ָ�
		if (CurrentFiredAmmo > 0 && RecoilInstance) {
			CurrentFiredAmmo = FGenericPlatformMath::Max(0.0f, 
				FGenericPlatformMath::CeilToFloat((RecoilInstance->RecoilRestoreTime - SecondsSinceStopFire) 
					/ RecoilInstance->RecoilRestoreTime * FiredAmmoWhenStop));
		}
		// ׼�Ļָ�
		float RestoreRatio = 1;
		if (RecoilInstance)
			RecoilInstance->GetCameraRestoreRatio(SecondsSinceStopFire, RestoreRatio);
		float TargetPicth = PitchOffsetWhenStop * (1 - RestoreRatio);
		float TargetYaw = YawOffsetWhenStop * (1 - RestoreRatio);
		ApplyNewRecoilCameraOffset(TargetPicth, TargetYaw);
	}
}

void AFPSHeroWeapon::Fire()
{
	IsFiring = true;
	if(Mode == FireMode::Auto && ShootIntervalSecond > 0)
		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AFPSHeroWeapon::SingleFire, ShootIntervalSecond, true, ShootIntervalSecond);
	SingleFire();
}

void AFPSHeroWeapon::EndFire(EFireEndReason Reason)
{
	IsFiring = false;
	SecondsSinceStopFire = 0;
	FiredAmmoWhenStop = CurrentFiredAmmo;
	PitchOffsetWhenStop = PitchOffset;
	YawOffsetWhenStop = YawOffset;
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}

void AFPSHeroWeapon::dealHit_Implementation(const FHitResult& Hit)
{
	EPhysicalSurface physType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

	UParticleSystem* EffectToPlay;

	FVector eyeLoc;
	FRotator eyeRot;
	Owner->GetActorEyesViewPoint(eyeLoc, eyeRot);

	switch (physType) {
	case PHYSMAT_FLESH: //default flesh
		EffectToPlay = DefaultFleshEffect;
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), BodyDamage, Hit.ImpactPoint - eyeLoc, Hit, GEngine->GetFirstLocalPlayerController(GetWorld()), this, DamageType);
		break;
	case PHYSMAT_FLESHVULNERABLE: //vulnerable flesh
		EffectToPlay = HeadshotEffect;
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), HeadDamage, Hit.ImpactPoint - eyeLoc, Hit, GEngine->GetFirstLocalPlayerController(GetWorld()), this, DamageType);
		break;
	default:
		EffectToPlay = DefaultImpactEffect;
		UButtonBoxComponent* box = Cast<UButtonBoxComponent>(Hit.Component);
		if (box)
			box->HitButton(Owner->GetController());
		break;
	}

	if (EffectToPlay) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EffectToPlay, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	}

	if (Owner) {
		Owner->PlayMontage(FireMontage);
	}
}

void AFPSHeroWeapon::PlayFireEffect()
{
	//ǹ����Ч
	if (MussleEffect) {
		UGameplayStatics::SpawnEmitterAttached(MussleEffect, MeshComp, MussleName);
	}
	//��Ч
	if(FireSound)
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	APlayerController* Controller = Cast<APlayerController>(Owner->GetController());

	if (RecoilInstance) {
		// ��ͷ����
		RecoilInstance->ApplyCameraShake(CurrentFiredAmmo, Controller);
		// ��ͷ�ƶ�
		float RecoilPitch, RecoilYaw;
		RecoilInstance->GetCameraMovement(CurrentFiredAmmo, RecoilPitch, RecoilYaw);
		ApplyNewRecoilCameraOffset(RecoilPitch, RecoilYaw);
	}
}

void AFPSHeroWeapon::SingleFire()
{
	CurrentFiredAmmo++;
	if (Owner) {
		// #TODO �ڵ����˳�����ǹ������
		// �õ��������λ�úͳ���
		FVector eyeLoc;
		FRotator eyeRot;
		Owner->GetActorEyesViewPoint(eyeLoc, eyeRot);
		FVector eyeDir = eyeRot.Vector();

		/* ��������ɢ */
		// ��������ɢ
		// ���ȵõ���ͷ��Up��Right��������������ϵ�µķ����������ٳ��϶�Ӧ�������ɢϵ��
		eyeDir.Normalize();
		FVector UpDir(0, 0, 1);
		FVector RightDir = FVector::CrossProduct(eyeDir, UpDir);
		RightDir.Normalize();
		float SpreadUp=0, SpreadRight=0;
		if (RecoilInstance) {
			RecoilInstance->GetDirectionalSpread(CurrentFiredAmmo, SpreadUp, SpreadRight);
		}
		eyeDir += UpDir * SpreadUp + RightDir * SpreadRight;
		eyeDir.Normalize();
		// �����ɢ
		// ������ɢϵ������Բ׶��ȡ�������
		float SpreadScale = 0;
		if (RecoilInstance) {
			RecoilInstance->GetRandomSpread(CurrentFiredAmmo, SpreadScale);
		}
		eyeDir = UKismetMathLibrary::RandomUnitVectorInConeInRadians(eyeDir, SpreadScale);

		// �õ�����׷��Ŀ���
		FVector traceEnd = eyeLoc + eyeDir * 10000;
		FHitResult Hit;
		FCollisionQueryParams para;
		// ���Խ�ɫ��ǹģ��
		para.AddIgnoredActor(Owner);
		para.AddIgnoredActor(this);
		// ʹ�ø�����ײ����
		para.bTraceComplex = true;
		para.bReturnPhysicalMaterial = true;
		if (GetWorld()->LineTraceSingleByChannel(Hit, eyeLoc, traceEnd, TRACECHANNEL_WEAPON, para)) {
			// ��������Ч��
			dealHit(Hit);
		}
		PlayFireEffect();
	}
}

void AFPSHeroWeapon::ApplyNewRecoilCameraOffset(float Pitch, float Yaw)
{
	if (!Owner)
		return;
	APlayerController* controller = Cast<APlayerController>(Owner->GetController());
	if (controller) {
		// ȡ��ʹ����ֵΪ��ͷ����
		controller->AddPitchInput(-(Pitch - PitchOffset));
		// ȡ��ʹ����ֵΪ��ͷ����
		controller->AddYawInput(-(Yaw - YawOffset));
		PitchOffset = Pitch;
		YawOffset = Yaw;
	}
}
