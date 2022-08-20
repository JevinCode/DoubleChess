﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DoubleChessOpenerLibrary
{
    public class Move : IEquatable<Move>
    {
        public Move(int board, int sX, int sY, int dX, int dY)
        {
            Board = board;
            SourceX = sX;
            SourceY = sY;
            DestX = dX;
            DestY = dY;
        }
        public bool Equals(Move other)
        {
            if (other.Board == Board && other.SourceX == SourceX && other.SourceY == SourceY && other.DestX == DestX && other.DestY == DestY)
                return true;
            return false;
        }
        public override bool Equals(object obj) => Equals(obj as Move);
        public override int GetHashCode()
        {
            int boardHash = Board.GetHashCode();
            int sXHash = SourceX.GetHashCode();
            int syHash = SourceY.GetHashCode();
            int dXHash = DestX.GetHashCode();
            int dYHash = DestY.GetHashCode();
            return boardHash ^ sXHash ^ syHash ^ dXHash ^ dYHash;
        }
        public int Board { get; set; }
        public int SourceX { get; set; }
        public int SourceY { get; set; }
        public int DestX { get; set; }
        public int DestY { get; set; }
        public string DisplayMove { 
            get
            {
                return $"{Board}:\t({SourceX},{SourceY}), ({DestX},{DestY})";
            }
        }
    }
}
