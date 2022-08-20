#include "OpeningEngine.h"

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
			m.src = Vei2{ std::stoi(coords[1]), std::stoi(coords[2]) };
			m.dest = Vei2{ std::stoi(coords[3]),std::stoi(coords[4]) };
			opener.push_back(m);

		}
		std::shared_ptr<OpeningBook> ob = std::make_shared<OpeningBook>(components[0], opener);
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
			it++;
		}
	}
}

OpeningEngine::OpeningMove OpeningEngine::SelectMove(std::mt19937& rng, const OpeningMove& move)
{
	CullBookVector(move);
	if (openingBooks.empty())
	{
		outOfMoves = true;
		return { true, {0,0},{0,0} };
	}
	std::uniform_int_distribution dist(0, (int)openingBooks.size() - 1);
	auto retval = openingBooks[dist(rng)]->moves[0];
	CullBookVector(retval);
	return retval;
}

bool OpeningEngine::OutOfMoves() const
{
	return outOfMoves;
}