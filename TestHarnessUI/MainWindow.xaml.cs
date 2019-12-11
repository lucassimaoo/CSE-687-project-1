/*
Name: Derrick Ward
Class/Section: CSE 687 Object Oriented Design
Project 4
Description: This file acts as the controller between the UI and the Services
Date: 12/9/2019
*/

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using TestHarnessUI.Models;
using TestHarnessUI.Services;

namespace TestHarnessUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            this.Closing += ApplicationClossingEvent;
            DataContext = this;

            this.LogSystemMessage("System Online");

            // Setup Message Service
            this._MessageService.MessageReceived += MessageService_MessageReceived;
            this.LogSystemMessage("Listening for Messages.");
        }

        /// <summary>
        /// Event Handler for when the application is shutting down
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ApplicationClossingEvent(object sender, System.ComponentModel.CancelEventArgs e)
        {
            this._ApplicationClosing = true;

            if (this._MessageService != null)
            {
                this._MessageService.Dispose();
            }
        }

        /// <summary>
        /// Contains collection of incoming messages that have been received
        /// </summary>
        public ObservableCollection<string> IncomingMessages { get; private set; } = new ObservableCollection<string>();

        /// <summary>
        /// Contains collection of application wide messages (debug, incoming messages, etc)
        /// </summary>
        public ObservableCollection<string> ApplicationMessages { get; private set; } = new ObservableCollection<string>();

        /// <summary>
        /// Holds communication settings for sending and receiving messages
        /// </summary>
        public Settings Settings { get; set; } = new Settings();

        /// <summary>
        /// Message Service to communicate with Test Harness
        /// </summary>
        private IMessageService _MessageService = new TestHarnessMessageService();

        private ISettingsValidationService _SettingsValidationService = new SettingsValidationService();

        /// <summary>
        /// Indicates if the application is closing
        /// </summary>
        private Boolean _ApplicationClosing = false;

        /// <summary>
        /// Event Handler for when a incoming messaged has been recieved
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MessageService_MessageReceived(object sender, string e)
        {
            if (!this._ApplicationClosing)
            {
                // Jump onto Main UI Thread
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    this.IncomingMessages.Add(e);
                }));
            }
        }

        /// <summary>
        /// Event Handler for when the settings are saved
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SaveSettings_Clicked(object sender, RoutedEventArgs e)
        {
            try
            {
                SettingsValidationModel settingsValidationModel = this._SettingsValidationService.GetSettingsValidationModel(this.Settings);

                if (!settingsValidationModel.IsValid())
                {
                    //TODO: Show Settings Validation Messages
                    return;
                }

                this._MessageService.StartListeningForMessages(this.Settings);        
            }
            catch (Exception exception)
            {
                this.LogSystemMessage($"Error Occured:\n-----{exception}\n-----");
            }

            this.LogSystemMessage("Settings Modified and Saved.");
        }

        /// <summary>
        /// Event Handler for when Run Tests is called
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RunTests_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                SettingsValidationModel settingsValidationModel = this._SettingsValidationService.GetSettingsValidationModel(this.Settings);

                if (!settingsValidationModel.IsValid())
                {
                    //TODO: Show Settings Validation Messages
                    return;
                }

                if (!File.Exists(this.TextBox_XmlFilePath.Text))
                {
                    //TODO: Show Error Message Stating file does not exist
                    return;
                }

                //TODO: Validate Xml File First

                // Read in all XML Content
                string xmlContent = File.ReadAllText(this.TextBox_XmlFilePath.Text);

                this._MessageService.SendMessage(this.Settings, xmlContent);
            }
            catch (Exception exception)
            {
                this.LogSystemMessage($"Error Occured:\n-----{exception}\n-----");
            }

            this.LogSystemMessage($"Test Sent:\t{this.TextBox_XmlFilePath.Text}");
        }

        //TODO: Add Event Handler for when Browse Files is clicked

        /// <summary>
        /// Logs System Messages
        /// </summary>
        /// <param name="mesage"></param>
        private void LogSystemMessage(string message)
        {
            this.ApplicationMessages.Add($"{DateTime.Now.ToString()}:\t{message}");
        }
    }
}
