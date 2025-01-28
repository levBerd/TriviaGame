using Gui.Infastracture;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;

namespace Gui.view.Pages
{
    /// <summary>
    /// Interaction logic for CreateRoom.xaml
    /// </summary>
    public partial class CreateRoom : Page
    {
        private Communicator? m_communicator;
        public CreateRoom(Communicator? communicator)
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

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            string name = TextBoxRoomName.Text;
            int players;
            int time;
            int quiestionCount;

            bool isPlayersNumeric = int.TryParse(TextBoxRoomPlayers.Text, out players);
            bool isTimeNumeric = int.TryParse(TextBoxRoomTime.Text, out time);
            bool isQuestionsNumeric = int.TryParse(TextBoxQuestionCount.Text, out quiestionCount);

            if (isPlayersNumeric && isTimeNumeric && isQuestionsNumeric)
            {
                byte[] response = m_communicator.sendMessage(Serializer.SerializeRequest(new CreateRoomRequest(name, players, quiestionCount, time)));

                if (response[0] == (int)stutusId.Failed)
                {
                    ErrorResponse? errorResponse = Deserializer.DeserializeResponse<ErrorResponse>(response);
                    MessageBox.Show(errorResponse?.message, "Opps..", MessageBoxButton.OK, MessageBoxImage.Information);
                }
                else
                {
                    CreateRoomResponse? createRoomResponse = Deserializer.DeserializeResponse<CreateRoomResponse>(response);
                    if (createRoomResponse?.status == 1)
                    {
                        Room roomPage = new Room(m_communicator, TextBoxRoomName.Text, true);
                        NavigationService.Navigate(roomPage);
                    }
                    else
                    {
                        MessageBox.Show("Room creation Falied", "Room", MessageBoxButton.OK, MessageBoxImage.Information);
                    }
                }
            }
            else
            {
                if (!isPlayersNumeric)
                {
                    MessageBox.Show("Invalid input for players. Please enter a valid number.");
                }
                if (!isTimeNumeric)
                {
                    MessageBox.Show("Invalid input for time. Please enter a valid number.");
                }
                if (!isQuestionsNumeric)
                {
                    MessageBox.Show("Invalid input for questions. Please enter a valid number.");
                }
            }

        }
    }
}
