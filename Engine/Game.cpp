/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include <random>
#include "SpriteEffect.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			Vei2 loc = wnd.mouse.GetPos();
			OnClick(loc);
		}
	}
}

Vei2 Game::MapToCell(const Vei2& loc)
{
	//if we click outside of the board, return a dummy value
	if (loc.x < brd.GetOffset().x || loc.x > brd.GetOffset().x + ChessBoard::boardSize || loc.y < brd.GetOffset().y || loc.y > brd.GetOffset().y + ChessBoard::boardSize)
		return { 799,599 };
	int xDest = (loc.x - brd.GetOffset().x) / ChessBoard::cellSize;
	int yDest = (loc.y - brd.GetOffset().y) / ChessBoard::cellSize;
	return {xDest,yDest};
}

void Game::OnClick(const Vei2& loc)
{
	auto gridPos = MapToCell(loc);
	if (brd.isPromoting && (gridPos == Vei2{ 8,0 } || gridPos == Vei2{ 8,1 }))
		brd.OnClick(gridPos, playerTurn);

	else if (brd.IsValidLoc(gridPos))
		brd.OnClick(gridPos, playerTurn);

	if (brd.turnSwap)
	{
		playerTurn = playerTurn == Team::WHITE ? Team::BLACK : Team::WHITE;
		brd.turnSwap = false;
	}
}

void Game::ComposeFrame()
{
	brd.Draw(gfx);
	if (brd.isPromoting)
	{
		if (playerTurn == Team::WHITE)
		{
			Queen::DrawWhite(gfx, brd);
			Knight::DrawWhite(gfx, brd);
		}
		else
		{
			Queen::DrawBlack(gfx, brd);
			Knight::DrawBlack(gfx, brd);
		}
	}
}
