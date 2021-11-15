#pragma once

#include "FPSHero.h"
#include "CoreMinimal.h"
#include "UObject\Object.h"
#include "FPSHeroRecoilBase.generated.h"

class APlayerController;

UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, Category = "Weapon")
class FPSHERO_API UFPSHeroRecoilBase : public UObject
{
	GENERATED_BODY()
public:
	UFPSHeroRecoilBase();

	// ��ȡ����������̵�׼��ƫ����
	UFUNCTION(BlueprintImplementableEvent, Category = "Recoil")
		void GetCameraMovement(float AmmoIndex, float& pitch, float& yaw);

	// ��ȡ����������̵ķ�����׼����ɢ
	UFUNCTION(BlueprintImplementableEvent, Category = "Recoil")
		//Spread value equal to 1 means 45��spread
		void GetDirectionalSpread(float AmmoIndex, float& SpreadUp, float& SpreadRight);

	// ��ȡ����������̵����׼����ɢ
	UFUNCTION(BlueprintImplementableEvent, Category = "Recoil")
		//Spread value equal to 1 means 45��spread
		void GetRandomSpread(float AmmoIndex, float& scale);

	// ��ȡ���������ľ�ͷ��ʱ��ص���
	UFUNCTION(BlueprintImplementableEvent, Category = "Recoil")
		//From 0 to 1 with TimeSinceStop. Totally restored when return 1.
		void GetCameraRestoreRatio(float TimeSinceStop, float& RestoreRatio);

	// ������ͷ����
	UFUNCTION(BlueprintImplementableEvent, Category = "Recoil")
		void ApplyCameraShake(float AmmoIndex, APlayerController* controller);

	// �������ָ�ʱ��
	UPROPERTY(EditAnywhere, Category = "Recoil")
		float RecoilRestoreTime = 0.5;
};