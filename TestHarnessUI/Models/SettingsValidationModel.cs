using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestHarnessUI.Models
{
    public class SettingsValidationModel
    {
        public ValidationInfo TestServerHost = new ValidationInfo();
        public ValidationInfo TestServerPort = new ValidationInfo();
        public ValidationInfo MessageListeningHost = new ValidationInfo();
        public ValidationInfo MessageListeningPort = new ValidationInfo();

        /// <summary>
        /// Indicates if Settings Validation Model is valid
        /// </summary>
        /// <returns></returns>
        public Boolean IsValid()
        {
            return this.TestServerHost.IsValid
                   && this.TestServerPort.IsValid
                   && this.MessageListeningHost.IsValid
                   && this.MessageListeningPort.IsValid;
        }

        /// <summary>
        /// Gets a list of all validation messages
        /// </summary>
        /// <returns></returns>
        public List<string> GetValidationMessages()
        {
            return this.TestServerHost.ValidationMessages.Concat(this.TestServerPort.ValidationMessages)
                                                         .Concat(this.MessageListeningHost.ValidationMessages)
                                                         .Concat(this.MessageListeningPort.ValidationMessages).ToList();
        }
    }
}
