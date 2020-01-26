using System;
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing;

namespace ScreenAutoRotate
{
	public class Tray
	{
		private readonly NotifyIcon icon;
		private readonly Container container;

		public Tray(string[] Ports, uint Displays)
		{
			container = new Container();
			icon = new NotifyIcon(container);
			icon.Icon = new Icon("icon.ico");
			icon.Text = "Automatic screen rotation";
			ContextMenu context = new ContextMenu();
			MenuItem settings = new MenuItem();
			settings.Index = 0;
			settings.Text = "Settings";
			MenuItem settings_port = new MenuItem();
			settings_port.Index = 0;
			settings_port.Text = "Port";
			int i = 0;
			foreach (string p in Ports)
			{
				MenuItem port = new MenuItem();
				port.Index = i++;
				port.Text = p;
				port.Checked = Properties.Settings.Default.port == p;
				port.Click += (o, e) =>
				{
					var old = Properties.Settings.Default.port;
					foreach (MenuItem item in settings_port.MenuItems) item.Checked = false;
					port.Checked = true;
					Properties.Settings.Default.port = p;
					Properties.Settings.Default.Save();
				};
				settings_port.MenuItems.Add(port);
			}
			settings.MenuItems.Add(settings_port);
			MenuItem settings_display = new MenuItem();
			settings_display.Index = 1;
			settings_display.Text = "Rotatable display";
			for (i=0; i<Displays; i++)
			{
				uint temp = (uint) i + 1;
				MenuItem display = new MenuItem();
				display.Index = i;
				display.Text = $"Display {temp}";
				display.Checked = Properties.Settings.Default.display == temp;
				display.Click += (o, e) => 
				{
					var old = Properties.Settings.Default.display;
					foreach (MenuItem item in settings_display.MenuItems) item.Checked = false;
					display.Checked = true;
					Properties.Settings.Default.display = temp;
					Properties.Settings.Default.Save();
				};
				settings_display.MenuItems.Add(display);
			}
			settings.MenuItems.Add(settings_display);
			MenuItem settings_position = new MenuItem();
			settings_position.Index = 2;
			settings_position.Text = "Save Display Positions";
			settings_position.Click += (o, e) =>
			{
				string Format = Display.LandscapeMode(Properties.Settings.Default.display) ? "landscape" : "portrait";
				for (i = 1; i <= Display.Connected(); i++)
				{
					(long x, long y) = Display.Position((uint)i);
					Properties.Settings.Default[$"display{i}_{Format}_pos_x"] = x;
					Properties.Settings.Default[$"display{i}_{Format}_pos_y"] = y;
					Properties.Settings.Default.Save();
				}
			};
			settings.MenuItems.Add(settings_position);
			context.MenuItems.Add(settings);
			MenuItem exit = new MenuItem();
			exit.Index = 1;
			exit.Text = "Beenden";
			exit.Click += (o, e) => 
			{
				Display.ResetAllRotations();
				Application.Exit();
			};
			context.MenuItems.Add(exit);
			icon.ContextMenu = context;
			icon.Visible = true;
		}

		public void Notify(string title, string message, ToolTipIcon icon, int time)
		{
			this.icon.ShowBalloonTip(time, title, message, icon);
		}

		public void Notify(string title, string message)
		{
			Notify(title, message, ToolTipIcon.Info, 0);
		}


	}
}