using Gui.Infastracture;
using Gui.Sounds;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;

namespace Gui.view.Pages
{
    /// <summary>
    /// Interaction logic for OneVOne.xaml
    /// </summary>
    enum OveVOneRoomState
    {
        LOOKING_FOR_OPPONENT,
        GAME_STARTED
    }

    public partial class OneVOne : Page
    {
        private Communicator? m_communicator;
        private Thread? refreshPageThread;
        private bool needRefresh;
        public OneVOne(Communicator? communicator)
        {
            InitializeComponent();
            m_communicator = communicator;

            MainText.Text = "SEARCHING FOR OPPONENT!";

            needRefresh = true;
            refreshPageThread = new Thread(refreshPage);
            refreshPageThread.Start();
        }
        private void BackBTN_Click(object sender, MouseButtonEventArgs e)
        {
            needRefresh = false;
            m_communicator.sendMessage(Serializer.SerializeAllRequests((byte)CodeID.LeaveOneVOneWait, "")); // leave request

            if (NavigationService.CanGoBack)
            {
                AudioManager.Instance.PlayBackgroundMusic(@"..\..\..\Sounds\MenuMusic.wav");
                NavigationService.GoBack();
            }
            else
            {
                MessageBox.Show("There's no previous page to navigate back to.");
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

                byte[] response = m_communicator.sendMessage(Serializer.SerializeAllRequests((byte)CodeID.GetOneVOneState, "")); // oneVOne state request
                GetOneVOneStateResponse? getOneVOneStateResponse = Deserializer.DeserializeResponse<GetOneVOneStateResponse>(response);

                if (getOneVOneStateResponse.roomState == (uint)OveVOneRoomState.GAME_STARTED)
                {
                    needRefresh = false;
                    this.Dispatcher.Invoke(() =>
                    {
                        GameRoom gameRoomPage = new GameRoom(m_communicator, 10); // 10 seconds default for one v one
                        NavigationService.Navigate(gameRoomPage);
                    });
                }

                Thread.Sleep(300);
            }
        }
    }
}
