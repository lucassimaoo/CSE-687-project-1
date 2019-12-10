using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TestHarnessUI.Models;

namespace TestHarnessUI.Services
{
    public interface IMessageService
    {
        /// <summary>
        /// Messaged Recieved Event Handler
        /// </summary>
        event EventHandler<string> MessageReceived;

        /// <summary>
        /// Send Message to Remote Test Server
        /// </summary>
        /// <param name="settings"></param>
        /// <param name="testGroup"></param>
        void SendMessage(Settings settings, TestGroup testGroup);

        /// <summary>
        /// Starts Listening for Incoming Messages and Raises Message Received Event
        /// </summary>
        /// <param name="settings"></param>
        void StartListeningForMessages(Settings settings);
    }
}
