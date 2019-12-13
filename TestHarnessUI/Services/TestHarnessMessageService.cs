using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
        private Task _FetchMessageTask;
        private CancellationTokenSource _FetchMessageTaskCancellationToken;

        public event EventHandler<string> MessageReceived;

        /// <summary>
        /// Sends Message to Test Harness Server
        /// </summary>
        /// <param name="settings"></param>
        /// <param name="testGroup"></param>
        public void SendMessage(Settings settings, string message)
        {
            //TODO: Call Send Message in CLI Wrapper
        }

        /// <summary>
        /// Listens for Incoming Messages and Raises Message Received Events
        /// </summary>
        /// <param name="settings"></param>
        public void StartListeningForMessages(Settings settings)
        {
            this.StopListeningForMessagesAsync();

            // Create new Cancellation Token
            this._FetchMessageTaskCancellationToken = new CancellationTokenSource();

            // Start new Task to keep fetching new incoming messages
            this._FetchMessageTask = Task.Run(() => { 
                while (true)
                {
                    // If there are subscribers to the MessageReceived Event
                    if (MessageReceived != null)
                    {
                        // Get Incoming Message
                        string message = $"{DateTime.Now.ToString()}:\tTest Message";

                        // Raise Event and pass message
                        MessageReceived(this, message);
                    }

                    // Sleep thread so other threads (Main UI Thread) can wait up
                    Thread.Sleep(500);
                }
            }, this._FetchMessageTaskCancellationToken.Token);
        }

        /// <summary>
        /// Stops Listening for Incoming Messages
        /// </summary>
        public void StopListeningForMessagesAsync()
        {
            // Stop current task fetching new incoming messages
            if (this._FetchMessageTask != null)
            {
                this._FetchMessageTaskCancellationToken.Cancel();
                this._FetchMessageTaskCancellationToken.Dispose();                
            }
        }

        /// <summary>
        /// Runs Dispose Operations
        /// </summary>
        public void Dispose()
        {
            this.StopListeningForMessagesAsync();
        }
    }
}
