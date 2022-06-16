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
	const Surface& s;
	Vei2 pos;
	inline static Vei2 EnCroissantSquare;
	inline static Vei2 EnCroissantPawn;
	inline static bool isEnCroissantable;
};

class Pawn : public Piece
{
public:
	Pawn(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
	void Update(const Vei2& loc) override;
private:
	int numMoves = 0;
};

class Bishop : public Piece
{
public:
	Bishop(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};

class Knight : public Piece
{
public:
	Knight(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};

class Rook : public Piece
{
public:
	Rook(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};

class Queen : public Piece
{
public:
	Queen(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};

class King : public Piece
{
public:
	King(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};