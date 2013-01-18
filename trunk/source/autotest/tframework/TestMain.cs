using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using System.Diagnostics;
using System.Windows.Forms;

namespace tframework
{
    public enum TestResultType
    {
        Passed,
        Failed,
        Yield,
    }

    public abstract class TestResult
    {
        public abstract TestResultType resultType { get; }
        public virtual string info { get{return "";} }
        public virtual object userData { get { return null; } }
    }

    public abstract class TestCase
    {
        public TestCase() { }
        public abstract string name();
        public abstract IEnumerable<TestResult> runSteps();
        public virtual void kill() { }
        public virtual bool isActived() { return true; }
        public virtual int weight() { return -1; }
        public void setParent(TestMain tm){mParent = tm;}

        protected TestMain mParent = null;
    }

    public class TestMain
    {
        public enum EnumCaseResult : int
        {
            ENUMCASE_PASSED_TONEXT = 0,
            ENUMCASE_FAILED_TONEXT ,
            ENUMCASE_OVER,
        };

        public delegate void OutPutStringFunc(string s);
        public delegate void OnTestPassedFunc(TestCase tcase);
        public delegate void OnTestFailedFunc(TestCase tcase);

        public TestMain() { }

        public bool initialize()
        {
            shutdown();

            OnOutPutInfo = null;
            OnOutPutInfo += _outputToConsole;
            OnTestFailed += _outputFailedToConsole;
            OnTestPassed += _outputPassedToConsole;
            return true;
        }

        public void shutdown()
        {
            if (mCaseIterator != null && mCaseIterator.Current != null && 
                mCaseCo != null && mCaseCo.Current != null)
            {
                mCaseIterator.Current.kill();
            }
            mCaseCo = null;
            mCaseIterator = null;
            mCases.Clear();
            mYieldTimer = null;
            OnOutPutInfo = null;
            OnTestFailed = null;
            OnTestPassed = null;
        }

        public Dictionary<string,List<TestCase>> loadTestCases(string plugin_path)
        {
            Dictionary<string, List<TestCase>> dic = new Dictionary<string, List<TestCase>>();

            mCases.Clear();
            string[] dll_names = Directory.GetFiles(plugin_path, "*.dll");
            foreach (string name in dll_names)
            {
                Assembly assembly = Assembly.LoadFrom(name);

                if (assembly == null)
                {
                    OnOutPutInfo("Load " + name + " Failed!");
                    continue;
                }

                string ass_name = assembly.ToString();
                if (!dic.ContainsKey(ass_name))
                    dic[ass_name] = new List<TestCase>();

                Type[] types = assembly.GetExportedTypes();
                foreach (Type t in types)
                {
                    if (t.BaseType != null && t.BaseType == typeof(TestCase))
                    {
                        try
                        {
                            TestCase tcase = assembly.CreateInstance(t.FullName) as TestCase;
                            if (tcase != null)
                            {
                                tcase.setParent(this);

                                if(tcase.isActived())
                                    dic[ass_name].Add(tcase);
                            }
                            else
                            {
                                OnOutPutInfo("CreateInstance " + t.FullName + " Failed!");
                            }
                        }
                        catch (System.Exception) { }
                    }
                }

                dic[ass_name].Sort(new TestCaseCompare());
            }

            return dic;
        }

        public EnumCaseResult enumerateCases()
        {
            if (mCases.Count == 0)
                return EnumCaseResult.ENUMCASE_OVER;

            if (mCaseIterator == null)
                mCaseIterator = mCases.GetEnumerator();

            if (mCaseIterator.Current == null)
            {
                if (!_moveNextTextCase())
                    return EnumCaseResult.ENUMCASE_OVER;
            }

            bool passed = false;
            if (mCaseIterator.Current != null)
            {
                bool next_case = false;

                try
                {
                    if (mCaseCo.Current == null)
                    {                        
                        OnOutPutInfo("--------------------------");
                        OnOutPutInfo("TestCase: " + mCaseIterator.Current.name() + " Started");

                        _moveNextCaseStep();
                    }

                    switch (mCaseCo.Current.resultType)
                    {
                        case TestResultType.Failed:
                            OnTestFailed(mCaseIterator.Current);
                            OnOutPutInfo("TestCase: " + mCaseIterator.Current.name() + " Failed ");
                            next_case = true;
                            passed = false;
                            break;
                        case TestResultType.Passed:
                            OnTestPassed(mCaseIterator.Current);
                            OnOutPutInfo("TestCase: " + mCaseIterator.Current.name() + " Passed ");
                            next_case = true;
                            passed = true;
                            break;
                        case TestResultType.Yield:
                            float time = (float)mCaseCo.Current.userData;
                            if (time == 0.0f)
                            {
                                _moveNextCaseStep();
                            }
                            else
                            {
                                if (mYieldTimer == null)
                                    mYieldTimer = new YieldTimer(time);

                                if (mYieldTimer.yieldOver())
                                {
                                    mYieldTimer = null;
                                    _moveNextCaseStep();
                                }
                            }
                            break;
                    }
                }
                catch (System.Exception ex)
                {
                    OnTestFailed(mCaseIterator.Current);
                    OnOutPutInfo("TestCase: " + mCaseIterator.Current.name() + " Failed in unknown issue !");
                    OnOutPutInfo("Failed Info:" + ex.Message);
                    OnOutPutInfo("Failed Stack:" + ex.StackTrace);
                    mYieldTimer = null;
                    passed = false;
                    next_case = true;
                }

                if (next_case)
                {
                    _moveNextTextCase();

                    OnOutPutInfo("--------------------------");
                    OnOutPutInfo("  ");
                }

            }

            return passed ? EnumCaseResult.ENUMCASE_PASSED_TONEXT : EnumCaseResult.ENUMCASE_FAILED_TONEXT;
        }

        private bool _moveNextTextCase()
        {
            if (mCaseIterator.MoveNext())
            {
                mCaseCo = mCaseIterator.Current.runSteps().GetEnumerator();
                return true;
            }
            else
                return false;
        }

        private bool _moveNextCaseStep()
        {
            if (mCaseCo.MoveNext())
            {
                if (mCaseCo.Current.info != null && mCaseCo.Current.info.Length != 0)
                    OnOutPutInfo(mCaseCo.Current.info);
                return true;
            }
            else
                return false;
        }

        public Form MainForm { get; set; }
        public List<TestCase> Cases { get { return mCases; } }

        protected void _outputToConsole(string s) { }
        protected void _outputPassedToConsole(TestCase tcase) { }
        protected void _outputFailedToConsole(TestCase tcase) { }

        YieldTimer mYieldTimer = null;
        IEnumerator<TestCase> mCaseIterator = null;
        IEnumerator<TestResult> mCaseCo = null;
        List<TestCase> mCases = new List<TestCase>();

        public event OnTestFailedFunc OnTestFailed = null;
        public event OnTestPassedFunc OnTestPassed = null;
        public event OutPutStringFunc OnOutPutInfo = null;
    }

    public class TestResultPassed : TestResult
    {
        public TestResultPassed() {}
        public override TestResultType resultType { get { return TestResultType.Passed; } }
    }

    public class TestResultFailed : TestResult
    {
        public TestResultFailed(string s) { mInfo = s; }
        public override string info { get { return mInfo; } }
        public override TestResultType resultType { get { return TestResultType.Failed; } }
        string mInfo = "";
    }

    public class TestResultYield : TestResult
    {
        public TestResultYield(float wait_secs) { mWaitTime = wait_secs;}
        public TestResultYield(string s, float wait_secs) { mInfo = s; mWaitTime = wait_secs; }
        public override string info { get { return mInfo; } }
        public override TestResultType resultType { get { return TestResultType.Yield; } }
        public override object userData { get { return mWaitTime; } }
        string mInfo = "";
        float mWaitTime = 0.0f;
    }
    
    class YieldTimer
    {
        public YieldTimer(float t_secs)
        {
            mYieldTime = t_secs > 0.0f ? t_secs : 0.0f;
            mWatch = Stopwatch.StartNew();
        }

        public bool yieldOver()
        {
            return (mWatch.ElapsedMilliseconds / 1000.0f) < mYieldTime ? false : true;
        }

        Stopwatch mWatch = null; 
        float mYieldTime = 0.0f;
    }

    public class TestCaseCompare : IComparer<TestCase>
    {
        public int Compare(TestCase x, TestCase y)
        {
            int xweight = x.weight();
            int yweight = y.weight();

            if (xweight != yweight)
                return yweight - xweight;

            string xname = x.name();
            string yname = y.name();

            return string.Compare(xname, yname, true);
        }
    }
}
