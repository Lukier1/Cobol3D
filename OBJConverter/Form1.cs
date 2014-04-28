using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
namespace OBJConverter
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            label1.Text = "Otworzono: " + openFD.FileName;
            System.Globalization.CultureInfo ci = new System.Globalization.CultureInfo("pl", true);
            ci.NumberFormat.NumberDecimalSeparator = ".";
            Application.CurrentCulture = ci;
        }
        String convertResult = "";
        String mModelName, mObjectName, mMaterialName;
      
        private bool ConvertModel(String fileName)
        {
           string content = System.IO.File.ReadAllText(fileName);
           string vertexList = "";
           string[] linesFile = System.IO.File.ReadAllLines(fileName);
           List<float> vertexPos = new List<float>(), texPos = new List<float>(), norPos = new List<float>();
           int numberOfVertex = 0;
           foreach (string line in linesFile)
            {
               string[] items = line.Split(' ');
               if (items.Length != 0)
               {
                   switch (items[0])
                   {
                       case "mtllib":
                           mModelName = items[1];
                           break;
                       case "o":
                           mObjectName = items[1];
                           break;
                       case "usemtl":
                           mMaterialName = items[1];
                           break;
                       case "s":
                           break;
                       case "v":
                           vertexPos.Add(float.Parse(items[1],System.Globalization.CultureInfo.InvariantCulture));
                           vertexPos.Add(float.Parse(items[2], System.Globalization.CultureInfo.InvariantCulture));
                           vertexPos.Add((-1.0f)*float.Parse(items[3], System.Globalization.CultureInfo.InvariantCulture));
                           break;
                       case "vt":
                           texPos.Add(float.Parse(items[1], System.Globalization.CultureInfo.InvariantCulture));
                           texPos.Add(1.0f-float.Parse(items[2], System.Globalization.CultureInfo.InvariantCulture));
                           break;
                       case "vn":
                           norPos.Add(float.Parse(items[1], System.Globalization.CultureInfo.InvariantCulture));
                           norPos.Add(float.Parse(items[2], System.Globalization.CultureInfo.InvariantCulture));
                           norPos.Add((-1.0f)*float.Parse(items[3], System.Globalization.CultureInfo.InvariantCulture));
                           break;
                       case "f":
                           String[] triangleElements = new String[3];
                           for(int i = 0; i < 3; i++) {
                               ++numberOfVertex;
                                string[] subItems = items[i+1].Split('/');
                                int[] verNums = { (int.Parse(subItems[0])-1)*3,  (int.Parse(subItems[1])-1)*2,  (int.Parse(subItems[2])-1)*3 };
                                triangleElements[i] = vertexPos[verNums[0]].ToString() + " " + vertexPos[verNums[0] + 1].ToString() + " " + vertexPos[verNums[0] + 2].ToString() + " " +
                                    texPos[verNums[1]].ToString() + " " + texPos[verNums[1] + 1].ToString() + " " +
                                     norPos[verNums[2]].ToString() + " " + norPos[verNums[2] + 1].ToString() + " " + norPos[verNums[2] + 2].ToString() + "\n";
                           }
                           vertexList+= triangleElements[2] + triangleElements[1] + triangleElements[0];
                           break;
                   }
               }
            }
           convertResult += "Vertex Count: " + numberOfVertex.ToString() + "\n\nData:\n\n" + vertexList;
            //MessageBox.Show(linesFile[0]);
            return true;
        }
        private void LoadButton_Click(object sender, EventArgs e)
        {
            DialogResult result = openFD.ShowDialog();
            if (result == DialogResult.OK)
            {
                label1.Text = "Otworzono: " + openFD.FileName;
                ConvertModel(openFD.FileName);
            }
            Console.WriteLine(result);

        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            if (convertResult.Length != 0)
            {
                System.IO.File.Delete("C:/Projekty/CPP/Cobol3D/Cobol3D/data/" + FileNameTB.Text + ".cobM");
                System.IO.File.WriteAllText("C:/Projekty/CPP/Cobol3D/Cobol3D/data/" + FileNameTB.Text + ".cobM", convertResult);
                MessageBox.Show("Prawidlowo zapisano do pliku");
            }
            else
                MessageBox.Show("Nie podano pliku .obj");
        }
    }
}
