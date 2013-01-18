using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace modelview.ui
{
    using modelview;
    partial class ActionTimeLine : UserControl
    {
        public ActionTimeLine()
        {
            InitializeComponent();
        }       

        public ActionTimeLineComponent EditorComponent = null;
        bool mLock = false;

        public void updateUI()
        {
            mLock = true;

            this.trackBarAction.Minimum = 0;
            this.trackBarAction.Maximum = this.EditorComponent.ActionLength;
            this.trackBarAction.Value = this.EditorComponent.CurrentFrame;

            _updateFrameLabelUIText();

            mLock = false;
        }

        public void resetUI()
        {
            mLock = true;

            this.buttonPause.Enabled = false;
            this.buttonPause.Visible = false;

            this.buttonPlay.Visible = true;
            this.buttonPlay.Enabled = true;

            this.trackBarAction.Minimum = 0;
            this.trackBarAction.Maximum = this.EditorComponent.ActionLength;
            this.trackBarAction.Value = this.EditorComponent.CurrentFrame;

            _updateFrameLabelUIText();

            mLock = false;
        }

        private void _updateFrameLabelUIText()
        {
            string s1 = EditorComponent.CurrentFrame.ToString();
            string s2 = EditorComponent.ActionLength.ToString();

            this.buttonLabel.Text = s1 + "/" + s2;
        }

        private void buttonPlay_Click(object sender, EventArgs e)
        {
            if (!EditorComponent.isAnimationEnable())
                return;

            this.buttonPlay.Enabled = false;
            this.buttonPlay.Visible = false;

            this.buttonPause.Enabled = true;
            this.buttonPause.Visible = true;

            this.trackBarAction.Enabled = false;

            EditorComponent.startActionPlay();
        }

        private void buttonPause_Click(object sender, System.EventArgs e)
        {
            this.buttonPause.Enabled = false;
            this.buttonPause.Visible = false;

            this.buttonPlay.Visible = true;
            this.buttonPlay.Enabled = true;

            this.trackBarAction.Enabled = true;

            EditorComponent.pauseActionPlay();
        }

        private void buttonStop_Click(object sender, System.EventArgs e)
        {
            this.buttonPause.Enabled = false;
            this.buttonPause.Visible = false;

            this.buttonPlay.Visible = true;
            this.buttonPlay.Enabled = true;

            this.trackBarAction.Enabled = true;

            mLock = true;
            this.trackBarAction.Value = 0;
            mLock = false;

            EditorComponent.stopActionPlay();
        }

        private void trackBarAction_Scroll(object sender, EventArgs e)
        {
            if (mLock)
                return;

            EditorComponent.setDynamicActorFrame(this.trackBarAction.Value);

            _updateFrameLabelUIText();
        }

        private void toolPlayMode_Click(object sender, EventArgs e)
        {
            EditorComponent.IsLoop = !EditorComponent.IsLoop;

            this.toolPlayMode.Text = EditorComponent.IsLoop ? "单次播放模式" : "循环播放模式";
        }
    }
}
