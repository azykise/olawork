using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace modelview
{
    using editor;
    class ActionManager 
    {
        public ActionManager(ModelViewDoc doc)
        {
            mDoc = doc;
        }

        public void update(float _elasped)
        {
            if (mDoc.CurrentDynamicModel == null)
            {
                return;
            }

            int total_frame = mDoc.CurrentDynamicModel.CurrentAction.FrameNumber;
            float total_time = mDoc.CurrentDynamicModel.CurrentAction.TotalTime;

            float clip_time = total_time / total_frame;

            if (mStart)
            {
                mPassedTime += _elasped;
                float passed_time = 0;
                if (mPassedTime <= total_time)
                {
                    passed_time = mPassedTime;
                }
                else
                {
                    if (Loop)
                    {
                        while (mPassedTime > total_time)
                            mPassedTime -= total_time;
                        passed_time = mPassedTime;
                    }
                    else
                    {
                        mStart = false;
                        mPassedTime = total_time;
                        passed_time = mPassedTime;                        
                    }
                }

                CurrentFrame = (int)(passed_time / total_time * total_frame);
                mDoc.CurrentDynamicModel.setPassedTime(passed_time);
                EditorGraph.postMessage(mDoc,new MessageBase("UIUpdateCurrentFrame",CurrentFrame.ToString()));
            }
            else
            {
                int current_frame = CurrentFrame;
                if (current_frame > total_frame)
                {
                    current_frame = total_frame;
                }

                float t = clip_time * current_frame;
                mPassedTime = t;

                mDoc.CurrentDynamicModel.setPassedTime(mPassedTime);
            }
        }

        public void startActionPlay()
        {
            if (mDoc.CurrentDynamicModel == null)
            {
                return;
            }

            mStart = true;
            float n = mDoc.CurrentDynamicModel.CurrentAction.TotalTime / mDoc.CurrentDynamicModel.CurrentAction.FrameNumber;
            mPassedTime = CurrentFrame * n;                
        }

        public void stopActionPlay()
        {
            mStart = false;
            CurrentFrame = 0;
            mPassedTime = 0;
        }

        public void pauseActionPlay()
        {
            mStart = false;
        }

        public bool isPlaying()
        {
            return mStart;
        }

        public bool Loop = false;
        public int CurrentFrame = 0;

        float mPassedTime = 0;
        bool mStart = false;
        ModelViewDoc mDoc = null;
    }
}
