using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace DoubleChessOpenerLibrary
{
    public static class Config
    {
        public static IDataConnection connection = new TextConnector();
    }
}