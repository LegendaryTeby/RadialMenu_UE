#include "RadialWidget.h"

#include "Components/CanvasPanelSlot.h"

#define LOG(_msg, ...) UE_LOG(LogTemp, Warning, TEXT(_msg), ##__VA_ARGS__)

void URadialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeRadialWidget();
}

void URadialWidget::InitializeRadialWidget()
{
	Viewport = GetWorld()->GetGameViewport();
	
	GetRadialElements();
	
	const int& _max = RadialElements.Num();
	AngleParse = RadialAngle / _max;

	ElementInfos.Empty();
	for (int32 i = 0; i < _max; ++i)
	{
		if (URadialElementWidget* _widget = RadialElements[i])
		{
			if (UCanvasPanelSlot* _slot = Cast<UCanvasPanelSlot>(_widget->Slot))
			{
				const float _angleD = RadialOffset + AngleParse * i + AngleParse / 2.0f;
				const float _angleR = FMath::DegreesToRadians(_angleD);
				_slot->SetAnchors(ElementAnchors);
				_slot->SetAlignment(ElementAlignment);
				_slot->SetPosition(GetRadialPosition(_angleR, RadialRadius));
				
				const float& _angle =	_angleD > 360.0f ? _angleD - 360.0f :
										_angleD < 0 ? 360.0f + _angleD : _angleD;
				
				ElementInfos.Add(_widget, FRadialInfo(_angle, AngleParse));
			}
		}
	}
}
void URadialWidget::GetRadialElements()
{
	const TArray<UWidget*>& _widgets = RadialPanel->GetAllChildren(); 
	const int _maxT = _widgets.Num();

	RadialElements.Empty();
	for (int i = 0; i < _maxT; i++)
	{
		if (URadialElementWidget* _widget = Cast<URadialElementWidget>(_widgets[i]))
			RadialElements.Add(_widget);
	}	
}

void URadialWidget::UpdateRadialWidget(const float& _x, const float& _y)
{
	if (!SelectionActive) return;

	CurrentAngle = GetAngle(_x, _y);
	if (RadialCenterImage) RadialCenterImage->SetRenderTransformAngle(CurrentAngle);

	UpdateRadialSelection(CurrentAngle);
}
void URadialWidget::UpdateRadialWidgetRaw(const float& _cursorPositionX, const float& _cursorPositionY)
{
	if (!SelectionActive || !Viewport) return;
	
	FVector2D _viewportSize;
	Viewport->GetViewportSize(_viewportSize);
	
	const FVector2D& _anchorsInViewport = RadialAnchorsInViewport.Maximum - (RadialAnchorsInViewport.Maximum - RadialAnchorsInViewport.Minimum) / 2.0f;	//Get centered Radial Widget anchors in Viewport
	const FVector2D& _widgetPosition = _viewportSize * _anchorsInViewport;		//Widget position from Anchors in Viewport & Viewport Size
		
	const float& _x = _widgetPosition.X - _cursorPositionX;
	const float& _y = _widgetPosition.Y - _cursorPositionY;
		
	UpdateRadialWidget(_x, _y);
}

void URadialWidget::UpdateRadialSelection(const float& _angle)
{
	if (URadialElementWidget* _widget = GetRadialElementWidget(_angle))
	{
		if (_widget != CurrentWidget)
		{
			if (CurrentWidget)
				CurrentWidget->UnHoverWidgetInternal();
		
			CurrentWidget = _widget;
			CurrentWidget->HoverWidgetInternal();
		}
	}
}

URadialElementWidget* URadialWidget::GetRadialElementWidget(const float& _angle)
{
	for (const auto& _current : ElementInfos)
	{
		const FRadialInfo& _info = _current.Value;
		
		if (_info.IsAngleInRange(_angle))
			return _current.Key;
	}

	return nullptr;
}

void URadialWidget::StartRadialSelection()
{
	SelectionActive = true;
	OnStartSelection.Broadcast();
}
void URadialWidget::StopRadialSelection()
{
	SelectionActive = false;	
	OnStopSelection.Broadcast();
}

void URadialWidget::SelectRadial()
{
	if (CurrentWidget)
	{
		CurrentWidget->SelectWidgetInternal();
	}

	OnSelection.Broadcast(CurrentWidget);
}

void URadialWidget::ClearRadialSelection()
{
	if (CurrentWidget)
	{
		CurrentWidget->UnHoverWidgetInternal();
	}

	CurrentWidget = nullptr;
	OnClearSelection.Broadcast();
}

float URadialWidget::GetAngle(const float& _x, const float& _y) const
{
	return FMath::RadiansToDegrees(FMath::Atan2(_y, _x)) + 180.0;		// Atan2 result is [-180, +180]. + 180 set the result to [0, 360]
}
FVector2D URadialWidget::GetRadialPosition(const float& _angle, const float& _radius) const
{
	const float _x = FMath::Cos(_angle) * _radius;
	const float _y = FMath::Sin(_angle) * _radius;
	return FVector2D(_x, _y);
}