// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"
#include "Announcement.h"

void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();


}

void ABlasterHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();

	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

void ABlasterHUD::AddAnnouncement()
{
	APlayerController* PlayerController = GetOwningPlayerController();

	if (PlayerController && AnnouncementClass)
	{
		Announcement = CreateWidget<UAnnouncement>(PlayerController, AnnouncementClass);
		Announcement->AddToViewport();
	}
}


void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		if (HUDPackage.CrosshairCenter)
			DrawCrosshair(HUDPackage.CrosshairCenter, ViewportCenter,
				FVector2D(0.f, 0.f), HUDPackage.CrosshairColor);
		if (HUDPackage.CrosshairLeft)
			DrawCrosshair(HUDPackage.CrosshairLeft, ViewportCenter,
				FVector2D(-SpreadScaled, 0.f), HUDPackage.CrosshairColor);
		if (HUDPackage.CrosshairRight)
			DrawCrosshair(HUDPackage.CrosshairRight, ViewportCenter,
				FVector2D(SpreadScaled, 0.f), HUDPackage.CrosshairColor);
		if (HUDPackage.CrosshairTop)
			DrawCrosshair(HUDPackage.CrosshairTop, ViewportCenter,
				FVector2D(0.f, -SpreadScaled), HUDPackage.CrosshairColor);
		if (HUDPackage.CrosshairBottom)
			DrawCrosshair(HUDPackage.CrosshairBottom, ViewportCenter,
				FVector2D(0.f, SpreadScaled), HUDPackage.CrosshairColor);
	}
}

void ABlasterHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter,
	FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint = FVector2D(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y);

	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y,
		TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f, CrosshairColor);
}
