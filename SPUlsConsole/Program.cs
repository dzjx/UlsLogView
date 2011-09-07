namespace SPUlsConsole
{
    using System;
    using System.Reflection;

    /// <summary>
    /// Main class program
    /// </summary>
    class Program
    {
        /// <summary>
        /// Mains the specified args.
        /// </summary>
        /// <param name="args">The args.</param>
        static void Main(string[] args)
        {
            try
            {
                Assembly spAssembly = Assembly.Load(Constants.AssemblyName);
                Type diagService = spAssembly.GetType(Constants.DiagClassName, true);
                PropertyInfo propInfo = diagService.GetProperty("Local", BindingFlags.Static | BindingFlags.Public);

                object spDiagServiceObject = propInfo.GetValue(Activator.CreateInstance(diagService), null);
                propInfo = diagService.GetProperty("LogLocation");
                object logLocation = propInfo.GetValue(spDiagServiceObject, null);
                Console.WriteLine(logLocation.ToString());
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
    }
}
