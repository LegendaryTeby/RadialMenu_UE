#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_ActorTest.generated.h"

UCLASS()
class MENURADIAL_API AA_ActorTest : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Test")
	TArray<AActor*> Actors = { };
	UPROPERTY(EditAnywhere, Category = "Test")
	float Radius = 150;
	UPROPERTY(EditAnywhere, Category = "Test", meta = (ClampMin = "0.01", ClampMax = "360.0"))
	float Angle = 360;
	UPROPERTY(EditAnywhere, Category = "Test", meta = (ClampMin = "-180.0", ClampMax = "180.0"))
	float Offset = 0;

	UPROPERTY(EditAnywhere, Category = "Test")
	bool UseDebug = true;
	
public:	
	AA_ActorTest();
	
private:
	UFUNCTION(CallInEditor, Category = "Test") void Test();

	virtual bool ShouldTickIfViewportsOnly() const override { return UseDebug; }

	virtual void Tick(float DeltaSeconds) override;

	void Debug();
};
