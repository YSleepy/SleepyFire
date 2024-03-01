#include"SleepyFireVerbMessage.h"

FString FSleepyFireVerbMessage::ToString() const
{
	FString HumanReadableMessage;
	FSleepyFireVerbMessage::StaticStruct()->ExportText(/*out*/ HumanReadableMessage, this, /*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
	return HumanReadableMessage;
}
