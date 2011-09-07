namespace SPULS
{
    using System;
    using System.Reflection;
    using System.Security.Permissions;
    using System.IO;

    /// <summary>
    /// Implementation for the Interface
    /// </summary>
    public class SPUlsInfo : IUlsInfo
    {
        #region IUlsInfo Members

        /// <summary>
        /// Gets the log location.
        /// </summary>
        /// <returns>Log location</returns>
        [ReflectionPermission(SecurityAction.Assert)]
        public string GetLogLocation()
        {
            Assembly spAssembly;
            try
            {
                try
                {
                    spAssembly = Assembly.Load(Constants.AssemblyName);
                }
                catch (FileNotFoundException ex)
                {
                    return "-1";
                }
                Type diagService = spAssembly.GetType(Constants.DiagClassName, true);
                PropertyInfo propInfo = diagService.GetProperty("Local", BindingFlags.Static | BindingFlags.Public);
               
                object spDiagServiceObject = propInfo.GetValue(Activator.CreateInstance(diagService), null);
                propInfo = diagService.GetProperty("LogLocation");
                object logLocation = propInfo.GetValue(spDiagServiceObject, null);
                return logLocation.ToString();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return "-1";
            }
        }

        #endregion
    }
}
