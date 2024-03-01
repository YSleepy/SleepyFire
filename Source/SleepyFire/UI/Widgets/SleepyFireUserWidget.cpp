// Copyright XSleepy. All Rights Reserved.


#include "UI/Widgets/SleepyFireUserWidget.h"

void USleepyFireUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
