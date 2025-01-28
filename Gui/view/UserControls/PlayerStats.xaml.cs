using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Gui.view.UserControls
{
    /// <summary>
    /// Interaction logic for PlayerStats.xaml
    /// </summary>
    public partial class PlayerStats : UserControl, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler? PropertyChanged;

        // Constructor
        public PlayerStats()
        {
            InitializeComponent();
            DataContext = this;
        }

        // UserName 
        private string userName;

        public string UserName
        {
            get { return userName; }
            set
            {
                userName = value;
                tbName.Text = userName;
            }
        }


        // AnswerTime 
        private string answerTime;

        public string AnswerTime
        {
            get { return answerTime; }
            set
            {
                answerTime = value;
                tbCorrect.Text = answerTime;
            }
        }

        // Correct
        private string correct;

        public string Correct
        {
            get { return correct; }
            set
            {
                correct = value;
                tbTime.Text = correct;
            }
        }



        private int boxWidth;
        public double MainBoxWidth => BoxWidth - 90;
        public double BorderWidth => BoxWidth - 50;
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
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(BorderWidth)));
                }
            }
        }

        private string mainColor;

        public string MainColor
        {
            get { return mainColor; }
            set
            {
                mainColor = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("MainColor"));
            }
        }

        private string secondaryColor;

        public string SecondaryColor
        {
            get { return secondaryColor; }
            set
            {
                secondaryColor = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("SecondaryColor"));
            }
        }

        private string onChnageColor;

        public string OnChnageColor
        {
            get { return onChnageColor; }
            set
            {
                onChnageColor = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("OnChnageColor"));
            }
        }


        private int horizontalBorderWidth;
        public double SidesBorderWidth => HorizontalBorderWidth - 0.5;
        public int HorizontalBorderWidth
        {
            get { return horizontalBorderWidth; }
            set
            {
                horizontalBorderWidth = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(HorizontalBorderWidth)));
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(SidesBorderWidth)));
            }
        }
    }
}
