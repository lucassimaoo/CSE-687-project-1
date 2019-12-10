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
