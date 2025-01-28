using System;
using System.Windows.Media;

namespace Gui.Sounds
{
    public class AudioManager
    {
        private MediaPlayer _mediaPlayer;
        private static AudioManager _instance;

        private AudioManager()
        {
            _mediaPlayer = new MediaPlayer();
            _mediaPlayer.MediaEnded += MediaPlayer_MediaEnded;
        }

        public static AudioManager Instance
        {
            get
            {
                if (_instance == null)
                {
                    _instance = new AudioManager();
                }
                return _instance;
            }
        }

        private void MediaPlayer_MediaEnded(object sender, EventArgs e)
        {
            // loop the music
            //_mediaPlayer.Position = TimeSpan.Zero;
            //_mediaPlayer.Play();
        }

        public void PlayBackgroundMusic(string filePath)
        {
            // Clean old
            //_mediaPlayer.Close();

            //_mediaPlayer.Open(new Uri(filePath, UriKind.Relative));
            //_mediaPlayer.Play();
        }

        public void StopBackgroundMusic()
        {
            //_mediaPlayer.Stop();
        }

        public void PauseBackgroundMusic()
        {
            //_mediaPlayer.Pause();
        }

        public void ResumeBackgroundMusic()
        {
            //_mediaPlayer.Play();
        }

        public void SetVolume(double volume)
        {
            //_mediaPlayer.Volume = volume; // volume range: 0.0 to 1.0
        }

        public void Dispose()
        {
            _mediaPlayer.Close();
        }
    }
}
