// Fill out your copyright notice in the Description page of Project Settings.


#include "JSX_Javascript.h"

void UJSX_Javascript::BeginDestroy()
{
	DestroyContextAndIsolate();
	Super::BeginDestroy();
}

void UJSX_Javascript::DestroyContextAndIsolate()
{ 
	if (JavascriptIsolate)
	{
		JavascriptIsolate->ConditionalBeginDestroy();
	}
	if (JavascriptContext)
	{
		JavascriptContext->ConditionalBeginDestroy();
	}

	GetWorld()->ForceGarbageCollection(true);
}


void UJSX_Javascript::Activate(bool bReset)
{
	if (bExecuteSuperActivate)
	{
		Super::Activate(bReset);
	}
}