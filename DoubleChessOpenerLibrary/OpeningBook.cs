using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DoubleChessOpenerLibrary
{
    public class OpeningBook
    {
        public OpeningBook(string name, List<Move> moves)
        {
            Name = name;
            Moves = moves;
        }
        public string Name { get; set; }
        public List<Move> Moves { get; set; }
    }
}
