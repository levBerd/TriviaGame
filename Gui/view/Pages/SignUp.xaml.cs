using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;
using Gui.Infastracture;

namespace Gui.view.Pages
{
    /// <summary>
    /// Interaction logic for SignUp.xaml
    /// </summary>
    public partial class SignUp : Page
    {
        Communicator? m_communicator;
        public SignUp(Communicator? communicator)
        {
            InitializeComponent();
            m_communicator = communicator;
        }

        private void BackBTN_Click(object sender, MouseButtonEventArgs e)
        {
            if (NavigationService.CanGoBack)
            {
                NavigationService.GoBack();
            }
            else
            {
                MessageBox.Show("There's no previous page to navigate back to.");
            }
        }

        private void Grid_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if(secondClearableTextBoxPassword.Text != clearableTextBoxPassword.Text)
            {
                MessageBox.Show("Passwords doesn't match", "Signup", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            Address address = new Address(
                clearableTextBoxStreet.Text,
                int.TryParse(clearableTextBoxApt.Text, out int apt) ? (int?)apt : 0,
                clearableTextBoxCity.Text
            );

            SignupRequest signupRequest = new SignupRequest(
                clearableTextBoxUsername.Text,
                clearableTextBoxPassword.Text,
                clearableTextBoxEmail.Text,
                address,
                clearableTextBoxBirth.Text,
                clearableTextBoxPhone.Text
            );

            byte[]? response = m_communicator?.sendMessage(Serializer.SerializeRequest(signupRequest));

            if (response[0] == (int)stutusId.Failed)
            {
                ErrorResponse? errorResponse = Deserializer.DeserializeResponse<ErrorResponse>(response);
                MessageBox.Show(errorResponse?.message, "Opps..", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            SignupResponse? signupResponse = Deserializer.DeserializeResponse<SignupResponse>(response);
            if (signupResponse?.status == 1)
            {
                MainMenu menuPage = new MainMenu(m_communicator, clearableTextBoxUsername.Text);
                NavigationService.Navigate(menuPage);
            }
            else
            {
                MessageBox.Show("Signup Falied", "Signup", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }
    }
}
