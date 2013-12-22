using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Xml;

namespace forComic
{
    public partial class Form1 : Form
    {   WebResponse response = null;
        StreamReader reader = null;
        string ComicUrl = string.Empty;
        string serviceUrl = "http://comic.131.com";

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ComicUrl = textBox_url.Text;
            bool Next = false;
            int j = Convert.ToInt16(textBox_volume.Text);//start Volume

            do
            {
                Directory.CreateDirectory(textBox_savePath.Text + "\\Chapter" + j);
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(ComicUrl);
                request.Method = "GET";
                response = request.GetResponse();
                reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
                string urlContent = reader.ReadToEnd();

                int maxNum = maxNumberOfPage(urlContent);

                for (int i = 1; i < maxNum + 1; i++)
                {
                    request = (HttpWebRequest)WebRequest.Create(ComicUrl.Replace("1.html", i + ".html"));
                    request.Method = "GET";
                    response = request.GetResponse();
                    reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
                    string currentVolumeContent = reader.ReadToEnd();


                    int a = currentVolumeContent.IndexOf("http://131js.131.com/channel/comic/img.js?");
                    a = a + "http://131js.131.com/channel/comic/img.js?".Length + 4;
                    string suffix = string.Empty;
                    int b = currentVolumeContent.IndexOf(".jpg\"><");
                    suffix = ".jpg";
                    if (b == -1)
                    {
                        b = currentVolumeContent.IndexOf(".png\"><");
                        suffix = ".png";
                    }

                    string url = currentVolumeContent.Substring(a, b - a);

                    url += suffix;

                    //downfile(url, "d:\\aa.jpg");

                    down(url, "d:\\" + textBox_name.Text + "\\Chapter" + j + "\\Chapter" + i + ".jpg");
                }

                hasNext(urlContent);
                j++;
            }
            while (!Next);
        }


        public void down(string url, string localpath)
        {
            string ural = url;
            string filepath = localpath;
            WebClient mywebclient = new WebClient();
            mywebclient.DownloadFile(ural, filepath);
            
        }

        public bool hasNext(string urlContent)
        {
            string result = urlContent;

            int a = result.IndexOf("观看同类漫画");
            
            //下一话：<a href="/content/2066/151303/1.html">第216话</a></span>
            a = result.IndexOf("下一话：<a href=");
            int b = result.Substring(a + "下一话：<a href=".Length).IndexOf("\">");
            string NextUrl = result.Substring(a + "下一话：<a href=".Length).Substring(1, b - 1);

            ComicUrl = serviceUrl + NextUrl;

            if (a == -1)
            {
                return true;
            }
            return false;
        }

        public int maxNumberOfPage(string urlContent)
        {
            string result = urlContent;
            int a = result.IndexOf("当前");

            //<p>当前 <em>2</em>/20</p>
            string b = result.Substring(a);
            a = b.IndexOf(">/");
            b = b.Substring(a, 10);

            string num = b.Substring(2,b.IndexOf("</")-2);


            return Convert.ToInt16(num);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderDlg = new FolderBrowserDialog();
            if (folderDlg.ShowDialog() == DialogResult.OK)
            {
                textBox_savePath.Text = folderDlg.SelectedPath;
            }
        }
    }
}
