#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Image.h"

#include "RadialElementWidget.h"

#include "RadialWidget.generated.h"

USTRUCT(BlueprintType)
struct FRadialInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	float Angle = 0;
	UPROPERTY(VisibleAnywhere)
	float MinAngle = 0;
	UPROPERTY(VisibleAnywhere)
	float MaxAngle = 0;

	FRadialInfo() { }

	FRadialInfo(const float _angle, const float _parse) : Angle(_angle)
	{
		const float _min = _angle - _parse / 2.0f;
		MinAngle = _min < 0 ? 360.0f + _min : _min;
		const float _max = _angle + _parse / 2.0f;
		MaxAngle = _max > 360.0f ? _max - 360.0f : _max;
	}

	FRadialInfo(const float& _angle, const float& _min, const float& _max) : Angle(_angle), MinAngle(_min), MaxAngle(_max) { }

	bool IsAngleInRange(const float& _angle) const
	{
		if (MinAngle > MaxAngle)
		{
			if (_angle > MinAngle && _angle <= 360.0 || _angle < MaxAngle && _angle >= 0)		//Check if Cursor Angle is between Min - 360 or 0 - Max
				return true;
		}
		else if (_angle < MaxAngle && _angle >= MinAngle)										//Else Check if Cursor is in Element Angle Range 
			return true;

		return false;
	}
};

UCLASS()
class MENURADIAL_API URadialWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelection, URadialElementWidget*, _widget);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClearSelection);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartSelection);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopSelection);
	
protected:
	//	Canvas Panel that contains all Radial Element Widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCanvasPanel* RadialPanel = nullptr;
	//	Optional Image (Rotate toward Radial Element)
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UImage* RadialCenterImage = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Radial Widget | Content")
	TArray<URadialElementWidget*> RadialElements = { };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Radial Widget | Radial Settings", meta = (ClampMin = "0.01", ClampMax = "360"))
	float RadialAngle = 360.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Radial Widget | Radial Settings")
	float RadialRadius = 150;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Radial Widget | Radial Settings", meta = (ClampMin = "-180", ClampMax = "180"))
	float RadialOffset = 0;
	//	Position of the Radial Widget in the Viewport (need to be manually set)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Radial Widget | Element Settings")
	FAnchors RadialAnchorsInViewport = FAnchors(0.5f);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Radial Widget | Element Settings")
	FAnchors ElementAnchors = FAnchors(0.5f);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Radial Widget | Element Settings")
	FVector2D ElementAlignment = FVector2D(0.5f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Radial Widget | Default Values")
	bool SelectionActive = true;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Radial Widget | Values")
	float AngleParse = 0.0f;
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentAngle = 0;
	UPROPERTY(BlueprintReadonly)
	URadialElementWidget* CurrentWidget = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<URadialElementWidget*, FRadialInfo> ElementInfos = { };

	UPROPERTY(BlueprintAssignable)
	FOnSelection OnSelection;
	UPROPERTY(BlueprintAssignable)
	FOnClearSelection OnClearSelection;
	UPROPERTY(BlueprintAssignable)
	FOnStartSelection OnStartSelection;
	UPROPERTY(BlueprintAssignable)
	FOnStopSelection OnStopSelection;

	UPROPERTY()
	UGameViewportClient* Viewport = nullptr;
	
public:
	FORCEINLINE float GetCursorAngle() const { return CurrentAngle; }

	FORCEINLINE FOnStartSelection& OnStartSelectionEvent() { return OnStartSelection; }
	FORCEINLINE FOnStopSelection& OnStopSelectionEvent() { return OnStopSelection; }
	
	FORCEINLINE FOnSelection& OnSelectionEvent() { return OnSelection; }
	FORCEINLINE FOnClearSelection& OnClearSelectionEvent() { return OnClearSelection; }
	
private:
	virtual void NativeConstruct() override;
	
	UFUNCTION(CallInEditor, Category = "Radial Widget | Utils") void InitializeRadialWidget();
	void GetRadialElements();

public:
	//	Update Radial Widget from X/Y coordinates
	UFUNCTION(BlueprintCallable) void UpdateRadialWidget(const float& _x, const float& _y);
	//	Update Radial Widget with Raw Position on screen
	UFUNCTION(BlueprintCallable) void UpdateRadialWidgetRaw(const float& _cursorPositionX, const float& _cursorPositionY);
private:
	void UpdateRadialSelection(const float& _angle);

	//	Return the Radial Element in range of the given Angle
	URadialElementWidget* GetRadialElementWidget(const float& _angle);
	
public:
	//	Allow to Start Radial Tick (if disabled and Widget is Visible in Viewport)
	UFUNCTION(BlueprintCallable) void StartRadialSelection();
	//	Allow to Stop Radial Tick even if the Widget is still Visible in the Viewport
	UFUNCTION(BlueprintCallable) void StopRadialSelection();

	//	Will Call SelectWidget on current RadialElementWidget (if any)
	UFUNCTION(BlueprintCallable) void SelectRadial();

	//	Will Unhover current RadialElementWidget (if any) and set to nullptr
	UFUNCTION(BlueprintCallable) void ClearRadialSelection();
	
private:
	float GetAngle(const float& _x, const float& _y) const;
	FVector2D GetRadialPosition(const float& _angle, const float& _radius) const;
};
