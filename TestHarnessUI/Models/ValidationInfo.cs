using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHarnessUI.Models
{
    public class ValidationInfo
    {
        public Boolean IsValid {get; set;} = true;

        public List<string> ValidationMessages { get; set; } = new List<string>();
    }
}
