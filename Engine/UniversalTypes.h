#pragma once
#include "Vec2.h"

enum class MoveType
{
	Normal,
	EnPassant,
	KingsideCastle,
	QueensideCastle
};

struct _Move
{
	_Move() = default;
	_Move(MoveType t)
		:
		type(t)
	{}
	_Move(const Vei2& src, const Vei2& dest, MoveType t)
		:
		src(src),
		dest(dest),
		type(t)
	{}
	bool operator==(const _Move& other)
	{
		return this->src == other.src && this->dest == other.dest && this->type == other.type;
	}
	MoveType type = MoveType::Normal;
	Vei2 src = { -1,-1 };
	Vei2 dest = { -1,-1 };
};