using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Gui.view.UserControls
{
    /// <summary>
    /// Interaction logic for ProgressBar.xaml
    /// </summary>
    public partial class ProgressBar : UserControl
    {
        private const double FullCircle = 360;

        public ProgressBar()
        {
            InitializeComponent();
        }

        public void UpdateProgress(int percentage)
        {
            double angle = FullCircle * percentage / 100;
            progressPath.Data = CreateArcGeometry(angle);
        }

        private Geometry CreateArcGeometry(double angle)
        {
            double radius = 32;
            double centerX = 38;
            double centerY = 38;

            double startAngle = -90; // Start from top
            double endAngle = startAngle + angle;

            PathGeometry pathGeometry = new PathGeometry();
            PathFigure pathFigure = new PathFigure();

            // Start point
            pathFigure.StartPoint = new Point(centerX, centerY - radius);

            // Arc segment
            ArcSegment arcSegment = new ArcSegment();
            arcSegment.Point = new Point(centerX + radius * Math.Cos(endAngle * Math.PI / 180), centerY + radius * Math.Sin(endAngle * Math.PI / 180));
            arcSegment.Size = new Size(radius, radius);
            arcSegment.IsLargeArc = angle > 180;
            arcSegment.SweepDirection = SweepDirection.Clockwise;
            arcSegment.RotationAngle = 0;
            pathFigure.Segments.Add(arcSegment);

            pathGeometry.Figures.Add(pathFigure);
            return pathGeometry;
        }
    }
}
