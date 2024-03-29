using System.ComponentModel;
using DoubleChessOpenerLibrary;

namespace DoubleChessOpenerUI
{
    public partial class CreateOpeningBookForm : Form
    {
        BindingList<Move> moves = new BindingList<Move>();
        public CreateOpeningBookForm()
        {
            InitializeComponent();
            InitializeList();
        }
        private void InitializeList()
        {
            plyListBox.DataSource = moves;
            plyListBox.DisplayMember = nameof(DoubleChessOpenerLibrary.Move.DisplayMove);
        }

        private void addPlyButton_Click(object sender, EventArgs e)
        {
            if (board2CheckBox.Checked)
                board = 2;
            else
                board = 1;

            if (kingsideCastleBlackCheckBox.Checked)
            {
                moves.Add(new Move(board, "kb"));
                kingsideCastleBlackCheckBox.Checked = false;
            }
            else if (kingsideCastleWhiteCheckBox.Checked)
            {
                moves.Add(new Move(board, "kw"));
                kingsideCastleWhiteCheckBox.Checked = false;
            }
            else if (queensideCastleBlackCheckBox.Checked)
            {
                moves.Add(new Move(board, "qb"));
                queensideCastleBlackCheckBox.Checked = false;
            }
            else if (queensideCastleWhiteCheckBox.Checked)
            {
                moves.Add(new Move(board, "qw"));
                queensideCastleWhiteCheckBox.Checked = false;
            }

            else if (ValidateCoordinates())
            {
                Move m = new Move(board, sourceX, sourceY, destX, destY);
                moves.Add(m);

                sourceXTextBox.Clear();
                sourceYTextBox.Clear();
                destXTextBox.Clear();
                destYTextBox.Clear();
            }
        }

        private int board = -1;
        private int sourceX = -1;
        private int sourceY = -1;
        private int destX = -1;
        private int destY = -1;
        bool ValidateCoordinates()
        {
            string errorMsg = "";
            bool output = true;
            if(int.TryParse(sourceXTextBox.Text, out sourceX) == false)
            {
                errorMsg += "Please enter a number between 0 and 7 in the Source X field.\n";
                output = false;
            }
            if(sourceX < 0 || sourceX > 7)
            {
                errorMsg += "Please enter a number between 0 and 7 in the Source X field.\n";
                output = false;
            }
            if (int.TryParse(sourceYTextBox.Text, out sourceY) == false)
            {
                errorMsg += "Please enter a number between 0 and 7 in the Source Y field.\n";
                output = false;
            }
            if (sourceY < 0 || sourceY > 7)
            {
                errorMsg += "Please enter a number between 0 and 7 in the Source Y field.\n";
                output = false;
            }
            if (int.TryParse(destXTextBox.Text, out destX) == false)
            {
                errorMsg += "Please enter a number between 0 and 7 in the Dest X field.\n";
                output = false;
            }
            if (destX < 0 || destX > 7)
            {
                errorMsg += "Please enter a number between 0 and 7 in the Dest X field.\n";
                output = false;
            }
            if (int.TryParse(destYTextBox.Text, out destY) == false)
            {
                errorMsg += "Please enter a number between 0 and 7 in the Dest Y field.\n";
            }
            if (destY < 0 || destY > 7)
            {
                errorMsg += "Please enter a number between 0 and 7 in the Dest Y field.\n";
                output = false;
            }


            if(sourceX == destX && sourceY == destY)
            {
                errorMsg += "Destination coordinatees must be different from source coordinates!\n";
                output = false;
            }

            if(!output)
            {
                MessageBox.Show(errorMsg);
            }

            return output;
        }

        private void createBookButton_Click(object sender, EventArgs e)
        {
            if(ValidateForm())
            {
                OpeningBook ob = new OpeningBook(nameTextBox.Text, moves.ToList());
                Config.connection.CreateOpeningBook(ob);
                List<Move> otherMoves = moves.ToList();
                foreach(Move move in otherMoves)
                {
                    move.Board = move.Board == 1 ? 2 : 1;
                }
                OpeningBook ob2 = new OpeningBook(nameTextBox.Text, otherMoves);
                Config.connection.CreateOpeningBook(ob2);
                moves.Clear();
                nameTextBox.Clear();
            }
        }
        private bool ValidateForm()
        {
            bool output = true;
            if(nameTextBox.Text.Length == 0)
            {
                MessageBox.Show("Please enter a name for this book.");
                output = false;
            }
            return output;
        }

        private void removeSelectedButton_Click(object sender, EventArgs e)
        {
            Move m = (Move)plyListBox.SelectedItem;

            if(m != null)
            {
                moves.Remove(m);
            }
        }

        private void filterDuplicatesButton_Click(object sender, EventArgs e)
        {
            Config.connection.FilterDuplicates();
        }

        private void kingsideCastleBlackCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            kingsideCastleWhiteCheckBox.Checked = false;
            queensideCastleWhiteCheckBox.Checked = false;
            queensideCastleBlackCheckBox.Checked = false;
        }

        private void kingsideCastleWhiteCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            kingsideCastleBlackCheckBox.Checked = false;
            queensideCastleWhiteCheckBox.Checked = false;
            queensideCastleBlackCheckBox.Checked = false;
        }

        private void queensideCastleBlackCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            kingsideCastleWhiteCheckBox.Checked = false;
            queensideCastleWhiteCheckBox.Checked = false;
            kingsideCastleBlackCheckBox.Checked = false;
        }

        private void queensideCastleWhiteCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            kingsideCastleWhiteCheckBox.Checked = false;
            queensideCastleBlackCheckBox.Checked = false;
            kingsideCastleBlackCheckBox.Checked = false;
        }
    }
}