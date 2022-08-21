using DoubleChessOpenerLibrary.TextHelpers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DoubleChessOpenerLibrary
{
    public class TextConnector : IDataConnection
    {
        public void CreateOpeningBook(OpeningBook book)
        {
            List<OpeningBook> books = OpeningBooksFile.FullFilePath().LoadFile().ConvertToOpeningBooks();
            if(books.Contains(book))
            {
                throw new Exception("Sequence already exists!");
            }
            string line = book.Name;
            foreach(Move m in book.Moves)
            {
                if (m.IsCastleMove)
                    line += $",{m.Board}|{m.CastleString}";
                else
                    line += $",{m.Board}|{m.SourceX}|{m.SourceY}|{m.DestX}|{m.DestY}|";
            }
            File.AppendAllText(OpeningBooksFile.FullFilePath(), line + Environment.NewLine);
        }

        public void FilterDuplicates()
        {
            List<OpeningBook> books = OpeningBooksFile.FullFilePath().LoadFile().ConvertToOpeningBooks();
            List<OpeningBook> uniques = books.Distinct().ToList();
            List<string> lines = new List<string>();
            foreach(OpeningBook book in uniques)
            {
                string line = book.Name;
                foreach(Move m in book.Moves)
                {
                    line += $",{m.Board}|{m.SourceX}|{m.SourceY}|{m.DestX}|{m.DestY}|";
                }
                lines.Add(line);
            }
            File.WriteAllLines(OpeningBooksFile.FullFilePath(), lines);
        }

        private const string OpeningBooksFile = "OpeningBooks.csv";
    }

}
