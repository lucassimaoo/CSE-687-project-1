using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHarnessUI.Models
{
    /// <summary>
    /// Class to hold all the test dll paths for a single test group
    /// </summary>
    public class TestGroup
    {
        public IEnumerable<string> FilePaths { get; set; }
    }
}
