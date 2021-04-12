// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/GCObject.h"

/**
 * 
 */
template<typename TypeName>
class PROJECT_TIMEWARS_API UObjectQueue : public FGCObject
{
	
public:
	UObjectQueue() {}
	
	~UObjectQueue() {}

	void Empty()
	{
		FScopeLock Lock(&CriticalSection);	// Exclusive write
		ActionsQueue.Empty();
	}

	bool Dequeue(TypeName& OutItem)
	{
		FScopeLock Lock(&CriticalSection);	// Exclusive write

		if (ActionsQueue.Num() > 0)
		{
			OutItem = ActionsQueue.Pop();
			return true;
		}

		return false;
	}

	void Enqueue(const TypeName& Item)
	{
		FScopeLock Lock(&CriticalSection);	// Exclusive write
		
		ActionsQueue.EmplaceAt(0, Item);
	}

	bool IsEmpty() const { return ActionsQueue.Num() == 0;	}

	bool Peek(TypeName& OutItem) const
	{
		if (!IsEmpty()) {
			OutItem = ActionsQueue.Top();
			return true;
		}
	
		return false;
	}

	TypeName* Peek() { return ActionsQueue.Num() > 0 ? &ActionsQueue.Top() : nullptr; }

	bool Pop()
	{
		TypeName* temp;
		return Dequeue(temp);
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		for (TypeName el : ActionsQueue)
		{
			Collector.AddReferencedObject(el);
		}
	}
	
private:
	UPROPERTY()
	TArray<TypeName> ActionsQueue;

	/** Critical sections manager */
	FCriticalSection CriticalSection;
};
