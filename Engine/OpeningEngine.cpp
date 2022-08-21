#include "OpeningEngine.h"
#include <cassert>
#include "ChiliWin.h"
#include <debugapi.h>

OpeningEngine::OpeningEngine()
{
	std::ifstream inFile(OpeningBooks);
	std::vector<std::string> lines;
	std::string line;
	if (inFile.is_open())
	{
		while (std::getline(inFile, line))
		{
			lines.push_back(line);
		}
		inFile.close();
	}
	for (std::string& s : lines)
	{
		std::vector<std::string> components = StringSplitter::split(s, ',');
		std::string name = components[0];
		OpeningMove m;
		std::vector<OpeningMove> opener;
		for (int i = 1; i < components.size(); i++)
		{
			std::vector<std::string> coords = StringSplitter::split(components[i], '|');
			m.isBoard1 = std::stoi(coords[0]) == 1 ? true : false;
			if (coords.size() < 3)
			{
				std::string castleInstruction = coords[1];
				if (castleInstruction == "kb")
				{
					m.move = { {4,7}, {0,0}, MoveType::KingsideCastle };
				}
				else if (castleInstruction == "qb")
				{
					m.move = { {4,7}, {0,0}, MoveType::QueensideCastle };
				}
				else if (castleInstruction == "kw")
				{
					m.move = { {4,0},{0,0}, MoveType::KingsideCastle };
				}
				else if (castleInstruction == "qw")
				{
					m.move = { {4,0},{0,0}, MoveType::QueensideCastle };
				}
			}
			else
			{
				m.move = { {std::stoi(coords[1]), std::stoi(coords[2])}, {std::stoi(coords[3]), std::stoi(coords[4])}, MoveType::Normal };
			}
			opener.push_back(m);

		}
		auto ob = std::make_shared<OpeningBook>(name, opener);
		openingBooks.push_back(ob);
	}
}

void OpeningEngine::CullBookVector(const OpeningMove& move)
{
	for (auto it = openingBooks.begin(); it != openingBooks.end();)
	{
		if (it->get()->moves.size() == 0 || it->get()->moves[0] != move)
		{
			it = openingBooks.erase(it);
		}
		else
		{
			it->get()->moves.erase(it->get()->moves.begin());
			if (it->get()->moves.size() == 0)
			{
				it = openingBooks.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}

OpeningEngine::OpeningBook OpeningEngine::SelectBook(std::mt19937& rng, const OpeningMove& move)
{
	CullBookVector(move);
	if (openingBooks.empty())
	{
		outOfMoves = true;
		std::vector<OpeningMove> mvs;
		return { std::string("Empty"), mvs };
	}
	std::uniform_int_distribution dist(0, (int)openingBooks.size() - 1);
	int idx = dist(rng);
	assert(idx < openingBooks.size());
	auto retval = OpeningBook(*openingBooks[dist(rng)]);
	if (retval.moves.size() == 0)
	{
		OutputDebugString(L"Houston, we have a problem");
	}
	assert(retval.moves.size() > 0);
	CullBookVector(retval.moves[0]);
	return retval;
}

bool OpeningEngine::OutOfMoves() const
{
	return outOfMoves;
}