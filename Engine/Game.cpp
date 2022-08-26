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
	brd1({ 80,50 }),
	brd2({ 480, 50 }),
	mrAI(Team::BLACK, brd1, brd2),
	font("Images\\Fixedsys16x28.bmp")
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
	if (gameIsOver)
	{
		if (playerTurn == Team::BLACK)
		{
			font.DrawText("White Wins!", { 375, 400 }, Colors::White, gfx);
		}
		else
		{
			font.DrawText("Black Wins!", { 375, 400 }, Colors::White, gfx);
		}
		return;
	}
	if (playerTurn == Team::BLACK)
	{
		mrAI.HandleMoveEvent(curSelection == BoardSelection::Board1);
		TestForCheckmate();
		if(!gameIsOver)
			playerTurn = Team::WHITE;
	}
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			Vei2 loc = wnd.mouse.GetPos();
			OnClick(loc);
			if (brd1.turnSwap)
			{
				TestForCheckmate();
				playerTurn = playerTurn == Team::WHITE ? Team::BLACK : Team::WHITE;
				brd2.isEnPassantable = false;
				brd1.turnSwap = false;
			}
			else if (brd2.turnSwap)
			{
				TestForCheckmate();
				playerTurn = playerTurn == Team::WHITE ? Team::BLACK : Team::WHITE;
				brd1.isEnPassantable = false;
				brd2.turnSwap = false;
			}
		}
	}
}
void Game::TestForCheckmate()
{
	if (brd1.IsCheckmate() || brd2.IsCheckmate())
	{
		gameIsOver = true;
	}
}
Vei2 Game::MapToCell(const Vei2& loc)
{
	auto offSet1 = brd1.GetOffset();
	auto offSet2 = brd2.GetOffset();
	if (loc.x >= offSet1.x && loc.x <= offSet1.x + ChessBoard::boardSize && loc.y <= offSet1.y && loc.y >= offSet1.y - 240)
	{
		int xDest = (loc.x - offSet1.x) / ChessBoard::cellSize;
		int yDest = (offSet1.y - loc.y) / ChessBoard::cellSize;
		curSelection = BoardSelection::Board1;
		return { xDest,yDest };
	}
	else if (loc.x >= offSet2.x && loc.x <= offSet2.x + ChessBoard::boardSize && loc.y <= offSet2.y && loc.y >= offSet2.y - 240)
	{
		int xDest = (loc.x - offSet2.x) / ChessBoard::cellSize;
		int yDest = (offSet2.y - loc.y) / ChessBoard::cellSize;
		curSelection = BoardSelection::Board2;
		return { xDest,yDest };
	}
	//if we click outside of the board, return a dummy value
	return { 799,599 };

}

void Game::OnClick(const Vei2& loc)
{
	if (brd1.isPromoting)
	{
		if (queenPromotionArea.Contains(loc))
			brd1.HandlePromotionClick(playerTurn, MoveType::QueenPromotion);
		else if (knightPromotionArea.Contains(loc))
			brd1.HandlePromotionClick(playerTurn, MoveType::KnightPromotion);
	}
	else if (brd2.isPromoting)
	{
		if (queenPromotionArea.Contains(loc))
			brd2.HandlePromotionClick(playerTurn, MoveType::QueenPromotion);
		else if (knightPromotionArea.Contains(loc))
			brd2.HandlePromotionClick(playerTurn, MoveType::KnightPromotion);
	}
	auto gridPos = MapToCell(loc);
	if (curSelection != prevSelection)
	{
		brd1.ReleaseHighlights();
		brd2.ReleaseHighlights();
	}
	prevSelection = curSelection;
	if(curSelection == BoardSelection::Board1)
	{
		if (brd2.whiteInCheck)
			return;

		else if (ChessBoard::IsValidLoc(gridPos))
			brd1.OnClick(gridPos, playerTurn);

	}
	else
	{
		if (brd1.whiteInCheck)
			return;

		else if (ChessBoard::IsValidLoc(gridPos))
			brd2.OnClick(gridPos, playerTurn);
	}
}

void Game::ComposeFrame()
{
	brd1.Draw(gfx);
	brd2.Draw(gfx);
	if (!mrAI.MidGame())
	{
		font.DrawText("AI is playing:", { 150, 400 }, Colors::White, gfx);
		font.DrawText(mrAI.GetBookName(), { 50, 450 }, Colors::Green, gfx);
	}
	if (brd1.isPromoting || brd2.isPromoting)
	{
		font.DrawText("Select Piece To Promote Into:", { 50, 400 }, Colors::Green, gfx);
		if (playerTurn == Team::WHITE)
		{
			Queen::DrawWhite(gfx, { 550, 400 });
			Knight::DrawWhite(gfx, { 600, 400 });
		}
		else
		{
			Queen::DrawBlack(gfx, { 550, 400 });
			Knight::DrawBlack(gfx, { 600, 400 });
		}
		if (queenPromotionArea.Contains(wnd.mouse.GetPos()))
		{
			gfx.DrawBorder(queenPromotionArea, Colors::Blue, 3);
		}
		else if (knightPromotionArea.Contains(wnd.mouse.GetPos()))
		{
			gfx.DrawBorder(knightPromotionArea, Colors::Blue, 3);
		}
	}
}
