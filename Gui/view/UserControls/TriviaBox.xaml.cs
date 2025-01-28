using Gui.Infastracture;
using Gui.Sounds;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Media;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Gui.view.UserControls
{
    /// <summary>
    /// Interaction logic for button.xaml
    /// </summary>
    public partial class TriviaBox : UserControl, INotifyPropertyChanged
    {
        public TriviaBox()
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

        public static readonly DependencyProperty DimmingPanelVisibilityProperty = DependencyProperty.Register(
            "DimmingPanelVisibility", typeof(Visibility), typeof(TriviaBox), new PropertyMetadata(Visibility.Collapsed));

        public Visibility DimmingPanelVisibility
        {
            get { return (Visibility)GetValue(DimmingPanelVisibilityProperty); }
            set { SetValue(DimmingPanelVisibilityProperty, value); }
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
        private string originalMainColor;

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

        private int textSize;

        public int TextSize
        {
            get { return textSize; }
            set
            {
                textSize = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("TextSize"));
            }
        }

        private void UserControl_MouseEnter(object sender, MouseEventArgs e)
        {
            originalMainColor = MainColor;
            MainColor = onChnageColor;

            HorizontalBorderWidth += 3;
            SoundPlayer player = new SoundPlayer(@"..\..\..\Sounds\button_hover.wav");
            player.Load();
            player.Play();
        }

        private void UserControl_MouseLeave(object sender, MouseEventArgs e)
        {
            MainColor = originalMainColor;
            HorizontalBorderWidth -= 3;
        }

        private void UserControl_MouseDown(object sender, MouseEventArgs e)
        {
            SoundPlayer player = new SoundPlayer(@"..\..\..\Sounds\button.wav");
            player.Load();
            player.Play();
        }

    }
}
