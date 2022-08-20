#pragma once

#include <string>
#include "StringSplitter.h"
#include <random>
#include "Vec2.h"
class OpeningEngine
{
public:
	struct OpeningMove
	{
		OpeningMove() = default;
		OpeningMove(bool isBoard1, const Vei2& src, const Vei2& dest)
			:
			isBoard1(isBoard1),
			src(src),
			dest(dest)
		{}
		bool operator==(const OpeningMove& other)
		{
			return this->src == other.src && this->dest == other.dest && this->isBoard1 == other.isBoard1;
		}
		bool operator!=(const OpeningMove& other)
		{
			return !(*this == other);
		}
		bool isBoard1;
		Vei2 src;
		Vei2 dest;
	};
	struct OpeningBook
	{
		OpeningBook(const std::string name, std::vector<OpeningMove>& moves)
			:
			name(name),
			moves(moves)
		{}
		OpeningBook(const OpeningBook& other)
			:
			name(other.name),
			moves(other.moves)
		{}
		std::string name;
		std::vector<OpeningMove> moves;
	};
	OpeningEngine();
	OpeningBook SelectBook(std::mt19937& rng, const OpeningMove& move);
	bool OutOfMoves() const;
private:

	//removes all books where the first move in the book doesn't match the move that the opponent made.
	//for the books where the moves match, the move is removed, so that the next move at the front of the book
	//is one of the candidate moves for the AI to select.
	void CullBookVector(const OpeningMove& move);
	std::vector<std::shared_ptr<OpeningBook>> openingBooks;
	const std::string OpeningBooks = "OpeningBooks.csv";
	bool outOfMoves = false;
};