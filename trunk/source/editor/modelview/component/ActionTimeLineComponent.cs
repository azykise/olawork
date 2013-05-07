using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace modelview
{
    using editor;
    class ActionTimeLineComponent : EditorComponent
    {
        public ActionTimeLineComponent(ModelViewDoc doc) :
            base("ModelPropertyComponent")
        {
            Doc = doc;

            ActionTimeLineUI = new modelview.ui.ActionTimeLine();
            ActionTimeLineUI.EditorComponent = this;

            registMessage("UIUpdateCurrentFrame", onUIUpdateCurrentFrameMessage);
            registMessage("AfterModelCreated", onAfterModelCreatedMessage);
            registMessage("AfterModelActionChanged", onAfterModelActionChangedMessage);
        }

        public void setDynamicActorFrame(int frame)
        {
            CurrentFrame = frame;
            //if (Doc.CurrentDynamicModel != null)
            //{
            //    Doc.ActionMng.CurrentFrame = frame;
            //}
        }

        public bool isAnimationEnable()
        {
            return false;//Doc.CurrentDynamicModel != null;
        }

        public void startActionPlay()
        {
            Doc.ActionMng.startActionPlay();
        }

        public void stopActionPlay()
        {
            Doc.ActionMng.stopActionPlay();
        }

        public void pauseActionPlay()
        {
            Doc.ActionMng.pauseActionPlay();
        }

        public void resetUIByCurrentAction()
        {
            //if (Doc.CurrentDynamicModel != null)
            //{
            //    ActionLength = Doc.CurrentDynamicModel.CurrentAction.FrameNumber;
            //}
            //else
            //    ActionLength = 100;

            stopActionPlay();

            CurrentFrame = 0;
            ActionTimeLineUI.resetUI();
        }

        private void onUIUpdateCurrentFrameMessage(EditorComponent from, MessageBase msg)
        {
            if (msg.Messages.Count != 0)
            {
                string s = msg.Messages[0];
                if (int.TryParse(s, out CurrentFrame))
                {
                    ActionTimeLineUI.updateUI();
                }
            }
        }

        private void onAfterModelCreatedMessage(EditorComponent from, MessageBase msg)
        {
            resetUIByCurrentAction();
        }

        private void onAfterModelActionChangedMessage(EditorComponent from, MessageBase msg)
        {
            resetUIByCurrentAction();
        }

        protected bool mLoop = false;

        public ModelViewDoc Doc = null;
        public modelview.ui.ActionTimeLine ActionTimeLineUI = null;

        public int ActionLength = 100;
        public int CurrentFrame = 0;
        public bool IsLoop
        {
            get { return mLoop; }
            set 
            {
                mLoop = value;
                Doc.ActionMng.Loop = mLoop;
            }
        }
    }

}
