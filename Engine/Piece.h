#pragma once
#include "Graphics.h"

enum class Team
{
	WHITE,
	BLACK
};
class Piece
{
public:
	Piece(Team t, const Surface& surf, const Vei2& pos);
	virtual void Draw(Graphics& gfx, const Vei2& loc) const = 0;
	bool IsCroissant() const;
	virtual void Update(const Vei2& loc);
	const Vei2& GetPosition() const;
	void SetPosition(const Vei2& loc);
	const Vei2& GetEnCroissantSquare() const;
	const Vei2& GetEnCroissantPawnLoc() const;
	Team GetTeam() const;
private:
	Team team;
protected:
	bool IsValidLoc(const Vei2& loc);
	const Surface& s;
	Vei2 pos;
	inline static Vei2 EnCroissantSquare;
	inline static Vei2 EnCroissantPawn;
	inline static bool isEnCroissantable;
};