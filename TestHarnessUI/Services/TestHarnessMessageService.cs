using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TestHarnessUI.Models;

namespace TestHarnessUI.Services
{
    public class TestHarnessMessageService : IMessageService
    {
        public event EventHandler<string> MessageReceived;

        /// <summary>
        /// Sends Message to Test Harness Server
        /// </summary>
        /// <param name="settings"></param>
        /// <param name="testGroup"></param>
        public void SendMessage(Settings settings, TestGroup testGroup)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Listens for Incoming Messages and Raises Message Received Events
        /// </summary>
        /// <param name="settings"></param>
        public void StartListeningForMessages(Settings settings)
        {
            if (MessageReceived != null)
            {
                string message = $"{DateTime.Now.ToString()}:\tTest Message";
                MessageReceived(this, message);
            }
        }
    }
}
