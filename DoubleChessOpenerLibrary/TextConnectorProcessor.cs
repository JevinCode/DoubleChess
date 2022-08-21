using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DoubleChessOpenerLibrary.TextHelpers
{
    public static class TextConnectorProcessor
    {
        public static string FullFilePath(this string fileName)
        {
            return $"{ConfigurationManager.AppSettings["filePath"]}\\{fileName}";
        }
        public static List<string> LoadFile(this string file)
        {
            if (!File.Exists(file))
            {
                return new List<string>();
            }

            return File.ReadAllLines(file).ToList();
        }

        public static List<OpeningBook> ConvertToOpeningBooks(this List<string> lines)
        {
            List<OpeningBook> books = new List<OpeningBook>();

            foreach (string line in lines)
            {
                //opening book is stored as name,int|int|int|int,int|int|int|int,...,kb,...,int|int|int|int
                List<Move> moves = new List<Move>();
                string[] cols = line.Split(',');
                string name = cols[0];
                for(int i = 1; i < cols.Length; i++)
                {
                    string[] coords = cols[i].Split('|');
                    if (coords.Count() < 3)
                        moves.Add(new Move(int.Parse(coords[0]), coords[1]));
                    else moves.Add(new Move(int.Parse(coords[0]), int.Parse(coords[1]), int.Parse(coords[2]), int.Parse(coords[3]), int.Parse(coords[4])));
                }
                OpeningBook ob = new OpeningBook(name, moves);
                books.Add(ob);
            }

            return books;
        }
    }
}
