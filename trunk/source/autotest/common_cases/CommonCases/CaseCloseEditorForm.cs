using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using editor;
using Sharp3D.Math.Core;
using tframework;

namespace common_cases
{
    class CaseCloseEditorForm : TestCase
    {
        public CaseCloseEditorForm() { }

        public override string name()
        {
            return "CaseCloseEditorForm";
        }

        public override IEnumerable<TestResult> runSteps()
        {
            if(mParent.MainForm.Contains(CaseGlobal.sEditorForm))
                mParent.MainForm.RemoveOwnedForm(CaseGlobal.sEditorForm);

            CaseGlobal.sEditorForm.Close();
            CaseGlobal.sEditorForm.Dispose();
            CaseGlobal.sEditorForm = null;

            GC.Collect();
            GC.WaitForFullGCComplete();

            yield return new TestResultPassed();
        }
    }
}
