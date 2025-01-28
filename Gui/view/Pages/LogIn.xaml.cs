using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Gui.Infastracture;

namespace Gui.view.Pages
{
    public partial class LogIn : Page
    {
        private Communicator? m_communicator;

        public LogIn(Communicator? communicator)
        {
            m_communicator = communicator;
            InitializeComponent();
        }
        private void LogInBTN_Click(object sender, RoutedEventArgs e)
        {
            string username = clearableTextBoxUsername.Text;
            string password = clearableTextBoxPassword.Text;

            byte[] response = m_communicator.sendMessage(Serializer.SerializeRequest(new LoginRequest(username, password)));

            if (response[0] == (int)stutusId.Failed)
            {
                ErrorResponse? errorResponse = Deserializer.DeserializeResponse<ErrorResponse>(response);
                MessageBox.Show(errorResponse?.message, "Opps..", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            LoginResponse? loginResponse = Deserializer.DeserializeResponse<LoginResponse>(response);
            if (loginResponse?.status == 1)
            {
                MainMenu menuPage = new MainMenu(m_communicator, username);
                NavigationService.Navigate(menuPage);
            }
            else
            {
                MessageBox.Show("Login Failed", "Login", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            
        }
        private void SignUpBTN_Click(object sender, RoutedEventArgs e)
        {
            SignUp signUpPage = new SignUp(m_communicator);
            NavigationService.Navigate(signUpPage);
        }
    }
}
