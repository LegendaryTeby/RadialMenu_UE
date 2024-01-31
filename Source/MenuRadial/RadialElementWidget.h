#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadialElementWidget.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class MENURADIAL_API URadialElementWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class URadialWidget;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelect);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHover);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnhover);

protected:
	//Event when this Element is Selected
	UPROPERTY(BlueprintAssignable)
	FOnSelect OnSelect;
	//Event when this Element is Hovered
	UPROPERTY(BlueprintAssignable)
	FOnHover OnHover;
	//Event when this Element is Unhovered
	UPROPERTY(BlueprintAssignable)
	FOnUnhover OnUnhover;
	
public:
	FORCEINLINE FOnSelect& OnSelectEvent() { return OnSelect; }	
	FORCEINLINE FOnHover& OnHoverEvent() { return OnHover; }	
	FORCEINLINE FOnUnhover& OnUnhoverEvent() { return OnUnhover; }
	
private:
	void SelectWidgetInternal();
	void HoverWidgetInternal();
	void UnHoverWidgetInternal();
	
public:
	//Call when Radial Widget select this element
	virtual void SelectWidget();
	//Call when Radial Widget hover this element
	virtual void HoverWidget();
	//Call when Radial Widget unhover this element
	virtual void UnHoverWidget();
};
