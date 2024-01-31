#include "A_ActorTest.h"

AA_ActorTest::AA_ActorTest()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AA_ActorTest::Test()
{
	const int& _max = Actors.Num();
	const float& _angle = 360.0f / _max;
	for (int i = 0; i < _max; i++)
	{
		if (AActor* _current = Actors[i])
		{
			const float _angleR = FMath::DegreesToRadians(_angle * i);
			const float _x = FMath::Cos(_angleR) * Radius;
			const float _y = FMath::Sin(_angleR) * Radius;
			_current->SetActorLocation(FVector(_x, _y, 0));
		}
	}
}

void AA_ActorTest::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Debug();
}

void AA_ActorTest::Debug()
{
	UWorld* W = GetWorld();
	DrawDebugCircle(W, GetActorLocation(), Radius, 50, FColor::Green, false, -1, 0, 1, FVector(0, 1, 0), FVector(1, 0, 0));
	const float _startAngle = FMath::DegreesToRadians(0 + Offset);
	const FVector& _startLocation = GetActorLocation() + FVector(FMath::Cos(_startAngle) * Radius, FMath::Sin(_startAngle) * Radius, 0);
	const float _endAngle = FMath::DegreesToRadians(Angle + Offset);
	const FVector& _endLocation = GetActorLocation() + FVector(FMath::Cos(_endAngle) * Radius, FMath::Sin(_endAngle) * Radius, 0); 
	DrawDebugSphere(GetWorld(), _startLocation, 5, 10, FColor::Blue);
	DrawDebugSphere(GetWorld(), _endLocation, 5, 10, FColor::Red);
	
	const int& _max = Actors.Num();
	const float& _parse = Angle / _max;
	for (int i = 0; i < _max; i++)
	{
		const float _angleD = Offset + _parse * i + _parse / 2.0f;
		const float _angleR = FMath::DegreesToRadians(_angleD);
		const float _x = FMath::Cos(_angleR) * Radius;
		const float _y = FMath::Sin(_angleR) * Radius;
		const FVector& _location = GetActorLocation() + FVector(_x, _y, 0);
		DrawDebugSphere(GetWorld(), _location, 10, 10, FColor::Green);
	}
	
	//GetWorldTimerManager().SetTimer();
	//GetWorldTimerManager().ClearTimer();
}

