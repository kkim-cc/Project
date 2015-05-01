﻿// FILE								: LeaderBoardPage.xaml.cs
// PROJECT					: Windows Programming(PROG2120) – Assignment 7: Windows Surface Programming
// PROGRAMMER(S)    : KiDuck Kim(DUCKIE), Robin Dagenais(ROB)
// FIRST VERSION		: NOV25-2014
// DESCRIPTION			: This file has the code for displaying the StoryBoard Page
//                 						   Get information for the game from a file and display.
using WMP_A7.Common;
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

// The Basic Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234237

namespace WMP_A7
{
    /// <summary>
    /// A basic page that provides the characteristics common to most applications.
    /// </summary>
    public sealed partial class LeaderBoardPage : Page
    {
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


        public LeaderBoardPage()
        {
            this.InitializeComponent();
            this.navigationHelper = new NavigationHelper(this);
            this.navigationHelper.LoadState += navigationHelper_LoadState;
            this.navigationHelper.SaveState += navigationHelper_SaveState;

            // load data from files...
            btnLoad.Click += new RoutedEventHandler(btnLoad_Click);

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
        private void navigationHelper_LoadState(object sender, LoadStateEventArgs e)
        {
        }

        /// <summary>
        /// Preserves the state associated with this page in case the application is suspended or the
        /// page is discarded from the navigation cache.  Values must conform to the serialization
        /// requirements of <see cref="SuspensionManager.SessionState"/>.
        /// </summary>
        /// <param name="sender">The source of the event; typically <see cref="NavigationHelper"/></param>
        /// <param name="e">Event data that provides an empty dictionary to be populated with the
        /// serializable state.</param>
        private void navigationHelper_SaveState(object sender, SaveStateEventArgs e)
        {
        }

        private async void btnLoad_Click(object sender, RoutedEventArgs e)
        {
            string fileContent = "";
            fileContent = await MainPage.ReadTextFile();
            if (fileContent == "" && MainPage.fileContent != "")
                fileContent = MainPage.fileContent;

            if (fileContent == "")
                return;

            int ntime;
            string strbuf = "";
            if(fileContent.Length > 0)
            {
                string[] line = fileContent.Split('\n');
                for (int i = 0; i < line.Length; i++)
                {
                    string[] field = line[i].Split('|');
                    if(field.Length > 2)
                    {
                        strbuf = String.Format("GameMode: {0,-10} UserName: {1,-20}", field[0], field[2]);
                        
                        if(Int32.TryParse(field[1], out ntime))
                        {
                            if(ntime < 60)
                                strbuf += String.Format(" GameTime: ({0} seconds)", ntime);
                            else
                                strbuf += String.Format(" GameTime: ({0} mins {1} seconds)", ntime/60, ntime % 60);
                        }

                        lbBoard.Items.Add(strbuf);
                    }
                }
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