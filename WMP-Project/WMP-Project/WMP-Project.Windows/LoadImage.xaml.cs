﻿// FILE				: LoadImage.xaml.cs
// PROJECT			: Windows Programming(PROG2120) – Assignment 7: Windows Surface Programming
// PROGRAMMER(S)    : KiDuck Kim(DUCKIE), Robin Dagenais(ROB)
// FIRST VERSION	: NOV25-2014
// DESCRIPTION		: Retrieves a bitmap from the selected source.
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
using Windows.UI.Xaml.Media.Imaging;

// The Basic Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234237

namespace WMP_Project
{
    /// <summary>
    /// A basic page that provides characteristics common to most applications.
    /// </summary>
    public sealed partial class LoadImage : Page
    {
        private string saveFile = null;
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
        /// process lifetime management.
        /// </summary>
        public NavigationHelper NavigationHelper
        {
            get { return this.navigationHelper; }
        }


        public LoadImage()
        {
            this.InitializeComponent();
            this.navigationHelper = new NavigationHelper(this);
            this.navigationHelper.LoadState += navigationHelper_LoadState;
            this.navigationHelper.SaveState += navigationHelper_SaveState;
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
            if (e.PageState != null && e.PageState.ContainsKey("saveFile"))
            {
                object value = null;
                if (e.PageState.TryGetValue("saveFile", out value))
                {
                    if (value != null)
                    {
                        saveFile = value.ToString();

                        // Open the file via the token that you stored when adding this file into the MRU list.
                        Windows.Storage.StorageFile file =
                            await Windows.Storage.AccessCache.StorageApplicationPermissions.MostRecentlyUsedList.GetFileAsync(saveFile);

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
            if (!string.IsNullOrEmpty(saveFile))
            {
                e.PageState["saveFile"] = saveFile;
            }
        }

        /// <summary>
        /// Change the Window Layout
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void PhotoPage_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (e.NewSize.Height / e.NewSize.Width >= 1)
            {
                VisualStateManager.GoToState(this, "Portrait", true);
            }
            else
            {
                VisualStateManager.GoToState(this, "DefaultLayout", true);
            }
        }

        /// <summary>
        /// Process The Load Image from files.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void btnGetPhoto_Click(object sender, RoutedEventArgs e)
        {
            Windows.Storage.Pickers.FileOpenPicker openPicker = new Windows.Storage.Pickers.FileOpenPicker();
            openPicker.SuggestedStartLocation = Windows.Storage.Pickers.PickerLocationId.PicturesLibrary;
            openPicker.ViewMode = Windows.Storage.Pickers.PickerViewMode.Thumbnail;

            // Filter to include a sample subset of file types.
            openPicker.FileTypeFilter.Clear();
            openPicker.FileTypeFilter.Add(".bmp");
            openPicker.FileTypeFilter.Add(".png");
            openPicker.FileTypeFilter.Add(".jpeg");
            openPicker.FileTypeFilter.Add(".jpg");

            // Open the file picker.
            Windows.Storage.StorageFile file = await openPicker.PickSingleFileAsync();

            // file is null if user cancels the file picker.
            if (file != null)
            {
                // Open a stream for the selected file.
                Windows.Storage.Streams.IRandomAccessStream fileStream =
                    await file.OpenAsync(Windows.Storage.FileAccessMode.Read);

                // Set the image source to the selected bitmap.
                bitmapImage = new BitmapImage();

                bitmapImage.SetSource(fileStream);
                displayImage.Source = bitmapImage;
                this.DataContext = file;

                saveFile = file.Path;
                // Add picked file to MostRecentlyUsedList.
                Windows.Storage.AccessCache.StorageApplicationPermissions.MostRecentlyUsedList.Add(file);
            }
        }

        private void btnUsePhoto_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(saveFile))
                return;

            MainPage.SetBgImage(saveFile);

            MainPage.Current.GoBack();
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
