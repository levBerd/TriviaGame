using Gui.Infastracture;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;

namespace Gui.view.Pages
{
    public partial class Room : Page
    {
        private Communicator? m_communicator;
        private Thread? refreshPageThread;
        private bool needRefresh;
        private int timePerQuestion;

        enum RoomState
        {
            INACTIVE,
	        ACTIVE,
	        GAME_STARTED
        };

        public Room(Communicator? communicator, string roomName, bool isAdmin)
        {
            InitializeComponent();
            m_communicator = communicator;

            needRefresh = true;
            refreshPageThread = new Thread(refreshPage);
            refreshPageThread.Start();

            roomNameTXT.Text = roomName;
            AdminNameTXT.Text = "waiting for data..";
            playersTXT.Text = "waiting for data..";
            QuestionCountTXT.Text = "waiting for data..";
            AnswerTimeTXT.Text = "waiting for data..";

            if (isAdmin)
            {
                backButtonGrid.Visibility = Visibility.Collapsed;
                adminPanelGrid.Visibility = Visibility.Visible;
            }
            else
            {
                backButtonGrid.Visibility = Visibility.Visible;
                adminPanelGrid.Visibility = Visibility.Collapsed;
            }
        }

        private void BackBTN_Click(object sender, MouseButtonEventArgs e)
        {
            if (NavigationService.CanGoBack)
            {
                byte[] response = m_communicator.sendMessage(Serializer.SerializeAllRequests((byte)CodeID.LeaveRoom, ""));

                LeaveRoomResponse? leaveResponse = Deserializer.DeserializeResponse<LeaveRoomResponse>(response);

                if(leaveResponse.status == 1)
                {
                    needRefresh = false;
                    NavigationService.GoBack();
                    return;
                }
            }

            MessageBox.Show("There's no previous page to navigate back to.");
        }


        private void StartGame_Click(object sender, MouseButtonEventArgs e)
        {
            byte[] response = m_communicator.sendMessage(Serializer.SerializeAllRequests((byte)CodeID.StartGameRequest, ""));
            StartGameResponse startGameResponse = Deserializer.DeserializeResponse<StartGameResponse>(response);

            if (startGameResponse.status != 1) {
                MessageBox.Show("Failed to Start room", "Start room", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            else
            {
                needRefresh = false;
                GameRoom gameRoomPage = new GameRoom(m_communicator, timePerQuestion);
                NavigationService.Navigate(gameRoomPage);
            }
        }

        private void CloseRoom_Click(object sender, MouseButtonEventArgs e)
        {
            byte[] response = m_communicator.sendMessage(Serializer.SerializeAllRequests((byte)CodeID.CloseRoomRequest, ""));
            CloseRoomResponse? serverResponse = Deserializer.DeserializeResponse<CloseRoomResponse>(response);

            if (serverResponse.status == 1)
            {
                needRefresh = false;
                NavigationService.GoBack();
                NavigationService.GoBack();
            }
            else
            {
                MessageBox.Show("Failed to Close room", "Close room", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }

        private void refreshPage()
        {
            while (needRefresh)
            {
                if (this.Dispatcher.HasShutdownFinished)
                {
                    this.Dispatcher.Invoke(() =>
                    {
                        // crush thread if window crushed
                    });
                }

                List<string> listContent = new List<string>();
                byte[] response = m_communicator.sendMessage(Serializer.SerializeAllRequests((byte)CodeID.GetRoomStateRequest, ""));

                GetRoomStateResponse? roomStateResponse = Deserializer.DeserializeResponse<GetRoomStateResponse>(response);

                if (roomStateResponse == null || roomStateResponse.status == null)
                {
                    this.Dispatcher.Invoke(() =>
                    {
                        byte[] response = m_communicator.sendMessage(Serializer.SerializeAllRequests((byte)CodeID.LeaveRoom, ""));
                        LeaveRoomResponse? leaveResponse = Deserializer.DeserializeResponse<LeaveRoomResponse>(response);

                        if (leaveResponse.status == 1)
                        {
                            needRefresh = false;
                            NavigationService.GoBack();
                            NavigationService.GoBack();
                        }
                    });
                    return;
                }

                timePerQuestion = (int)roomStateResponse.answerTimeOut;

                if (roomStateResponse.roomState == (uint)RoomState.GAME_STARTED)
                {
                    needRefresh = false;
                    this.Dispatcher.Invoke(() =>
                    {
                        GameRoom gameRoomPage = new GameRoom(m_communicator, timePerQuestion);
                        NavigationService.Navigate(gameRoomPage);
                    });
                    return;
                }
                
                if (roomStateResponse.players != null)
                {
                    foreach (string player in roomStateResponse.players)
                    {
                        listContent.Add(player);
                    }
                }

                this.Dispatcher.Invoke(() =>
                {
                    AnswerTimeTXT.Text = roomStateResponse?.answerTimeOut.ToString();
                    QuestionCountTXT.Text = roomStateResponse?.questionCount.ToString();
                    playersTXT.Text = roomStateResponse?.players?.Count.ToString();
                    listBoxPlayes.ItemsSource = listContent;
                    AdminNameTXT.Text = roomStateResponse.players[0];
                });

                Thread.Sleep(300);
            }
        }
    }
}
