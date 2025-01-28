using Gui.Infastracture;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Gui.view.Pages
{
    /// <summary>
    /// Interaction logic for JoinRoom.xaml
    /// </summary>
    /// 

    enum RoomState
    {
        INACTIVE,
        ACTIVE,
        GAME_STARTED
    }

    public partial class JoinRoom : Page
    {
        private Communicator? m_communicator;
        private Thread? refreshPageThread;
        private bool needRefresh;
        public JoinRoom(Communicator? communicator)
        {
            InitializeComponent();
            m_communicator = communicator;
            needRefresh = true;
            refreshPageThread = new Thread(refreshPage);
            refreshPageThread.Start();

            this.Loaded += Room_Loaded;
        }

        private void Room_Loaded(object sender, RoutedEventArgs e)
        {
            Console.WriteLine("Room page has been navigated into");
            needRefresh = true;
            refreshPageThread = new Thread(refreshPage);
            refreshPageThread.Start();
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

                List<RoomInfo> listContent = new List<RoomInfo>();

                byte[] response = m_communicator.sendMessage(Serializer.SerializeAllRequests(0x04, ""));

                GetRoomsResponse roomsResponse = Deserializer.DeserializeResponse<GetRoomsResponse>(response);

                if (roomsResponse != null && roomsResponse.rooms != null)
                {
                    foreach (var room in roomsResponse.rooms)
                    {
                        // get players in room
                        PlayersInRoomResponse? playersInRoom = Deserializer.DeserializeResponse<PlayersInRoomResponse>(m_communicator?.sendMessage(Serializer.SerializeRequest(new GetPlayersRequest(room.id))));
                        if (playersInRoom == null)
                        {
                            continue;
                        }
                        if (playersInRoom.status != (int)stutusId.Success)
                        {
                            continue;
                        }

                        string roomName = room.name;
                        string hostName = playersInRoom?.players[0];
                        string numPlayers = playersInRoom.players.Count.ToString() + "/" + room.maxPlayers.ToString();


                        listContent.Add(new RoomInfo(roomName, hostName, numPlayers, room.id, room.status));
                    }
                }
                
                this.Dispatcher.Invoke(() =>
                {
                    listBoxRooms.ItemsSource = listContent;
                });

                Thread.Sleep(700);
            }

        }

        private class RoomInfo
        {
            public string RoomName { get; set; }
            public string AdminName { get; set; }
            public string NumberOfPeople { get; set; }
            public uint roomid { get; set; }

            public RoomInfo(string roomName, string adminName, string numberOfPeople, uint roomid, uint roomStatus)
            {
                if ((RoomState)roomStatus == RoomState.GAME_STARTED)
                {
                    this.RoomName = roomName  + ", game started!";
                }
                else
                {
                    this.RoomName = roomName;
                }
                AdminName = adminName;
                NumberOfPeople = numberOfPeople;
                this.roomid = roomid;
            }

            public override string ToString()
            {
                return this.RoomName + ", admin:" + this.AdminName + ", number of people: " + this.NumberOfPeople;
            }
        }

        private void BackBTN_Click(object sender, MouseButtonEventArgs e)
        {
            if (NavigationService.CanGoBack)
            {
                needRefresh = false;
                NavigationService.GoBack();
            }
            else
            {
                MessageBox.Show("There's no previous page to navigate back to.");
            }
        }

        private void Grid_MouseEnter(object sender, MouseEventArgs e)
        {
            if (sender is Grid grid)
            {
                Color color = (Color)ColorConverter.ConvertFromString("#008080");
                Brush brush = new SolidColorBrush(color);
                ChangePolygonColor(grid, brush); // change to hover colors
            }
        }

        private void Grid_MouseLeave(object sender, MouseEventArgs e)
        {
            if (sender is Grid grid)
            {
                Color color = (Color)ColorConverter.ConvertFromString("#009999");
                Brush brush = new SolidColorBrush(color);
                ChangePolygonColor(grid, brush); // change from hover colors
            }
        }

        private void ChangePolygonColor(Grid grid, Brush color)
        {
            var polygon = grid.FindName("polygonElement") as Polygon;
            var polygon2 = grid.FindName("polygonElement2") as Polygon;
            var textBox = grid.FindName("MainBox") as Grid;

            // Change its fill color
            if (polygon != null)
            {
                polygon.Fill = color;
            }
            if (polygon2 != null)
            {
                polygon2.Fill = color;
            }
            if (textBox != null)
            {
                textBox.Background = color;
            }
        }

        private void roomClicked(object sender, MouseButtonEventArgs e)
        {
            Grid grid = (Grid)sender;
            RoomInfo ri = (RoomInfo)grid.DataContext;

            Byte[] response = m_communicator?.sendMessage(Serializer.SerializeRequest(new JoinRoomRequest(ri.roomid)));

            // join room
            if (response[0] == (int)stutusId.Failed)
            {
                ErrorResponse? errorResponse = Deserializer.DeserializeResponse<ErrorResponse>(response);
                MessageBox.Show(errorResponse?.message, "Opps..", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            JoinRoomResponse? joinRoomResponse = Deserializer.DeserializeResponse<JoinRoomResponse>(response);
            if (joinRoomResponse == null)
            {
                MessageBox.Show("Whops.. something went wrong");
            }
            if (joinRoomResponse.status != (int)stutusId.Success)
            {
                MessageBox.Show("server did not like that you pressed this button, please try again");
            }
            else
            {
                needRefresh = false;
                Room roomPage= new Room(m_communicator, ri.RoomName.ToString(), false);
                NavigationService.Navigate(roomPage);
            }
        }
    }
}
