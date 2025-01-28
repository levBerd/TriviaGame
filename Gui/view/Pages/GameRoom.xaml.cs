using Gui.Infastracture;
using Gui.Sounds;
using Gui.view.UserControls;
using System.Media;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;

namespace Gui.view.Pages
{
    /// <summary>
    /// Interaction logic for GameRoom.xaml
    /// </summary>
    /// 

    enum GameState
    {
        NotStarted = 1,
        WaitingForAnswers,
        WaitingForNewQuestion,
        GameEnded
    }

    public partial class GameRoom : Page
    {
        private const string correctAnswerColor = "#33cc33";
        private const string correctAnswerSecondaryColor = "#70db70";
        private const string wrongAnswerColor = "#ff0000";
        private const string wrongAnswerSecondaryColor = "#ff3333";

        private Communicator? m_communicator;
        private Thread? refreshPageThread;
        private bool needRefresh;
        private int answerChosen;
        bool soundPlayed = false;

        private SortedDictionary<int, int> answers; // button to question id
        private SortedDictionary<int, UserControls.TriviaBox> answerButtons;

        private GameState gameState;
        
        private int questionMaxTime;
        public GameRoom(Communicator? communicator, int timePerQuestion)
        {
            InitializeComponent();

            answerButtons = new SortedDictionary<int, UserControls.TriviaBox> { { 1, Ans1BTN }, { 2, Ans2BTN }, { 3, Ans3BTN }, { 4, Ans4BTN } };
            answers = new SortedDictionary<int, int> { };
            answerChosen = -1;
            m_communicator = communicator;
            needRefresh = true;
            gameState = GameState.NotStarted;
            refreshPageThread = new Thread(refreshPage);
            refreshPageThread.Start();
            this.questionMaxTime = timePerQuestion;
            AudioManager.Instance.PlayBackgroundMusic(@"..\..\..\Sounds\Game_Music.wav"); // here is the initialization
            AudioManager.Instance.PauseBackgroundMusic();
        }

        private void refreshPage()
        {
            List<int> randomQuestions = new List<int>{ 1, 2, 3, 4 };

            while (needRefresh)
            {

                if (this.Dispatcher.HasShutdownFinished)
                {
                    this.Dispatcher.Invoke(() =>
                    {
                        // crush thread if window crushed
                    });
                }

                byte[] response = m_communicator.sendMessage(Serializer.SerializeRequest(new GetGameStateRequest((uint)answerChosen)));
                GameStateResponse gameStateResponse = Deserializer.DeserializeResponse<GameStateResponse>(response);

                if (gameStateResponse.status == (uint)stutusId.Failed)
                {
                    Thread.Sleep(200);
                    continue;
                }

                if(gameStateResponse.status == (uint)GameState.NotStarted)
                {
                    MakeGameStartScreen(gameStateResponse.playerNames);
                    gameState = (GameState)gameStateResponse.status;
                    Thread.Sleep(200);
                    continue;
                }

                if(gameState == GameState.NotStarted)
                {
                    // game just started
                    this.Dispatcher.Invoke(() =>
                    {
                        MainGameBox.Visibility = Visibility.Visible;
                        TimerBox.Visibility = Visibility.Visible;
                        MainMessageBox.Visibility = Visibility.Collapsed;
                    });
                }
                if (gameStateResponse.status == (uint)GameState.WaitingForNewQuestion)
                {
                    //first message for wating
                    this.Dispatcher.Invoke(() =>
                    {
                        DimmingPanel.Visibility = Visibility.Collapsed;
                        Ans1BTN.DimmingPanelVisibility = Visibility.Collapsed;
                        Ans2BTN.DimmingPanelVisibility = Visibility.Collapsed;
                        Ans3BTN.DimmingPanelVisibility = Visibility.Collapsed;
                        Ans4BTN.DimmingPanelVisibility = Visibility.Collapsed;
                        TimerBox.Visibility = Visibility.Collapsed;
                    });
                    foreach (KeyValuePair<int, int> pair in answers)
                    {
                        if (pair.Value == gameStateResponse.correctAnswerId)
                        {
                            this.Dispatcher.Invoke(() =>
                            {
                                answerButtons[pair.Key].MainColor = correctAnswerColor;
                                answerButtons[pair.Key].OnChnageColor = correctAnswerColor;
                                answerButtons[pair.Key].SecondaryColor = correctAnswerSecondaryColor;
                            });
                                
                        }
                        else {
                            this.Dispatcher.Invoke(() =>
                            {
                                answerButtons[pair.Key].MainColor = wrongAnswerColor;
                                answerButtons[pair.Key].OnChnageColor = wrongAnswerColor;
                                answerButtons[pair.Key].SecondaryColor = wrongAnswerSecondaryColor; 
                            });
                                
                        }
                    }

                    if (!soundPlayed) // play sound just if it is the first answers response from server
                    {
                        this.Dispatcher.Invoke(() =>
                        {
                            AudioManager.Instance.PauseBackgroundMusic();
                        });
                        // play sound
                        SoundPlayer player;
                        if (answerChosen == gameStateResponse.correctAnswerId)
                        {

                            player = new SoundPlayer(@"..\..\..\Sounds\Totoah.wav");

                        }
                        else
                        {
                            player = new SoundPlayer(@"..\..\..\Sounds\Luzer.wav");
                        }
                        player.Load();
                        player.Play();
                        soundPlayed = true;
                    }

                    answerChosen = -1;
                }

                else if (gameStateResponse.status == (uint)GameState.WaitingForAnswers)
                {
                    this.Dispatcher.Invoke(() =>
                    {
                        AudioManager.Instance.ResumeBackgroundMusic();
                    });
                            
                    soundPlayed = false; // restart the sound
                    if (gameState == GameState.NotStarted || gameState == GameState.WaitingForNewQuestion)
                    {
                        Random rng = new Random();
                        randomQuestions = randomQuestions.OrderBy(_ => rng.Next()).ToList();

                        this.Dispatcher.Invoke(() =>
                        {
                            Ans1BTN.MainColor = "#ff0000";
                            Ans1BTN.OnChnageColor = "#660000";
                            Ans1BTN.SecondaryColor = "#ff4d4d";

                            Ans2BTN.MainColor = "#0066ff";
                            Ans2BTN.OnChnageColor = "#002db3";
                            Ans2BTN.SecondaryColor = "#3385ff";

                            Ans3BTN.MainColor = "#33cc33";
                            Ans3BTN.OnChnageColor = "#2eb82e";
                            Ans3BTN.SecondaryColor = "#5cd65c";

                            Ans4BTN.MainColor = "#cccc00";
                            Ans4BTN.OnChnageColor = "#b3b300";
                            Ans4BTN.SecondaryColor = "#ffff1a";
                        });
                    }

                    // questions in progress, getting question, answers, time
                    // change colors to base colors:
                    this.Dispatcher.Invoke(() =>
                    {
                        TimerBox.Visibility = Visibility.Visible;

                        // updateing questions
                        int questionCounter = 0;
                        foreach (KeyValuePair<int, string> answer in gameStateResponse.answers)
                        {
                            if (answer.Value.Length <= 20)
                            {
                                answerButtons[randomQuestions[questionCounter]].TextSize = 16;
                            }
                            if (answer.Value.Length > 20)
                            {
                                answerButtons[randomQuestions[questionCounter]].TextSize = 14;
                            }
                            if(answer.Value.Length > 25)
                            {
                                answerButtons[randomQuestions[questionCounter]].TextSize = 12;
                            }
                            if (answer.Value.Length > 30)
                            {
                                answerButtons[randomQuestions[questionCounter]].TextSize = 10;
                            }
                            answerButtons[randomQuestions[questionCounter]].Placeholder = answer.Value;

                            answers[randomQuestions[questionCounter]] = answer.Key;

                            questionCounter++;
                        }

                        // update timer
                        int percentage = (int)(((double)gameStateResponse.timeLeft / questionMaxTime) * 100);
                        int remainingPercentage = 100 - percentage;
                        circularProgressBar.UpdateProgress(remainingPercentage);
                        TimerTXT.Text = gameStateResponse.timeLeft.ToString();

                        // update questions according to question length
                        if(gameStateResponse.question.Length <= 56)
                        {
                            TriviaBoxQuestion.Visibility = Visibility.Visible;
                            BigTriviaBoxQuestion.Visibility = Visibility.Collapsed;

                            TriviaBoxQuestion.Placeholder = gameStateResponse.question;
                        }
                        else
                        {
                            BigTriviaBoxQuestion.Visibility = Visibility.Visible;
                            TriviaBoxQuestion.Visibility = Visibility.Collapsed;

                            int splitPosition = 56;
                            string firstPart;
                            string secondPart;

                            int lastSpaceIndex = gameStateResponse.question.LastIndexOf(' ', splitPosition);

                            // if no space is found split at the 56th character, edge case
                            if (lastSpaceIndex == -1)
                            {
                                lastSpaceIndex = splitPosition;
                            }

                            firstPart = gameStateResponse.question.Substring(0, lastSpaceIndex).Trim();
                            secondPart = gameStateResponse.question.Substring(lastSpaceIndex).Trim();

                            BigTriviaBoxQuestion.UpperText = firstPart;
                            BigTriviaBoxQuestion.LowerText = secondPart;
                        }
                    });
                }
                else if (gameStateResponse.status == (uint)GameState.GameEnded)
                {
                    this.Dispatcher.Invoke(() =>
                    {
                        // move to game results and break
                        needRefresh = false;
                        NavigationService.Navigate(new EndGame(gameStateResponse.results, m_communicator));
                        
                    });
                        
                }
                gameState = (GameState)gameStateResponse.status;
                Thread.Sleep(200);
            }
        }
        private void BackBTN_Click(object sender, MouseButtonEventArgs e)
        {
            byte[] response = m_communicator.sendMessage(Serializer.SerializeAllRequests((byte)CodeID.LeaveGame, ""));

            LeaveGameResponse? leaveResponse = Deserializer.DeserializeResponse<LeaveGameResponse>(response);

            if (leaveResponse.status == 1)
            {
                AudioManager.Instance.PlayBackgroundMusic(@"..\..\..\Sounds\MenuMusic.wav");
                needRefresh = false;
                // return to menu
                while (NavigationService.CanGoBack)
                {
                    NavigationService.GoBack();
                }
                NavigationService.GoForward();
                NavigationService.GoForward();
            }
            else
            {
                MessageBox.Show("There's no previous page to navigate back to.");
            }
        }

        private void dimAllPanelsApartOfOne(int buttonNum)
        {
            DimmingPanel.Visibility = Visibility.Visible;
            foreach (KeyValuePair<int, TriviaBox> pair in answerButtons)
            {
                if (pair.Key != buttonNum)
                {
                    pair.Value.DimmingPanelVisibility = Visibility.Visible;
                }
            }
        }

        private void TurnOffButtonHovers()
        {
            this.Dispatcher.Invoke(() =>
            {
                Ans1BTN.OnChnageColor = "#ff0000";
                Ans2BTN.OnChnageColor = "#0066ff";
                Ans3BTN.OnChnageColor = "#33cc33";
                Ans4BTN.OnChnageColor = "#cccc00";
            });
        }

        private void MakeGameStartScreen(List<string> names)
        {
            this.Dispatcher.Invoke(() =>
            {
                MainGameBox.Visibility = Visibility.Collapsed;
                TimerBox.Visibility = Visibility.Collapsed;
                MainMessageBox.Visibility = Visibility.Visible;
                if (names.Count > 0)
                {
                    MainMessageText.Text = "Opponents: " + string.Join(", ", names ?? new string[0].Where(x => x != null));
                }
                else
                {
                    MainMessageText.Text = "no opponents (no friends?)";
                }
            });
        }

        private void Ans1_Click(object sender, RoutedEventArgs e)
        {
            if (answerChosen != -1)
            {
                return;
            }

            if (gameState == GameState.WaitingForAnswers)
            {
                dimAllPanelsApartOfOne(1);
                TurnOffButtonHovers();
                answerChosen = answers[1];
            }
        }

        private void Ans2_Click(object sender, RoutedEventArgs e)
        {
            if (answerChosen != -1)
            {
                return;
            }

            if (gameState == GameState.WaitingForAnswers)
            {
                dimAllPanelsApartOfOne(2);
                TurnOffButtonHovers();
                answerChosen = answers[2];
            }
        }
        private void Ans3_Click(object sender, RoutedEventArgs e)
        {
            if (answerChosen != -1)
            {
                return;
            }

            if (gameState == GameState.WaitingForAnswers)
            {
                dimAllPanelsApartOfOne(3);
                TurnOffButtonHovers();
                answerChosen = answers[3];
            }
        }
        private void Ans4_Click(object sender, RoutedEventArgs e)
        {
            if (answerChosen != -1)
            {
                return;
            }

            if (gameState == GameState.WaitingForAnswers)
            {
                dimAllPanelsApartOfOne(4);
                TurnOffButtonHovers();
                answerChosen = answers[4];
            }
        }
    }
}
