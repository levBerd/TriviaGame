using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;

namespace Gui.view.UserControls
{
    /// <summary>
    /// Interaction logic for ClearableTextBox.xaml
    /// </summary>
    public partial class ClearableTextBox : UserControl, INotifyPropertyChanged
    {
        public ClearableTextBox()
        {
            DataContext = this;
            InitializeComponent();
        }

        private string placeholder;

        public event PropertyChangedEventHandler? PropertyChanged;

        public string Placeholder
        {
            get { return placeholder; }
            set 
            {
                placeholder = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Placeholder"));
            }
        }

        private int boxWidth;
        public double MainBoxWidth => BoxWidth - 90;

        public int BoxWidth
        {
            get { return boxWidth; }
            set
            {
                if (value != boxWidth)
                {
                    boxWidth = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(BoxWidth)));
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(MainBoxWidth)));
                }
            }
        }

        public string Text
        {
            get { return txtInput.Text; }
            set { txtInput.Text = value; }
        }

        private void txtInput_TextChanged(object sender, TextChangedEventArgs e)
        {
            if(string.IsNullOrEmpty(txtInput.Text))
            {
                tbPlaceholder.Visibility = Visibility.Visible;
            }
            else
            {
                tbPlaceholder.Visibility = Visibility.Hidden;
            }
        }
    }
}
