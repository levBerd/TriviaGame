using Gui.Infastracture;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;

namespace Gui.view.Pages
{
    /// <summary>
    /// Interaction logic for HighScores.xaml
    /// </summary>
    public partial class HighScores : Page
    {
        private Communicator? m_communicator;
        public HighScores(Communicator? communicator)
        {
            InitializeComponent();
            m_communicator = communicator;

            byte[] response = m_communicator.sendMessage(Serializer.getHighScoresRequest());

            if (response[0] == (int)stutusId.Failed)
            {
                ErrorResponse? errorResponse = Deserializer.DeserializeResponse<ErrorResponse>(response);
                MessageBox.Show(errorResponse?.message, "Opps..", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            HighScoreResponse? personalStatsResponse = Deserializer.DeserializeResponse<HighScoreResponse>(response);
            if (personalStatsResponse?.status == 1)
            {
                UserControls.TriviaBox[] scoreTriviaBoxes = { TriviaBoxScore1, TriviaBoxScore2, TriviaBoxScore3, TriviaBoxScore4, TriviaBoxScore5 };

                for (int i = 0; i < 5; i++)
                {
                    if (i < personalStatsResponse?.statistics?.Count)
                    {
                        scoreTriviaBoxes[i].Placeholder = personalStatsResponse?.statistics[i];
                    }
                    else
                    {
                        scoreTriviaBoxes[i].Placeholder = "Not enough users...";
                    }
                }
            }
            else
            {
                MessageBox.Show("Server did not return any stats ????", "Um...", MessageBoxButton.OK, MessageBoxImage.Information);
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
