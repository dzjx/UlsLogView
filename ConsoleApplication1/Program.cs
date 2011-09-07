using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConsoleApplication1
{
    using System.Reflection;
    using SPULS;

    class Program
    {
        static void Main(string[] args)
        {
          //  AssemblyName spAssemblyName = AssemblyName.GetAssemblyName("Microsoft.SharePoint.dll");
//SPULS.SPUlsInfo spUlsInfo=new SPUlsInfo();
  //          string s=spUlsInfo.GetLogLocation();


            Assembly spAssembly = typeof (Microsoft.SharePoint.Administration.SPDiagnosticsService).Assembly;
            Type diagService = spAssembly.GetType(Constants.DiagClassName, true);
            PropertyInfo propInfo = diagService.GetProperty("Local", BindingFlags.Static | BindingFlags.Public);

            object spDiagServiceObject = propInfo.GetValue(Activator.CreateInstance(diagService), null);
            propInfo = diagService.GetProperty("LogLocation");
            object logLocation = propInfo.GetValue(spDiagServiceObject, null);
         //   return logLocation.ToString();
            

            string s= Microsoft.SharePoint.Administration.SPDiagnosticsService.Local.LogLocation;
        }

    }
}
