/*
Name: Derrick Ward
Class/Section: CSE 687 Object Oriented Design
Project 4
Description: This is the model for a group of test dlls we want the test server to run
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
    /// Class to hold all the test dll paths for a single test group
    /// </summary>
    public class TestGroup
    {
        public IEnumerable<string> FilePaths { get; set; }
    }
}
