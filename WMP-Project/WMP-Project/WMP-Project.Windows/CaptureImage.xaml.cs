// FILE				: CaptureImage.xaml.cs
// PROJECT			: Windows Programming(PROG2120) – Assignment 7: Windows Surface Programming
// PROGRAMMER(S)    : KiDuck Kim(DUCKIE), Robin Dagenais(ROB)
// FIRST VERSION	: NOV25-2014
// 
//                 					   
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
using Windows.Storage;
using Windows.UI.Xaml.Media.Imaging;
using Windows.Storage.Streams;
using Windows.UI;
using Windows.Media.Capture;

// The Basic Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234237

namespace WMP_Project
{
    /// <summary>
    /// A basic page that provides characteristics common to most applications.
    /// </summary>
    public sealed partial class CaptureImage : Page
    {
        private string capturedFile = null;
        private BitmapImage bitmapImage;
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


        public CaptureImage()
        {
            this.InitializeComponent();
            this.navigationHelper = new NavigationHelper(this);
            this.navigationHelper.LoadState += navigationHelper_LoadState;
            this.navigationHelper.SaveState += navigationHelper_SaveState;
            capturedFile = "";
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
        /// a dictionary state preserved by this page during an earlier session.
        /// The state will be null the first time a page is visited.</param>
        private async void navigationHelper_LoadState(object sender, LoadStateEventArgs e)
        {
            if (e.PageState != null && e.PageState.ContainsKey("capturedFile"))
            {
                capturedFile = e.PageState["capturedFile"].ToString();
                if (!string.IsNullOrEmpty(capturedFile))
                {
                    // Open the file via the token that you stored when adding this file into the MRU list.
                    Windows.Storage.StorageFile file =
                        await Windows.Storage.AccessCache.StorageApplicationPermissions.MostRecentlyUsedList.GetFileAsync(capturedFile);

                    if (file != null)
                    {
                        // Open a stream for the selected file.
                        Windows.Storage.Streams.IRandomAccessStream fileStream =
                            await file.OpenAsync(Windows.Storage.FileAccessMode.Read);

                        // Set the image source to a bitmap.
                        bitmapImage = new BitmapImage();

                        bitmapImage.SetSource(fileStream);
                        displayImage.Source = bitmapImage;

                        // Set the data context for the page.
                        this.DataContext = file;
                    }
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
            if (!string.IsNullOrEmpty(capturedFile))
            {
                e.PageState["capturedFile"] = capturedFile;
            }
        }

        /// <summary>
        /// Process the Capture Image Button.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void btnCapturePhoto_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                NotifyUser("", true);

                // Using Windows.Media.Capture.CameraCaptureUI API to capture a photo
                CameraCaptureUI dialog = new CameraCaptureUI();
                Size aspectRatio = new Size(16, 16);
                dialog.PhotoSettings.CroppedAspectRatio = aspectRatio;

                StorageFile file = await dialog.CaptureFileAsync(CameraCaptureUIMode.Photo);
                if (file != null)
                {
                    bitmapImage = new BitmapImage();
                    using (IRandomAccessStream fileStream = await file.OpenAsync(FileAccessMode.Read))
                    {
                        bitmapImage.SetSource(fileStream);
                    }
                    displayImage.Source = bitmapImage;
                    btnReset.Visibility = Visibility.Visible;

                    // Store the file path 
                    capturedFile = file.Path;
                }
                else
                {
                    NotifyUser("No photo captured.", false);
                }
            }
            catch (Exception ex)
            {
                NotifyUser(ex.Message, true);
            }
        }

        /// <summary>
        /// Process of the Reset Button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnReset_Click(object sender, RoutedEventArgs e)
        {
            btnReset.Visibility = Visibility.Collapsed;
            displayImage.Source = new BitmapImage(new Uri(this.BaseUri, "Assets/placeholder.png"));

            // Clear file path in Application Data
            capturedFile = "";
        }

        /// <summary>
        /// Process of the Use Photo Button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUsePhoto_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(capturedFile))
                return;

            MainPage.SetBgImage(capturedFile);

            MainPage.Current.GoBack();
        }
        

        /// <summary>
        /// Display status / error message.
        /// </summary>
        /// <param name="strMessage"></param>
        /// <param name="bError"></param>
        private void NotifyUser(string strMessage, bool bError)
        {
            if(bError)
            {
                // Use the status message style.
                ELog.Foreground = new SolidColorBrush(Colors.DarkOrange);
            }
            else
            {
                // Use the error message style.
                ELog.Foreground = new SolidColorBrush(Colors.DarkGreen);
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
    }
}
