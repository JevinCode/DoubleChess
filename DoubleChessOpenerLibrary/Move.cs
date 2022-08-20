using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DoubleChessOpenerLibrary
{
    public class Move
    {
        public Move(int board, int sX, int sY, int dX, int dY)
        {
            Board = board;
            SourceX = sX;
            SourceY = sY;
            DestX = dX;
            DestY = dY;
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
