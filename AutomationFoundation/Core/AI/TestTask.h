#pragma once

UCLASS()
class ATestTask : FAsyncTaskBase
{
protected:
	virtual void DoTaskWork() override;
	virtual bool TryAbandonTask() override;

public:
	
};
