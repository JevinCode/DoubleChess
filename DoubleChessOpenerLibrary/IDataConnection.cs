using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DoubleChessOpenerLibrary
{
    public interface IDataConnection
    {
        void CreateOpeningBook(OpeningBook book);
    }
}
