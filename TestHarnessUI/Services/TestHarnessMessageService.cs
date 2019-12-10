using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TestHarnessUI.Models;
/*
Name: Derrick Ward
Class/Section: CSE 687 Object Oriented Design
Project 4
Description: This is a concrete implementation of the Message Service Interface, meant for talking to a Test Harness Server
Date: 12/9/2019
*/

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
