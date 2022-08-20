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
            List<OpeningBook> sameNames = books.Where(x => x.Name == book.Name).ToList();
            List<OpeningBook> sameMoves = books.Where(x => x.Moves == book.Moves).ToList();
            if(sameNames.Count > 0 || sameMoves.Count > 0)
            {
                throw new Exception("Opening already exists.");
            }
            string line = book.Name;
            foreach(Move m in book.Moves)
            {
                line += $",{m.Board}|{m.SourceX}|{m.SourceY}|{m.DestX}|{m.DestY}|";
            }

            File.AppendAllText(OpeningBooksFile.FullFilePath(), line + Environment.NewLine);
        }

        private const string OpeningBooksFile = "OpeningBooks.csv";
    }

}
