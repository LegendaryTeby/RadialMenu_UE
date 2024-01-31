#include "RadialElementWidget.h"

#define LOG(_msg, ...) UE_LOG(LogTemp, Warning, TEXT(_msg), ##__VA_ARGS__)

void URadialElementWidget::SelectWidgetInternal()
{
	OnSelect.Broadcast();
	SelectWidget();
}
void URadialElementWidget::HoverWidgetInternal()
{
	OnHover.Broadcast();
	HoverWidget();
}
void URadialElementWidget::UnHoverWidgetInternal()
{
	OnUnhover.Broadcast();
	UnHoverWidget();
}

void URadialElementWidget::SelectWidget()
{
	//...	
}
void URadialElementWidget::HoverWidget()
{
	//...
}
void URadialElementWidget::UnHoverWidget()
{
	//...
}
