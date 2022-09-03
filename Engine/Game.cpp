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
	mrAIWhite(Team::WHITE, brd1, brd2),
	mrAIBlack(Team::BLACK, brd1, brd2),
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
		return;
	}
	if (playerTurn == Team::BLACK)
		mrAIBlack.HandleMoveEvent(false);
	else
		mrAIWhite.HandleMoveEvent(false);

	playerTurn = playerTurn == Team::WHITE ? Team::BLACK : Team::WHITE;
	brd1.GenerateMoves(playerTurn);
	brd2.GenerateMoves(playerTurn);
	brd1.turnSwap = false;
	brd2.turnSwap = false;
	TestForCheckmate();

	
	
	//while (!wnd.mouse.IsEmpty())
	//{
	//	const auto e = wnd.mouse.Read();
	//	if (e.GetType() == Mouse::Event::Type::LPress)
	//	{
	//		Vei2 loc = wnd.mouse.GetPos();
	//		OnClick(loc);
	//		if (brd1.turnSwap)
	//		{
	//			playerTurn = playerTurn == Team::WHITE ? Team::BLACK : Team::WHITE;
	//			if(!UndoArea.Contains(loc))
	//				movesSelected.push(BoardSelection::Board1);
	//			brd2.isEnPassantable = false;
	//			brd1.GenerateMoves(playerTurn);
	//			brd2.GenerateMoves(playerTurn);
	//			brd1.turnSwap = false;
	//		}
	//		else if (brd2.turnSwap)
	//		{
	//			playerTurn = playerTurn == Team::WHITE ? Team::BLACK : Team::WHITE;
	//			if (!UndoArea.Contains(loc))
	//				movesSelected.push(BoardSelection::Board2);
	//			brd1.isEnPassantable = false;
	//			brd1.GenerateMoves(playerTurn);
	//			brd2.GenerateMoves(playerTurn);
	//			brd2.turnSwap = false;
	//		}
	//		TestForCheckmate();
	//	}
	//}
}

void Game::TestForCheckmate()
{
	if (brd1.IsCheckmate() || brd2.IsCheckmate())
	{
		gameIsOver = true;
	}
	else if (brd1.IsStalemate() || brd2.IsStalemate())
	{
		stalemate = true;
		gameIsOver = true;
	}
}
Vei2 Game::MapToCell(const Vei2& loc)
{
	auto offSet1 = brd1.GetOffset();
	auto offSet2 = brd2.GetOffset();
	if (loc.x >= offSet1.x && loc.x <= offSet1.x + ChessBoard::boardSize && loc.y >= offSet1.y && loc.y <= offSet1.y + 240)
	{
		int xDest = (loc.x - offSet1.x) / ChessBoard::cellSize;
		int yDest = 7 - (loc.y - offSet1.y) / ChessBoard::cellSize;
		curSelection = BoardSelection::Board1;
		return { xDest,yDest };
	}
	else if (loc.x >= offSet2.x && loc.x <= offSet2.x + ChessBoard::boardSize && loc.y >= offSet2.y && loc.y <= offSet2.y + 240)
	{
		int xDest = (loc.x - offSet2.x) / ChessBoard::cellSize;
		int yDest = 7 - (loc.y - offSet2.y) / ChessBoard::cellSize;
		curSelection = BoardSelection::Board2;
		return { xDest,yDest };
	}
	//if we click outside of the board, return a dummy value
	return { 799,599 };

}

void Game::OnClick(const Vei2& loc)
{
	if (UndoArea.Contains(loc))
	{
		if (movesSelected.empty())
			return;
		auto lastMoveMade = movesSelected.top();
		movesSelected.pop();
		if (lastMoveMade == BoardSelection::Board1)
		{
			brd1.RevertMove();
		}
		else if (lastMoveMade == BoardSelection::Board2)
		{
			brd2.RevertMove();
		}
		brd1.ClearHighlights();
		brd2.ClearHighlights();
		return;
	}
	if (brd1.isPromoting)
	{
		if (queenPromotionArea.Contains(loc))
		{
			brd1.HandlePromotionClick(playerTurn, MoveType::QueenPromotion);
		}
		else if (knightPromotionArea.Contains(loc))
		{
			brd1.HandlePromotionClick(playerTurn, MoveType::KnightPromotion);
			movesSelected.push(BoardSelection::Board1);
		}
		return;
	}
	else if (brd2.isPromoting)
	{
		if (queenPromotionArea.Contains(loc))
		{
			brd2.HandlePromotionClick(playerTurn, MoveType::QueenPromotion);
			movesSelected.push(BoardSelection::Board2);
		}
		else if (knightPromotionArea.Contains(loc))
		{
			brd2.HandlePromotionClick(playerTurn, MoveType::KnightPromotion);
			movesSelected.push(BoardSelection::Board2);
		}
		return;
	}
	auto gridPos = MapToCell(loc);
	if (curSelection != prevSelection)
	{
		brd1.ClearHighlights();
		brd2.ClearHighlights();
	}
	prevSelection = curSelection;
	if(curSelection == BoardSelection::Board1)
	{
		if (brd2.IsInCheck(playerTurn))
			return;

		if (ChessBoard::IsValidLoc(gridPos))
			brd1.OnClick(gridPos, playerTurn);

	}
	else
	{
		if (brd1.IsInCheck(playerTurn))
			return;

		if (ChessBoard::IsValidLoc(gridPos))
			brd2.OnClick(gridPos, playerTurn);
	}
}

void Game::ComposeFrame()
{
	brd1.Draw(gfx);
	brd2.Draw(gfx);/*
	if (!mrAI.MidGame())
	{
		font.DrawText("AI is playing:", { 150, 400 }, Colors::White, gfx);
		font.DrawText(mrAI.GetBookName(), { 50, 450 }, Colors::Green, gfx);
	}*/
	gfx.DrawSprite(600, 550, { 0,25,0,24 }, undoredo, SpriteEffect::Chroma(Colors::White));

	if (gameIsOver)
	{
		if (stalemate)
			font.DrawText("Stalemate!", { 375,400 }, Colors::White, gfx);

		else if (playerTurn == Team::BLACK)
			font.DrawText("White Wins!", { 375, 400 }, Colors::White, gfx);
		else
			font.DrawText("Black Wins!", { 375, 400 }, Colors::White, gfx);
	}
	if (brd1.isPromoting || brd2.isPromoting)
	{
		font.DrawText("Select Piece To Promote Into:", { 50, 400 }, Colors::Green, gfx);
		Piece::Draw(gfx, { 550, 400 }, Piece::PieceType::Queen, playerTurn);
		Piece::Draw(gfx, { 600, 400 }, Piece::PieceType::Knight, playerTurn);

		if (queenPromotionArea.Contains(wnd.mouse.GetPos()))
			gfx.DrawBorder(queenPromotionArea, Colors::Blue, 3);

		else if (knightPromotionArea.Contains(wnd.mouse.GetPos()))
			gfx.DrawBorder(knightPromotionArea, Colors::Blue, 3);
	}
}
