using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Media.Animation;
using System.Xml.Schema;
using System.Diagnostics;
using System.Drawing;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {   
            InitializeComponent();
            
        }

        Luckywheel luckywheel;
        private void Button_Click(object sender, RoutedEventArgs e)
        {   
            if (luckywheel == null)
            {
                luckywheel = new Luckywheel(wheel,start,showresult);
                luckywheel.Luckytime();
            }
            else
                luckywheel.Luckytime();


        }


    }
 


    public class Luckywheel
    {
        private TextBlock showshowresult;
        private int lastdraw;
        private Canvas wheel;
        private Button start;
        private Ellipse[] ellipse=new Ellipse[4];
        private List<MyList> wheelcomponent;
        public int currentgain {
            get;
            private set;
        }
        private class MyList
        {
            public string Color { get; set; }
            public int Money { get; set; }
        }

        public Luckywheel(Canvas wheel,Button start,TextBlock showshowresult)
        {
            currentgain = 1000;
            this.showshowresult = showshowresult;
            this.showshowresult.Text = "You have $1000, please click start!";
            lastdraw = -1;
            this.wheel = wheel;
            
            this.start = start;
            

            Initializingcolor();
            
        }
      
        private void Initializingcolor()
        {
            wheelcomponent = new List<MyList>();
            wheelcomponent.Add(new MyList() { Color = "Red", Money = 100 });
            wheelcomponent.Add(new MyList() { Color = "Blue", Money = 10 });
            wheelcomponent.Add(new MyList() { Color = "Green", Money = 0 });
            wheelcomponent.Add(new MyList() { Color = "Gray", Money = -500 });//add color to list first
            wheelcomponent = wheelcomponent.OrderBy(x => Guid.NewGuid()).ToList();//randomize the color
            BrushConverter bc = new BrushConverter();

            
                for (int i = 1; i < 5; i++)//change color of the display
            {
                ellipse[i - 1] = wheel.Children[i] as Ellipse;
                Debug.WriteLine(ellipse[i-1].Fill);
                Color color = (Color)ColorConverter.ConvertFromString(wheelcomponent[i-1].Color);
                
                bc.ConvertFrom(wheelcomponent[i - 1].Color);
                ellipse[i - 1].Fill =(Brush)bc.ConvertFrom(wheelcomponent[i - 1].Color); 
            }

        }

        private void Reset()//reset the properties
        {
            currentgain = 1000;
            showshowresult.Text = "You have $1000, please click start!";
            lastdraw = -1;
            start.Content = "Start";
            Initializingcolor();

        }

        async Task PutTaskDelay()
        {
            await Task.Delay(4000);
        }

        public async void Luckytime()//the main body of starting lucky draw
        {
            if (currentgain <= 0)//check if there is any money left
            {
                Reset();
                return;
            }
            start.IsEnabled = false;
            Random rand = new Random();
            int drawed = rand.Next(0, 4);

            if (lastdraw == -1)
            {
                Rotatewheel(0, 1440 + 90 * drawed);
            }
            else
            {   
                Rotatewheel(90 * (lastdraw), 1440+(drawed*90) );//pass the anlge to the display method
            }
            await PutTaskDelay(); //use async way to delay and do the animation at the same time
            payout(drawed);
            lastdraw = drawed;


        }
        private void payout(int index)
        {
            
            currentgain += wheelcomponent[index].Money;//update the gain
            updatemessage();//update display
            start.IsEnabled = true;
        }

        private void updatemessage()//display
        {
            string tmp;
            if (currentgain >= 0)
            {
                tmp = "Your current gain is: " + currentgain.ToString();

            }
            else
            {
                tmp = "No money left, reset the game please!";
                start.Content = "Reset";
            }
            showshowresult.Text = tmp;


        }

    
        private void Rotatewheel(int start, int end)//the display method
        {   

            DoubleAnimation rotateAnimation = new DoubleAnimation(start, end, new Duration(TimeSpan.FromSeconds(4)));//rotate to the calculated angle
            rotateAnimation.DecelerationRatio = 1;
            wheel.RenderTransformOrigin = new Point(0.5, 0.5);
            RotateTransform rt = new RotateTransform();
            wheel.RenderTransform = rt;
            rt.BeginAnimation(RotateTransform.AngleProperty, rotateAnimation);
            
        }

    }

}
