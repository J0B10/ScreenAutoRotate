using System;
using System.Windows.Forms;
using System.Threading;
using System.IO.Ports;
using System.Linq;
using System.Configuration;

namespace ScreenAutoRotate
{
    class Program
    {

        private static Thread UIThread;
        private static SerialPort Serial;
        private static Tray r;
        static void Main(string[] args)
        {

            string[] Ports = SerialPort.GetPortNames().Where(name => name != "COM1").ToArray();
            if (Properties.Settings.Default.port == "undefined")
            {
                if (Ports.Length == 1)
                {
                    Properties.Settings.Default.port = Ports[0];
                    Properties.Settings.Default.Save();
                }
            }

            uint Connected = Display.Connected();

            UIThread = new Thread(delegate ()
            {
                r = new Tray(Ports, Connected);
                Application.Run();
            });
            UIThread.Start();
            Thread.Sleep(500);
            int i = Int32.Parse(ConfigurationManager.AppSettings.Get("display"));
            Serial = new SerialPort();
            Serial.PortName = Properties.Settings.Default.port;
            Serial.BaudRate = 9600;
            try
            {
                Serial.Open();
                Serial.DataReceived += Serial_DataReceived;
            } catch (SystemException e)
            {
                r.Notify("Kommunikation fehlgeschlagen!", $"{e.Source}: {e.Message}");
            }
        }

        private static void Serial_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            char[] input = Serial.ReadExisting().ToLower().ToCharArray();
            foreach (char c in input)
            {
                (long x, long y) = (0L, 0L);
                var display = Properties.Settings.Default.display;
                switch (c) {
                    case 'l':
                        Display.Rotate(display, Display.Orientations.DEGREES_CW_0);
                        if (r != null) r.Notify("Automatische Drehung", $"Bildschirm {display} ist nun im Querformat");
                        for (uint i = 1; i <= Display.Connected(); i++)
                        {
                            (x, y) = Position(true, i);
                            Display.Move(i, x, y);
                        }
                        break;
                    case 'p':
                        Display.Rotate(display, Display.Orientations.DEGREES_CW_270);
                        if (r != null) r.Notify("Automatische Drehung", $"Bildschirm {display} ist nun Hochkant");
                        for (uint i = 1; i <= Display.Connected(); i++)
                        {
                            (x, y) = Position(false, i);
                            Display.Move(i, x, y);
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        private static (long, long) Position(bool LandscapeMode, uint Display)
        {
            string Format = LandscapeMode ? "landscape" : "portrait";
            object x = Properties.Settings.Default[$"display{Display}_{Format}_pos_x"];
            object y = Properties.Settings.Default[$"display{Display}_{Format}_pos_y"];
            if (x == null)
            {
                x = 0L;
            }
            if (y == null)
            {
                y = 0L;
            }
            return ((long) x, (long) y);
        }
    }
}
