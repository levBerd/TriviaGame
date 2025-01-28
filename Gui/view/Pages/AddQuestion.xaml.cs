using Gui.Infastracture;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;

namespace Gui.view.Pages
{
    /// <summary>
    /// Interaction logic for AddQuestion.xaml
    /// </summary>
    public partial class AddQuestion : Page
    {
        private Communicator? m_communicator;
        public AddQuestion(Communicator? communicator)
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
        private void Submit_Click(object sender, RoutedEventArgs e)
        {
            byte[] response = m_communicator.sendMessage(Serializer.SerializeRequest(new AddNewQuestionRequest(TextBoxQuestion.Text, TextBoxCorrectAns.Text, TextBoxWrongAns1.Text, 
                TextBoxWrongAns2.Text, TextBoxWrongAns3.Text, TextBoxDifficulty.Text, TextBoxCategory.Text)));
            if (response[0] == (int)stutusId.Failed)
            {
                ErrorResponse? errorResponse = Deserializer.DeserializeResponse<ErrorResponse>(response);
                MessageBox.Show(errorResponse?.message, "Opps..", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            MessageBox.Show("New Question added, thanks!", "Success", MessageBoxButton.OK, MessageBoxImage.Information);
            NavigationService.GoBack();
        }
    }
}
