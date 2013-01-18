using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace editor.ui
{    
    public partial class TimeLine : UserControl    
    {
        public TimeLine()
        {
            InitializeComponent();               
        }

        private void moveButtonOnX(int x)
        {
            int buttons_x = this.buttonMoveLeft.Location.X;
            int buttons_y = this.buttonMoveLeft.Location.Y;
            int buttons_c = this.buttonControl.Location.X + this.buttonControl.Size.Width / 2;
            int buttons_w = (buttons_c - buttons_x) * 2;

            int dx = x - buttons_c;

            if (buttons_x + dx <= 0)
            {
                this.buttonMoveLeft.Location = new Point(0, buttons_y);
                this.buttonControl.Location = new Point(this.buttonMoveLeft.Size.Width, buttons_y);
                this.buttonMoveRight.Location = new Point(this.buttonControl.Location.X + this.buttonControl.Size.Width, buttons_y); 
            }
            else if (buttons_x + buttons_w + dx>= this.Width)
            {
                this.buttonMoveRight.Location = new Point(this.Width - this.buttonMoveRight.Size.Width, buttons_y);
                this.buttonControl.Location = new Point(this.buttonMoveRight.Location.X - this.buttonControl.Size.Width, buttons_y);
                this.buttonMoveLeft.Location = new Point(this.buttonControl.Location.X - this.buttonMoveLeft.Size.Width, buttons_y);
            }
            else
            {

            }

        }

        private void setCurrentFrameOnX(int x)
        {
            if (x >= this.pictureBoxBack.MarkStartX && x <= this.pictureBoxBack.MarkEndX)
            {
                int mark_x = x - this.pictureBoxBack.MarkStartX + this.pictureBoxBack.OffsetX;
                float f = mark_x / this.pictureBoxBack.ClipStep;
                this.pictureBoxBack.CurrentFrame = (int)Math.Round(f);
            }
        }

        private void pictureBoxBack_MouseClick(object sender, MouseEventArgs e)
        {
            int x = e.X;
            int y = e.Y;

            if (y > TimeLinePictureBox.TIME_LINE_START_Y)
            {
                setCurrentFrameOnX(x);
            }
        }
    }

    class ButtonGroup
    {
        public ButtonGroup(Button bl,Button bm,Button br)
        {
            buttonl = bl;
            buttonm = bm;
            buttonr = br;
        }

        public void moveToX(int x)
        {

        }

        public System.Windows.Forms.Button buttonl;
        public System.Windows.Forms.Button buttonm;
        public System.Windows.Forms.Button buttonr;
    }

    class TimeLinePictureBox : PictureBox
    {
        public const int TIME_LINE_START_X = 10;
        public const int TIME_LINE_START_Y = 30;

        public const int TIME_LINE_SHORTLINE_LEN = 10;
        public const int TIME_LINE_LONELINE_LEN = 20;

        public const int MARK_LINE_START_X = TIME_LINE_START_X + 10;

        public const int CLIP_STEP_MIN = 2;

        public TimeLinePictureBox()
            : base()
        {

        }


        int mFrameNum = 100;
        int mCurrentFrame = 0;

        float mClipStep = CLIP_STEP_MIN;

        int mMarkStartX = MARK_LINE_START_X;
        int mMarkEndX = 100 - MARK_LINE_START_X;
        int mMarkLength = 100;

        int mTimeLineAxisStart = TIME_LINE_START_X;
        int mTimeLineAxisEnd = 100 - TIME_LINE_START_X;
        int mTimeLineAxisLength = 100;

        int mOffsetX = 0;
        float mScaleX = 1.0f;

        Pen mWLinePen = new Pen(Color.Black, 2);
        Pen mSLinePen = new Pen(Color.Black, 1);
        Pen mCFramePen = new Pen(Color.Black, 2);
        SolidBrush mFontBrush = new SolidBrush(Color.Black);
        Font mFont = new Font("宋体", 10);

        protected void update()
        {
            mTimeLineAxisStart = TIME_LINE_START_X;
            mTimeLineAxisEnd = this.Width - TIME_LINE_START_X;
            mTimeLineAxisLength = this.Width - 2 * TIME_LINE_START_X;

            mMarkStartX = MARK_LINE_START_X;
            mMarkEndX = mTimeLineAxisEnd - MARK_LINE_START_X;
            mMarkLength = mMarkEndX - mMarkStartX;

            mClipStep = (float)mMarkLength / (float)mFrameNum * mScaleX;
        }

        protected void drawLines(Graphics g)
        {
            g.DrawLine(mWLinePen, new Point(mTimeLineAxisStart, TIME_LINE_START_Y), new Point(mTimeLineAxisEnd, TIME_LINE_START_Y));

            int show_min_num = mFrameNum;
            int show_max_num = 0;

            for (int i = 0; i <= mFrameNum; i++)
            {
                int px = (int)(mClipStep * i) + mOffsetX + mMarkStartX;

                if (px >= mMarkStartX && px <= mMarkEndX)
                {
                    show_min_num = i <= show_min_num ? i : show_min_num;
                    show_max_num = i >= show_max_num ? i : show_max_num;
                }
            }

            int frame_num = show_max_num - show_min_num;

            int num_w = 1; 
            float step = mClipStep;
            while (step < CLIP_STEP_MIN && num_w < 4096)
            {
                step = step * CLIP_STEP_MIN;
                num_w += 1;
            }

            int mark_w = frame_num / 10 * num_w;
            mark_w = mark_w != 0 ? mark_w : 1;

            for (int i = show_min_num; i <= show_max_num; i += num_w)
            {
                int px0 = (int)(mClipStep * i) + mOffsetX + mMarkStartX;
                int py0 = TIME_LINE_START_Y;

                int px1 = px0;
                int py1 = (mark_w != 1) && (i % mark_w) == 0 ? py0 + 2 * TIME_LINE_SHORTLINE_LEN : py0 + TIME_LINE_SHORTLINE_LEN;

                g.DrawLine(mSLinePen, new Point(px0, py0), new Point(px1, py1));

                if (i % mark_w == 0)
                {
                    g.DrawString(i.ToString(), mFont, mFontBrush, new PointF(px1 - 7, py1 + 3));
                }
            }
        }

        protected void drawCurrentFrame(Graphics g)
        {
            int px = (int)(mClipStep * mCurrentFrame) + mOffsetX + mMarkStartX;

            if (px >= mMarkStartX && px <= mMarkEndX)
            {
                int x = px - 2;
                int y = TIME_LINE_START_Y + 2;
                int w = 5;
                int h = 15;

                g.DrawRectangle(mCFramePen, new Rectangle(x, y, w, h));    
            }
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);

            update();

            drawLines(pe.Graphics);
            drawCurrentFrame(pe.Graphics);
        }

        public int TotalFrame
        {
            get { return mFrameNum; }
            set
            {
                mFrameNum = value;
                this.Refresh();
            }
        }

        public int CurrentFrame
        {
            get { return mCurrentFrame; }
            set
            {
                mCurrentFrame = value;
                this.Refresh();
            }
        }

        public float ScaleX
        {
            get { return mScaleX; }
            set
            {
                mScaleX = value;
                this.Refresh();
            }
        }

        public int OffsetX
        {
            get { return mOffsetX; }
            set
            {
                mOffsetX = value;
                this.Refresh();
            }
        }

        public int MarkStartX
        {
            get { return mMarkStartX; }
        }

        public int MarkEndX
        {
            get { return mMarkEndX; }
        }

        public float ClipStep
        {
            get { return mClipStep; }
        }

    }
}
