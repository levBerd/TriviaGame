using Gui.Infastracture;
using Gui.Sounds;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;
namespace Gui.view.Pages
{
    public partial class EndGame : Page
    {
        private Communicator? m_communicator;

        public EndGame(List<PlayerResult> listContent, Communicator? communicator)
        {
            InitializeComponent();
            List<PlayerResult> orderedList = listContent.OrderByDescending(player => player.correctAnswerCount).ToList();
            listBoxPlayers.ItemsSource = orderedList;
            m_communicator = communicator;
            AudioManager.Instance.PlayBackgroundMusic(@"..\..\..\Sounds\MenuMusic.wav");
        }

        private void BackBTN_Click(object sender, MouseButtonEventArgs e)
        {
            byte[] response = m_communicator.sendMessage(Serializer.SerializeAllRequests(0xE, "")); // leave room request

            LeaveGameResponse? leaveResponse = Deserializer.DeserializeResponse<LeaveGameResponse>(response);

            if (leaveResponse.status == 1) 
            {

                while (NavigationService.CanGoBack)
                {
                    NavigationService.GoBack();
                }
                NavigationService.GoForward();
                NavigationService.GoForward();
                
            }
            else
            {
                ErrorResponse? errorREsponse = Deserializer.DeserializeResponse<ErrorResponse>(response);
                MessageBox.Show(errorREsponse?.message);
            }
            
        }
    }
}
