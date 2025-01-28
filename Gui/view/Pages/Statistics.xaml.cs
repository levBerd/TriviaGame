using Gui.Infastracture;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;

namespace Gui.view.Pages
{
    /// <summary>
    /// Interaction logic for Statistics.xaml
    /// </summary>
    public partial class Statistics : Page
    {
        private Communicator? m_communicator;
        public Statistics(Communicator? communicator)
        {
            InitializeComponent();
            m_communicator = communicator;

            byte[] response = m_communicator.sendMessage(Serializer.getPersonalStatsRequest());

            if (response[0] == (int)stutusId.Failed)
            {
                ErrorResponse? errorResponse = Deserializer.DeserializeResponse<ErrorResponse>(response);
                MessageBox.Show(errorResponse?.message, "Opps..", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            PersonalStatsResponse? personalStatsResponse = Deserializer.DeserializeResponse<PersonalStatsResponse>(response);
            if (personalStatsResponse?.status == 1)
            {
                TriviaBoxAverageTimeToAnswer.Placeholder = personalStatsResponse?.statistics[0];
                TriviaBoxTotalQuestionsCorrect.Placeholder = personalStatsResponse?.statistics[1];
                TriviaBoxTotalQuestions.Placeholder = personalStatsResponse?.statistics[2];
                TriviaBoxTotalGames.Placeholder = personalStatsResponse?.statistics[3];
                TriviaBoxScore.Placeholder = personalStatsResponse?.statistics[4];
            }
            else
            {
                MessageBox.Show("Server did not return any stats ", "Um...", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            
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
    }
}
