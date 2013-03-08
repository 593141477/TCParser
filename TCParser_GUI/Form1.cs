using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.IO;

namespace TCParser_GUI
{
    public partial class Form1 : Form
    {
        private string html_filename;
        private string data_folder;
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog op = new OpenFileDialog();
            op.Filter = "HTML数据页面(*.html;*.htm)|*.html;*.htm";
            op.ValidateNames = true;
            op.CheckFileExists = true;
            op.CheckPathExists = true;
            op.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyComputer);
            if (op.ShowDialog() == DialogResult.OK) 
                html_filename = op.FileName;

        }

        private void button2_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            if (dlg.ShowDialog() == DialogResult.OK) data_folder = dlg.SelectedPath;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            label9.Text = "";
            if (html_filename == null)
            {
                MessageBox.Show("请选择html文件", "提示");
                return;
            }
            if (data_folder == null)
            {
                MessageBox.Show("请选择数据目录","提示");
                return;
            }

            Stream sm = Assembly.GetExecutingAssembly().GetManifestResourceStream("TCParser_GUI.parse.exe");
            byte[] buf = new byte[sm.Length];
            sm.Read(buf, 0, (int)sm.Length);
            sm.Close();

            FileStream exe_file = new FileStream("parse.exe", FileMode.Create);
            exe_file.Write(buf, 0, buf.Length);
            exe_file.Close();
            buf = null;


            System.Diagnostics.Process p = new System.Diagnostics.Process();
            p.StartInfo = new System.Diagnostics.ProcessStartInfo();
            p.StartInfo.FileName = "parse.exe";
            p.StartInfo.Arguments = "\"" + html_filename + "\" \"" + data_folder + "\"";
            p.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;
            p.Start();

            Application.DoEvents();

            StreamReader reader = p.StandardOutput; // 截取输出流 
            string line = reader.ReadLine(); // 每次读取一行 
            reader.Close();

            if (line == null)
                line = "";
            if (line.IndexOf("Invalid") != -1)
            {
                label9.Text = "转换失败";
            }
            else if (line.IndexOf("Groups") != -1)
            {
                label9.Text = "转换成功";
            }
            else
            {
                label9.Text = "程序异常";
            }

            Application.DoEvents();
            if (!p.WaitForExit(100))
                p.Kill();

            File.Delete("parse.exe");

            html_filename = null;
            data_folder = null;
        }

    }
}
