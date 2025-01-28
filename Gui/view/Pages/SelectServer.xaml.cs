using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;

namespace Gui.view.Pages
{
    /// <summary>
    /// Interaction logic for SelectServer.xaml
    /// </summary>
    public partial class SelectServer : Page
    {
        private Communicator? m_communicator;

        public SelectServer()
        {
            InitializeComponent();
        }

        private void Grid_MouseDown(object sender, MouseButtonEventArgs e)
        {
            m_communicator = new Communicator(clearableTextBoxIP.Text);

            if (m_communicator.IsConnected)
            {
                LogIn menuPage = new LogIn(m_communicator);
                NavigationService.Navigate(menuPage);
            }
        }
    }
}
