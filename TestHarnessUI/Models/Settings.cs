/*
Name: Derrick Ward
Class/Section: CSE 687 Object Oriented Design
Project 4
Description: This is the model that will hold network communication info/settings
Date: 12/9/2019
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHarnessUI.Models
{
    /// <summary>
    /// Class to hold communication settings for sending and receiving messages
    /// </summary>
    public class Settings
    {
        public string TestServerHost { get; set; }
        public string TestServerPort { get; set; }
        public string MessageListeningHost { get; set; }
        public string MessageListeningPort { get; set; }
    }
}
