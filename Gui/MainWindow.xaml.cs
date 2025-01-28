using Gui.Sounds;
using System.ComponentModel;
using System.Windows;

namespace Gui
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        public MainWindow()
        {
            InitializeComponent();
            Main.Content = new view.Pages.SelectServer();
            AudioManager.Instance.PlayBackgroundMusic(@"..\..\..\Sounds\MenuMusic.wav");
        }

        private void Window_Closing(object sender, CancelEventArgs e)
        {
        }

        private void Window_MouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (e.ClickCount == 2)
            {
                btnMaximize_Click(sender, e);
                return;
            }

            if (WindowState == WindowState.Maximized)
            {
                var mousePosition = e.GetPosition(this);
                double percentHorizontal = mousePosition.X / ActualWidth;
                double percentVertical = mousePosition.Y / ActualHeight;

                WindowState = WindowState.Normal;

                Top = mousePosition.Y - (percentVertical * RestoreBounds.Height);
                Left = mousePosition.X - (percentHorizontal * RestoreBounds.Width);
            }

            DragMove();
        }

        private void btnMinimize_Click(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
        }

        private void btnMaximize_Click(object sender, RoutedEventArgs e)
        {
            if(WindowState == WindowState.Maximized)
            {
                WindowState = WindowState.Normal;
            }
            else
            {
                WindowState = WindowState.Maximized;
            }
        }

        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }
}