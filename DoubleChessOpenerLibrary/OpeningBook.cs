using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DoubleChessOpenerLibrary
{
    public class OpeningBook : IEquatable<OpeningBook>
    {
        public OpeningBook(string name, List<Move> moves)
        {
            Name = name;
            Moves = moves;
        }
        public bool Equals(OpeningBook other)
        {
            //Check for null and compare run-time types.
            if (Moves.SequenceEqual(other.Moves))
            {
                return true;
            }
            return false;
        }
        public override bool Equals(object obj) => Equals(obj as OpeningBook);
        public override int GetHashCode()
        {
            int hash = Moves[0].GetHashCode();
            for(int i = 1; i < Moves.Count; i++)
            {
                hash ^= Moves[i].GetHashCode();
            }
            return hash;
        }
        public string Name { get; set; }
        public List<Move> Moves { get; set; }
    }
}
