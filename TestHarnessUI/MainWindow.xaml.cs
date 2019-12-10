using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
            DataContext = this;

            this.LogSystemMessage("System Online");

            // Setup Message Service
            this.messageService.MessageReceived += MessageService_MessageReceived;
            this.LogSystemMessage("Listening for Messages.");
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
        IMessageService messageService = new TestHarnessMessageService();

        /// <summary>
        /// Event Handler for when a incoming messaged has been recieved
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MessageService_MessageReceived(object sender, string e)
        {
            this.IncomingMessages.Add(e);
        }

        /// <summary>
        /// Event Handler for when the settings are saved
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SaveSetting_Clicked(object sender, RoutedEventArgs e)
        {
            //TODO: Validate Settings using settings validation service first

            this.messageService.StartListeningForMessages(this.Settings);

            this.LogSystemMessage("Settings Modified and Saved.");
        }

        //TODO: Add Event Handler for when Run Tests is clicked

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
