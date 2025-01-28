using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Gui.Infastracture;

namespace Gui.view.Pages
{
    /// <summary>
    /// Interaction logic for MainMenu.xaml
    /// </summary>
    public partial class MainMenu : Page
    {
        private Communicator? m_communicator;
        public MainMenu(Communicator? communicator, string username)
        {
            InitializeComponent();
            m_communicator = communicator;
            UserName.Text = username;
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            CreateRoom createRoom = new CreateRoom(m_communicator);
            NavigationService.Navigate(createRoom);
        }

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            JoinRoom joinRoom = new JoinRoom(m_communicator);
            NavigationService.Navigate(joinRoom);
        }
        private void Statistics_Click(object sender, RoutedEventArgs e)
        {
            Statistics statistics = new Statistics(m_communicator);
            NavigationService.Navigate(statistics);
        }

        private void HighScores_Click(object sender, RoutedEventArgs e)
        {
            HighScores highScores = new HighScores(m_communicator);
            NavigationService.Navigate(highScores);
        }
        private void oneVOne_Click(object sender, RoutedEventArgs e)
        {
            byte[] response = m_communicator.sendMessage(Serializer.SerializeAllRequests((byte)CodeID.AddUserToOneVOneWait, "")); // add player to wait room request
            AddUserToOneVOneWaitResponse? addUserToOneVOneWaitResponse = Deserializer.DeserializeResponse<AddUserToOneVOneWaitResponse>(response);

            if (addUserToOneVOneWaitResponse.state == (uint)OveVOneRoomState.LOOKING_FOR_OPPONENT)
            {
                OneVOne oneVOne = new OneVOne(m_communicator);
                NavigationService.Navigate(oneVOne);
            }
            else
            {
                GameRoom gameRoom = new GameRoom(m_communicator, 10);
                NavigationService.Navigate(gameRoom);
            }
        }
        private void AddQuestion_Click(object sender, RoutedEventArgs e)
        {
            AddQuestion addQuestion = new AddQuestion(m_communicator);
            NavigationService.Navigate(addQuestion);
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void Amount_Click(object sender, RoutedEventArgs e)
        {
            this.m_communicator.sendMessage(Serializer.SerializeAllRequests((byte)0x50, ""));
        }
    }
}
