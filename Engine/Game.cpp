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
	gfx( wnd ),
	brd1({ 30,30 }),
	brd2({ 430, 30 })
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
	if (loc.x >= brd1.GetOffset().x && loc.x <= brd1.GetOffset().x + ChessBoard::boardSize && loc.y >= brd1.GetOffset().y && loc.y <= brd1.GetOffset().y + ChessBoard::boardSize)
	{
		int xDest = (loc.x - brd1.GetOffset().x) / ChessBoard::cellSize;
		int yDest = (loc.y - brd1.GetOffset().y) / ChessBoard::cellSize;
		curSelection = BoardSelection::Board1;
		return { xDest,yDest };
	}
	else if(loc.x >= brd2.GetOffset().x && loc.x <= brd2.GetOffset().x + ChessBoard::boardSize && loc.y >= brd2.GetOffset().y && loc.y <= brd2.GetOffset().y + ChessBoard::boardSize)
	{
		int xDest = (loc.x - brd2.GetOffset().x) / ChessBoard::cellSize;
		int yDest = (loc.y - brd2.GetOffset().y) / ChessBoard::cellSize;
		curSelection = BoardSelection::Board2;
		return { xDest,yDest };
	}
	return { 799,599 };

}

void Game::OnClick(const Vei2& loc)
{
	auto gridPos = MapToCell(loc);
	if (curSelection != prevSelection)
	{
		brd1.ReleaseHighlights();
		brd2.ReleaseHighlights();
	}
	prevSelection = curSelection;
	if(curSelection == BoardSelection::Board1)
	{
		if (brd1.isPromoting && (gridPos == Vei2{ 8,0 } || gridPos == Vei2{ 8,1 }))
			brd1.OnClick(gridPos, playerTurn);

		else if (ChessBoard::IsValidLoc(gridPos))
			brd1.OnClick(gridPos, playerTurn);

		if (brd1.turnSwap)
		{
			playerTurn = playerTurn == Team::WHITE ? Team::BLACK : Team::WHITE;
			brd1.turnSwap = false;
		}
	}
	else
	{
		if (brd2.isPromoting && (gridPos == Vei2{ 8,0 } || gridPos == Vei2{ 8,1 }))
			brd2.OnClick(gridPos, playerTurn);

		else if (ChessBoard::IsValidLoc(gridPos))
			brd2.OnClick(gridPos, playerTurn);

		if (brd2.turnSwap)
		{
			playerTurn = playerTurn == Team::WHITE ? Team::BLACK : Team::WHITE;
			brd2.turnSwap = false;
		}
	}
}

void Game::ComposeFrame()
{
	brd1.Draw(gfx);
	brd2.Draw(gfx);
	if (brd1.isPromoting)
	{
		if (playerTurn == Team::WHITE)
		{
			Queen::DrawWhite(gfx, brd1);
			Knight::DrawWhite(gfx, brd1);
		}
		else
		{
			Queen::DrawBlack(gfx, brd1);
			Knight::DrawBlack(gfx, brd1);
		}
	}

	else if (brd2.isPromoting)
	{
		if (playerTurn == Team::WHITE)
		{
			Queen::DrawWhite(gfx, brd1);
			Knight::DrawWhite(gfx, brd1);
		}
		else
		{
			Queen::DrawBlack(gfx, brd1);
			Knight::DrawBlack(gfx, brd1);
		}
	}
}
