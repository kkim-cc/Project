// FILE				: MainPage.xaml.cs
// PROJECT			: Windows Programming(PROG2120) – Project: Puzzle
// PROGRAMMER(S)    : KiDuck Kim(DUCKIE), Robin Dagenais(ROB)
// FIRST VERSION	: December 5, 2014
// DESCRIPTION		: This file is the Main Page program and contains most
//                    of the game's functions.
using WMP_Project.Common;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using System.Threading.Tasks;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI;
using Windows.UI.Xaml.Shapes;
using Windows.UI.Input;

// The Basic Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234237

namespace WMP_Project
{
    /// <summary>
    /// A basic page that provides characteristics common to most applications.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public static MainPage Current;
        public static string filename = "game_score.dat";
        public static string fileContent = "";
        public static string bgfilename="";
        
        private int nGameTime;
        private DispatcherTimer timer;
        private bool bGameIsRun;
        private bool bGameIsEnd;
        private Puzzle puzzle;

        private NavigationHelper navigationHelper;
        private ObservableDictionary defaultViewModel = new ObservableDictionary();


        /// <summary>
        /// This can be changed to a strongly typed view model.
        /// </summary>
        public ObservableDictionary DefaultViewModel
        {
            get { return this.defaultViewModel; }
        }

        /// <summary>
        /// NavigationHelper is used on each page to aid in navigation and 
        /// process lifetime management
        /// </summary>
        public NavigationHelper NavigationHelper
        {
            get { return this.navigationHelper; }
        }

        /// <summary>
        /// Initialize the game components
        /// </summary>
        public MainPage()
        {
            this.InitializeComponent();
            this.navigationHelper = new NavigationHelper(this);
            this.navigationHelper.LoadState += navigationHelper_LoadState;
            this.navigationHelper.SaveState += navigationHelper_SaveState;

            Current = this;
            bGameIsRun = false;
            bGameIsEnd = false;
            nGameTime = 0;

            // Timer...
            timer = new DispatcherTimer();
            timer.Tick += timer_Tick;
            timer.Interval = TimeSpan.FromSeconds(1);

            puzzle = new Puzzle();
        }

        /// <summary>
        /// Get Current Game's Elapsed Time
        /// </summary>
        /// <returns></returns>
        public int GetElapsedTime()
        {
            return nGameTime;
        }

        public void DisplayTime()
        {
            if (nGameTime >= 60)
                GTime.Text = String.Format("Elapsed Time: {0} mins {1} seconds.", nGameTime / 60, nGameTime % 60);
            else
                GTime.Text = String.Format("Elapsed Time: {0} seconds.", nGameTime);
        }

        /// <summary>
        /// Handle the Timer Function
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void timer_Tick(object sender, object e)
        {
            if (bGameIsRun == false)
                return;

            nGameTime++;

            DisplayTime();
        }
        /// <summary>
        /// Populates the page with content passed during navigation. Any saved state is also
        /// provided when recreating a page from a prior session.
        /// </summary>
        /// <param name="sender">
        /// The source of the event; typically <see cref="NavigationHelper"/>
        /// </param>
        /// <param name="e">Event data that provides both the navigation parameter passed to
        /// <see cref="Frame.Navigate(Type, Object)"/> when this page was initially requested and
        ///  a dictionary state preserved by this page during an earlier session.
        ///  The state will be null the first time a page is visited.</param>
        private void navigationHelper_LoadState(object sender, LoadStateEventArgs e)
        {
            if (e.PageState != null && e.PageState.ContainsKey("bGameIsRun"))
            {
                bGameIsRun = true;
                bGameIsEnd = false;

                if (e.PageState.ContainsKey("nGameTime"))
                {
                    string strbuf = e.PageState["StartGame"].ToString();
                    Int32.TryParse(strbuf, out nGameTime);

                    DisplayTime();
                }

                if (e.PageState.ContainsKey("Puzzle"))
                {
                    puzzle = new Puzzle(e.PageState["Puzzle"].ToString());
                    DisplayPuzzle();
                    timer.Start();
                }
            }
        }

        /// <summary>
        /// Preserves state associated with this page in case the application is suspended or the
        /// page is discarded from the navigation cache.  Values must conform to the serialization
        /// requirements of <see cref="SuspensionManager.SessionState"/>.
        /// </summary>
        /// <param name="sender">The source of the event; typically <see cref="NavigationHelper"/></param>
        /// <param name="e">Event data that provides an empty dictionary to be populated with
        /// serializable state.</param>
        private void navigationHelper_SaveState(object sender, SaveStateEventArgs e)
        {
            if(bGameIsRun && !bGameIsEnd)
            {
                e.PageState["bGameIsRun"] = "1";
                e.PageState["nGameTime"] = nGameTime.ToString();
                e.PageState["Puzzle"] = puzzle.Stringify();
            }
        }

        private void DisplayPuzzle()
        {
            List<Rectangle> rect = puzzle.GetList();

            if (rectangleItems.Items.Count > 0)
                rectangleItems.Items.Clear();
            // add the pieces of puzzle & assign the process of handler 
            foreach (Rectangle rc in rect)
            {
                rectangleItems.Items.Add(rc);
            }
        }

        #region NavigationHelper registration

        /// The methods provided in this section are simply used to allow
        /// NavigationHelper to respond to the page's navigation methods.
        /// 
        /// Page specific logic should be placed in event handlers for the  
        /// <see cref="GridCS.Common.NavigationHelper.LoadState"/>
        /// and <see cref="GridCS.Common.NavigationHelper.SaveState"/>.
        /// The navigation parameter is available in the LoadState method 
        /// in addition to page state preserved during an earlier session.
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            navigationHelper.OnNavigatedTo(e);
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            navigationHelper.OnNavigatedFrom(e);
        }

        #endregion

        private void btnLoadImage_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(LoadImage));
        }

        private void btnCaptureImage_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(CaptureImage));
        }

        private void StopGame()
        {
            if (!bGameIsRun)
                return;
            timer.Stop();
        }

        private void ClearGame()
        {
            if (bGameIsRun)
                StopGame();

            puzzle.Clear();
            rectangleItems.Items.Clear();
            nGameTime = 0;
        }

        private async void InitializeGame()
        {
            // initialize game
            await puzzle.InitializeGame(bgfilename);
            DisplayPuzzle();
        }

        private void btnStartGame_Click(object sender, RoutedEventArgs e)
        {
            if (bGameIsRun)
                return;

            if(string.IsNullOrEmpty(bgfilename))
            {
                NotifyUser("Please click Load/Capture Image first.", NotifyType.ErrorMessage);
                return;
            }

            if (rectangleItems.Items.Count == 0 || bGameIsEnd)
            {
                InitializeGame();
            }
            // start game timer.
            timer.Start();
            nGameTime = 0;
            bGameIsRun = true;
            bGameIsEnd = false;
        }

        private void btnPauseGame_Click(object sender, RoutedEventArgs e)
        {
            StopGame();
        }

        private void btnClearGame_Click(object sender, RoutedEventArgs e)
        {
            ClearGame();
        }

        private async void btnGameScore_Click(object sender, RoutedEventArgs e)
        {
            StopGame();

            if (fileContent == "")
            {
                await ReadTextFile();
            }

            this.Frame.Navigate(typeof(LeaderBoardPage));
        }


        public static void SetBgImage(string filename)
        {
            bgfilename = filename;
        }

        /// <summary>
        /// Read information from a saved file.
        /// </summary>
        /// <returns></returns>
        public static async Task<string> ReadTextFile()
        {
            string contents = "";

            StorageFolder localFolder = ApplicationData.Current.LocalFolder;

            try
            {
                StorageFile textFile = await localFolder.GetFileAsync(filename);
                using (IRandomAccessStream textStream = await textFile.OpenReadAsync())
                {
                    using (DataReader textReader = new DataReader(textStream))
                    {
                        uint textLength = (uint)textStream.Size;
                        await textReader.LoadAsync(textLength);
                        contents = textReader.ReadString(textLength);
                        if (fileContent == "")
                            fileContent = contents;
                    }
                }
            }
            catch (Exception e)
            {
                MainPage.Current.NotifyUser(e.ToString(), NotifyType.ErrorMessage);
                return contents;
            }
            return contents;
        }


        /// <summary>
        /// Save information to file
        /// </summary>
        /// <param name="contents"></param>
        /// <returns></returns>
        public static async Task WriteTextFile(string contents)
        {
            StorageFolder localFolder = ApplicationData.Current.LocalFolder;
            fileContent += contents;
            try
            {
                StorageFile textFile = await localFolder.CreateFileAsync(filename, CreationCollisionOption.ReplaceExisting);

                using (IRandomAccessStream textStream = await textFile.OpenAsync(FileAccessMode.ReadWrite))
                {
                    using (DataWriter textWriter = new DataWriter(textStream))
                    {
                        textWriter.WriteString(fileContent);
                        await textWriter.StoreAsync();
                    }
                }
            }
            catch (Exception e)
            {
                // error message!!!
                MainPage.Current.NotifyUser(e.ToString(), NotifyType.ErrorMessage);
            }
        }

        public void GoBack()
        {
            Frame frame = Window.Current.Content as Frame;
            if (frame == null)
            {
                return;
            }

            if (frame.CanGoBack)
            {
                frame.GoBack();
                //e.Handled = true;
            }
        }

        public enum NotifyType
        {
            StatusMessage,
            ErrorMessage
        };

        /// <summary>
        /// Display Message to User.
        /// </summary>
        /// <param name="strMessage"></param>
        /// <param name="type"></param>
        public void NotifyUser(string strMessage, NotifyType type)
        {
            switch (type)
            {
                // Use the status message style.
                case NotifyType.StatusMessage:
                    ELog.Foreground = new SolidColorBrush(Colors.Green);

                    break;
                // Use the error message style.
                case NotifyType.ErrorMessage:
                    ELog.Foreground = new SolidColorBrush(Colors.Orange);
                    //ELog.Style = Resources["ErrorStyle"] as Style;
                    break;
            }
            ELog.Text = strMessage;

            // Collapse the StatusBlock if it has no text to conserve real estate.
            if (ELog.Text != String.Empty)
            {
                ELog.Visibility = Windows.UI.Xaml.Visibility.Visible;
            }
            else
            {
                ELog.Visibility = Windows.UI.Xaml.Visibility.Collapsed;
            }
        }
        private void Canvas_Tapped(object sender, TappedRoutedEventArgs e)
        {
            if (!bGameIsRun)
                return;

            UIElement element = (UIElement)sender;

            IEnumerable<UIElement> hits = VisualTreeHelper.FindElementsInHostCoordinates(e.GetPosition(this), element);
            foreach (Rectangle em in hits)
            { 
                if(em != null)
                {
                    int i = rectangleItems.Items.IndexOf(em);
                    if(i != -1)
                    {
                        //
                        string msg;
                        var pos = e.GetPosition(this);
                        msg = string.Format("Tabbed: point({0}, {1}) exist in the list {2}",
                            (int)pos.X, (int)pos.Y, i);
                        NotifyUser(msg, NotifyType.StatusMessage);
                        TabProcess(i);
                        return;
                    }
                }
            }
        }
		
		
		/// <summary>
        /// Provides feedback to the user and calls the process to move image segments
        /// </summary>
        public void MoveProcess(int nId, int ndirection)
        {
            if (!bGameIsRun)
                return;

            puzzle.MoveProcess(nId, ndirection);
            NotifyUser("Move: " + nId.ToString() + "/" + ndirection.ToString(), NotifyType.StatusMessage);
        }

		
		/// <summary>
        /// Handles the process of moving the image segments
        /// </summary>
        public void TabProcess(int nId)
        {
            if (!bGameIsRun)
                return;

            int next = 0;
            int move = puzzle.TabProcess(nId);
            if(move == Puzzle.moveDown)
            {
                // delete two and add.
                next = nId + 1;
                rectangleItems.Items.RemoveAt(next);
                rectangleItems.Items.RemoveAt(nId);
                if(nId >= rectangleItems.Items.Count)
                {
                    rectangleItems.Items.Add(puzzle.GetRect(nId));
                }
                else
                {
                    rectangleItems.Items.Insert(nId, puzzle.GetRect(nId));
                }
                if (next >= rectangleItems.Items.Count)
                {
                    rectangleItems.Items.Add(puzzle.GetRect(next));
                }
                else
                {
                    rectangleItems.Items.Insert(next, puzzle.GetRect(next));
                }
            }
            else if (move == Puzzle.moveUp)
            {
                next = nId - 1;
                rectangleItems.Items.RemoveAt(nId);
                rectangleItems.Items.RemoveAt(next);
                if (nId - 1 >= rectangleItems.Items.Count)
                {
                    rectangleItems.Items.Add(puzzle.GetRect(next));
                }
                else
                {
                    rectangleItems.Items.Insert(next, puzzle.GetRect(next));
                }
                if (nId >= rectangleItems.Items.Count)
                {
                    rectangleItems.Items.Add(puzzle.GetRect(nId));
                }
                else
                {
                    rectangleItems.Items.Insert(nId, puzzle.GetRect(nId));
                }
            }
            else if (move == Puzzle.moveLeft)
            {
                next = nId - 4;
                rectangleItems.Items.RemoveAt(nId);
                rectangleItems.Items.RemoveAt(next);
                if (nId - 4 >= rectangleItems.Items.Count)
                {
                    rectangleItems.Items.Add(puzzle.GetRect(nId - 4));
                }
                else
                {
                    rectangleItems.Items.Insert(next, puzzle.GetRect(next));
                }
                if (nId >= rectangleItems.Items.Count)
                {
                    rectangleItems.Items.Add(puzzle.GetRect(nId));
                }
                else
                {
                    rectangleItems.Items.Insert(nId, puzzle.GetRect(nId));
                }
            }
            else if(move == Puzzle.moveRight)
            {
                next = nId + 4;
                rectangleItems.Items.RemoveAt(next);
                rectangleItems.Items.RemoveAt(nId);
                if (nId >= rectangleItems.Items.Count)
                {
                    rectangleItems.Items.Add(puzzle.GetRect(nId));
                }
                else
                {
                    rectangleItems.Items.Insert(nId, puzzle.GetRect(nId));
                }
                if (nId + 4 >= rectangleItems.Items.Count)
                {
                    rectangleItems.Items.Add(puzzle.GetRect(next));
                }
                else
                {
                    rectangleItems.Items.Insert(next, puzzle.GetRect(next));
                }

            }
            if (move > 0)
            {
                if(puzzle.IsGameEnd())
                {
                    StopGame();
                    bGameIsEnd = true;
                    this.Frame.Navigate(typeof(Success));
                }
            }
        }
    }
}
