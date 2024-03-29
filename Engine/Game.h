/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Surface.h"
#include "ChessBoard.h"
#include "ChessAI.h"
#include "Font.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	enum class BoardSelection
	{
		Board1,
		Board2,
		NIL
	};
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
	Vei2 MapToCell(const Vei2& loc);
	void OnClick(const Vei2& loc);
	void TestForCheckmate();
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	Font font;
	ChessBoard brd1;
	ChessBoard brd2;
	Team playerTurn = Team::WHITE;
	BoardSelection curSelection = BoardSelection::NIL;
	BoardSelection prevSelection = BoardSelection::NIL;
	ChessAI mrAI;

	RectI queenPromotionArea = { {551, 400}, {581, 431} };
	RectI knightPromotionArea = { {599, 400}, {629, 430} };
	MoveType promotionPiece = MoveType::Normal;

	bool gameIsOver = false;

	/********************************/
};