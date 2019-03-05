// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrolRouteComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto Component = ControlledPawn->FindComponentByClass<UPatrolRouteComponent>();
	if (!Component) { return EBTNodeResult::Failed; }
	auto PatrolRoute = Cast<UPatrolRouteComponent>(Component);
	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0) { 
		UE_LOG(LogTemp, Warning, TEXT("Guard is missing patrol points"));
		return EBTNodeResult::Failed; 
	}
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComponent->SetValueAsObject(WayponitKey.SelectedKeyName, PatrolPoints[Index]);
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComponent->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
	// UE_LOG(LogTemp, Warning, TEXT("Waypoint Index: %i"), Index);
	return EBTNodeResult::Succeeded;
}