using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.UI;
using Windows.UI.Input;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Shapes;

namespace WMP_Project.Common
{
    class ManipulationInputProcessor
    {
        private GestureRecognizer gestureRecognizer;
        private UIElement element;
        private UIElement reference;
        private Point current;
        private Point moved;
        private TransformGroup cumulativeTransform;
        private MatrixTransform previousTransform;
        private CompositeTransform deltaTransform;
        private int nPos;

        public ManipulationInputProcessor(GestureRecognizer gr, UIElement target, UIElement referenceframe, int pos)
        {
            this.gestureRecognizer = gr;
            this.element = target;
            this.reference = referenceframe;
            this.nPos = pos;

            this.gestureRecognizer.GestureSettings =
                Windows.UI.Input.GestureSettings.Tap |
                Windows.UI.Input.GestureSettings.ManipulationTranslateX |
                Windows.UI.Input.GestureSettings.ManipulationTranslateY |
                Windows.UI.Input.GestureSettings.ManipulationRotate |
                Windows.UI.Input.GestureSettings.ManipulationScale |
                Windows.UI.Input.GestureSettings.ManipulationTranslateInertia |
                Windows.UI.Input.GestureSettings.ManipulationScaleInertia;

            // Set up pointer event handlers. These receive input events that are used by the gesture recognizer.
            this.element.PointerCanceled += OnPointerCanceled;
            this.element.PointerPressed += OnPointerPressed;
            this.element.PointerReleased += OnPointerReleased;
            this.element.PointerMoved += OnPointerMoved;

            // Set up event handlers to respond to gesture recognizer output
            this.gestureRecognizer.Tapped += OnTapped;
            this.gestureRecognizer.ManipulationStarted += OnManipulationStarted;
            this.gestureRecognizer.ManipulationUpdated += OnManipulationUpdated;
            this.gestureRecognizer.ManipulationCompleted += OnManipulationCompleted;
            InitializeTransforms();
        }
        void OnPointerPressed(object sender, Windows.UI.Xaml.Input.PointerRoutedEventArgs args)
        {
            // Route the events to the gesture recognizer
            // The points are in the reference frame of the canvas that contains the rectangle element.
            this.gestureRecognizer.ProcessDownEvent(args.GetCurrentPoint(this.reference));
            // Set the pointer capture to the element being interacted with
            this.element.CapturePointer(args.Pointer);
            // Mark the event handled to prevent execution of default handlers
            args.Handled = true;
        }
        void OnPointerReleased(object sender, Windows.UI.Xaml.Input.PointerRoutedEventArgs args)
        {
            this.gestureRecognizer.ProcessUpEvent(args.GetCurrentPoint(this.reference));
            args.Handled = true;
        }
        void OnPointerCanceled(object sender, Windows.UI.Xaml.Input.PointerRoutedEventArgs args)
        {
            this.gestureRecognizer.CompleteGesture();
            args.Handled = true;
        }
        void OnPointerMoved(object sender, Windows.UI.Xaml.Input.PointerRoutedEventArgs args)
        {
            this.gestureRecognizer.ProcessMoveEvents(args.GetIntermediatePoints(this.reference));
            args.Handled = true;
        }

        void OnTapped(object sender, TappedEventArgs e)
        {
            if (this.element is Rectangle) { ProcessTap(this.element as Rectangle); }
        }

        void ProcessTap(Rectangle shape)
        {
            MainPage.Current.TabProcess(nPos);
        }

        void ProcessMove(Rectangle shape, int direction)
        {
            MainPage.Current.MoveProcess(nPos, direction);
        }

        public void InitializeTransforms()
        {
            this.cumulativeTransform = new TransformGroup();
            this.deltaTransform = new CompositeTransform();
            this.previousTransform = new MatrixTransform() { Matrix = Matrix.Identity };

            this.cumulativeTransform.Children.Add(previousTransform);
            this.cumulativeTransform.Children.Add(deltaTransform);

            this.element.RenderTransform = this.cumulativeTransform;
        }
        void OnManipulationStarted(object sender, ManipulationStartedEventArgs e)
        {
            // get a current position
            current = e.Position;
        }
        void OnManipulationUpdated(object sender, ManipulationUpdatedEventArgs e)
        {
            // set a moved position
            moved = e.Position;
        }
        void OnManipulationCompleted(object sender, ManipulationCompletedEventArgs e)
        {
            // check current / moved position
            // check direction.. up / down / right / bottom.
            int nDirection = 0;

            //if (current.X > moved.X)
            //    nDirection = moveLeft;
            //else
            //    nDirection = moveRight;
            //if (current.Y > moved.Y)
            //    nDirection += moveUp;
            //else
            //    nDirection += moveDown;

            if (this.element is Rectangle) { ProcessMove(this.element as Rectangle, nDirection); }
        }
    }
}
