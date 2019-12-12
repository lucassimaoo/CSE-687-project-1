using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TestHarnessUI.Models;

namespace TestHarnessUI.Services
{
    public class SettingsValidationService : ISettingsValidationService
    {
        public SettingsValidationModel GetSettingsValidationModel(Settings settings)
        {
            // Create a deep copy of settings model, so state is not changed during validation
            var settingsDeepCopy = JsonConvert.DeserializeObject<Settings>(JsonConvert.SerializeObject(settings));

            var settingsValidationModel = new SettingsValidationModel();

            settingsValidationModel.TestServerHost = this.GetTestServerHostValidationInfo(settingsDeepCopy);
            settingsValidationModel.TestServerPort = this.GetTestServerPortValidationInfo(settingsDeepCopy);
            settingsValidationModel.MessageListeningHost = this.GetMessageListeningHostValidationInfo(settingsDeepCopy);
            settingsValidationModel.MessageListeningPort = this.GetMessageListeningPortValidationInfo(settingsDeepCopy);

            return settingsValidationModel;
        }

        /// <summary>
        /// Get Validation Info for Test Server Host
        /// </summary>
        /// <param name="settings"></param>
        /// <returns></returns>
        private ValidationInfo GetTestServerHostValidationInfo(Settings settings)
        {
            var validationInfo = new ValidationInfo();

            if (string.IsNullOrEmpty(settings.TestServerHost))
            {
                validationInfo.IsValid &= false;
                validationInfo.ValidationMessages.Add("Test Server Host Setting Can't be NULL or Empty!");
            }

            return validationInfo;
        }

        /// <summary>
        /// Get Validation Info for Test Server Port
        /// </summary>
        /// <param name="settings"></param>
        /// <returns></returns>
        private ValidationInfo GetTestServerPortValidationInfo(Settings settings)
        {
            var validationInfo = new ValidationInfo();

            int port;

            Boolean isANumber = int.TryParse(settings.TestServerPort, out port);

            if (string.IsNullOrEmpty(settings.TestServerPort))
            {
                validationInfo.IsValid &= false;
                validationInfo.ValidationMessages.Add("Test Server Port Setting Can't Be NULL Or Empty!");
            }

            if (!isANumber)
            {
                validationInfo.IsValid &= false;
                validationInfo.ValidationMessages.Add("Test Server Port Setting Must Be An Integer!");
            }

            return validationInfo;
        }

        /// <summary>
        /// Get Validation Info for Message Listening Host
        /// </summary>
        /// <param name="settings"></param>
        /// <returns></returns>
        private ValidationInfo GetMessageListeningHostValidationInfo(Settings settings)
        {
            var validationInfo = new ValidationInfo();

            if (string.IsNullOrEmpty(settings.MessageListeningHost))
            {
                validationInfo.IsValid &= false;
                validationInfo.ValidationMessages.Add("Message Listening Host Setting Can't be NULL or Empty!");
            }

            return validationInfo;
        }

        /// <summary>
        /// Get Validation Info for Message Listening Port
        /// </summary>
        /// <param name="settings"></param>
        /// <returns></returns>
        private ValidationInfo GetMessageListeningPortValidationInfo(Settings settings)
        {
            var validationInfo = new ValidationInfo();

            int port;

            Boolean isANumber = int.TryParse(settings.MessageListeningPort, out port);

            if (string.IsNullOrEmpty(settings.MessageListeningPort))
            {
                validationInfo.IsValid &= false;
                validationInfo.ValidationMessages.Add("Message Listening Port Setting Can't Be NULL Or Empty!");
            }

            if (!isANumber)
            {
                validationInfo.IsValid &= false;
                validationInfo.ValidationMessages.Add("Message Listening Port Setting Must Be An Integer!");
            }

            return validationInfo;
        }
    }
}
